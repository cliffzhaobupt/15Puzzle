//
//  RankingAccessor.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-18.
//
//

#include "RankingAccessor.h"

//constructor
//parse json in the file
//set client and request objects
RankingAccessor::RankingAccessor(const char * const fileName) :
_globalRankingCount(0),
_waitingForResponse(false)
{
    unsigned long jsonFileSize;
    const char * jsonText = (const char *) CCFileUtils::sharedFileUtils()->getFileData(fileName, "r+", & jsonFileSize);
    _localRankings.Parse<kParseDefaultFlags>(jsonText);
    _fullJsonFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str();
    if (_localRankings.IsArray()) {
        _localRankingCount = _localRankings.Size();
        _localRankingValid = true;
    } else {
        _localRankingCount = 0;
        _localRankingValid = false;
    }
    
    //initialize CCHttpClient object
    _client = CCHttpClient::getInstance();
    
    //initialize CCHttpRequest object
    //for the GET request of getting global ranking
    _getRankingRequest = new CCHttpRequest;
    _getRankingRequest->setRequestType(CCHttpRequest::kHttpGet);
    _getRankingRequest->setUrl(GET_RANKING_URL);
    _getRankingRequest->setResponseCallback(this, callfuncND_selector(RankingAccessor::callbackForGetRankingsFromServer));
    
    //initialize CHttpRequest object
    //for the POST request of adding ranking to global ranking
    _addRankingRequest = new CCHttpRequest;
    _addRankingRequest->setRequestType(CCHttpRequest::kHttpPost);
    _addRankingRequest->setUrl(POST_ADD_RANKING_URL);
    
}

//static member function
RankingAccessor * RankingAccessor::createRankingAccessor(const char *const fileName) {
    while (true) {
        RankingAccessor * raCreated = new RankingAccessor(fileName);
        if (raCreated->isLocalRankingValid()) {
            raCreated->autorelease();
            return raCreated;
        }
        delete raCreated;
    }
}

//whether parsed successfully
bool RankingAccessor::isLocalRankingValid() const {
    return _localRankingValid;
}

//how many ranking items in the local ranking
int RankingAccessor::getLocalRankingCount() const {
    return _localRankingCount;
}

//get user name of one item in the local ranking
const char * RankingAccessor::getLocalUserAtIndex(unsigned int i) const {
    return _localRankings[i]["user"].GetString();
}

//get time of one item in the local ranking
int RankingAccessor::getLocalTimeAtIndex(unsigned int i) const {
    return _localRankings[i]["time"].GetInt();
}

//insert one item into the local ranking and global ranking
void RankingAccessor::insertRankingItem(const char *name, int time) {
    //save new ranking to local json file
    Document newRankings;
    newRankings.SetArray();
    
    Document::AllocatorType & allocator = newRankings.GetAllocator();
    
    bool checkWhetherInsert = true;
    for (unsigned int i = 0, size = getLocalRankingCount() ; i < size ; ++ i) {
        if (checkWhetherInsert) {
            if (time > getLocalTimeAtIndex(i)) {
                newRankings.PushBack(_localRankings[i], allocator);
            } else {
                Value newItem;
                newItem.SetObject();
                newItem.AddMember("user", name, allocator);
                newItem.AddMember("time", time, allocator);
                newRankings.PushBack(newItem, allocator);
                newRankings.PushBack(_localRankings[i], allocator);
                checkWhetherInsert = false;
            }
        } else {
            newRankings.PushBack(_localRankings[i], allocator);
        }
    }
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    newRankings.Accept(writer);
    const char * newRankingsJson = buffer.GetString();
    FILE * file = fopen(_fullJsonFilePath, "w+");
    fputs(newRankingsJson, file);
    fclose(file);
    
    ++ _localRankingCount;
    while (true) {
        _localRankings.Parse<kParseDefaultFlags>(newRankingsJson);
        if (_localRankings.IsArray()) {
            break;
        }
    }
    
    //generate post data
    //send add ranking POST request to the server
    CCString * requestDataStr = CCString::createWithFormat("name=%s&time=%d", name, time);
    _addRankingRequest->setRequestData(requestDataStr->getCString(), requestDataStr->length());
    _client->send(_addRankingRequest);

}

//send GET request to acquire the ranking from server
void RankingAccessor::getRankingsFromServer(CCSprite * loadingIcon, CCSprite * networkError, UIListView * globalList) {
    //send request
    _client->setTimeoutForConnect(10);
    _client->send(_getRankingRequest);
    
    //display loading icon
    _loadingIcon = loadingIcon;
    _loadingIcon->setVisible(true);
    
    //set CCListView of global ranking list for later use
    _globalList = globalList;
    
    //hide newwork error icon
    _networkErrorIcon = networkError;
    _networkErrorIcon->setVisible(false);
    
    _waitingForResponse = true;
    
}

//will be called after the response of getting global ranking arrived
void RankingAccessor::callbackForGetRankingsFromServer(CCNode *node, CCObject *obj) {
    
    _waitingForResponse = false;
    
    //hide loading icon
    if (_loadingIcon) {
        _loadingIcon->setVisible(false);
    }
    
    CCHttpResponse * response = (CCHttpResponse *) obj;

    //if the request failed
    //show network error icon
    if (! response->isSucceed()) {
        CCLog("Receive Error! %s", response->getErrorBuffer());
        _networkErrorIcon->setVisible(true);
        return;
    }
    
    //if the request success
    //hide network error icon
    _networkErrorIcon->setVisible(false);
    
    //parse response JSON data
    std::vector<char> * data = response->getResponseData();
    std::string dataStr = "";
    for (std::vector<char>::const_iterator iter = data->begin() ; iter != data->end() ; ++ iter) {
        dataStr += *iter;
    }
    
    do {
        _globalRankings.Parse<kParseDefaultFlags>(dataStr.c_str());
        _globalRankingCount = _globalRankings.Size();
    } while (! _globalRankings.IsArray());
    
    for (int i = 0, len = getGlobalRankingCount() ; i < len ; ++i) {
        UILabel * rankingLabel = UILabel::create();
        CCString * text = CCString::createWithFormat("%d  -  %s  -  %ds",(i + 1), getGlobalUserAtIndex(i), getGlobalTimeAtIndex(i));
        rankingLabel->setText(text->getCString());
        rankingLabel->setFontSize(40);
        rankingLabel->setFontName("Marker Felt");
        //push the UILabel to UIListView
        _globalList->pushBackCustomItem(rankingLabel);
    }
}

//how many ranking items in the global ranking
int RankingAccessor::getGlobalRankingCount() const {
    return _globalRankingCount;
}

//get user name of one item in the local ranking
const char * RankingAccessor::getGlobalUserAtIndex(unsigned int i) const {
    return _globalRankings[i]["name"].GetString();
}

//get time of one item in the global ranking
int RankingAccessor::getGlobalTimeAtIndex(unsigned int i) const {
    return _globalRankings[i]["time"].GetInt();
}

//is waiting for the response of getting global ranking request
bool RankingAccessor::isWaitingForResponse() {
    return _waitingForResponse;
}

//when user exit the ranking scene
//we clear unfinished request callbacks
//by replacing the original callbacks of unfinished request
//of blank callback
void RankingAccessor::clearUnfinishedRequests() {
    _getRankingRequest->setResponseCallback(this, httpresponse_selector(RankingAccessor::blankCallback));
}

//blank callback
void RankingAccessor::blankCallback(cocos2d::CCNode *node, cocos2d::CCObject *obj) {

}

RankingAccessor::~RankingAccessor() {
    _getRankingRequest->release();
    _addRankingRequest->release();
}