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
RankingAccessor::RankingAccessor(const char * const fileName) :
_globalRankingCount(0),
_waitingForResponse(false),
_loadingIcon(NULL),
_destroyClient(false)
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
    
    _clientInClass = CCHttpClient::getInstance();
    _getRequest = new CCHttpRequest;
    _getRequest->setRequestType(CCHttpRequest::kHttpGet);
    _getRequest->setUrl("http://0.0.0.0:3000/rankings/get");
    _getRequest->setResponseCallback(this, callfuncND_selector(RankingAccessor::callbackForGetRankingsFromServer));
    
    _postRequest = new CCHttpRequest;
    _postRequest->setRequestType(CCHttpRequest::kHttpPost);
    _postRequest->setUrl("http://0.0.0.0:3000/rankings/add");
    
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

//how many ranking items in the ranking
int RankingAccessor::getLocalRankingCount() const {
    return _localRankingCount;
}

//get user name of one item in the ranking
const char * RankingAccessor::getLocalUserAtIndex(unsigned int i) const {
    return _localRankings[i]["user"].GetString();
}

//get time of one item in the ranking
int RankingAccessor::getLocalTimeAtIndex(unsigned int i) const {
    return _localRankings[i]["time"].GetInt();
}

//insert one item into the ranking
void RankingAccessor::insertRankingItem(const char *name, int time) {
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
    
    CCString * requestDataStr = CCString::createWithFormat("name=%s&time=%d", name, time);
    _postRequest->setRequestData(requestDataStr->getCString(), requestDataStr->length());
    _clientInClass->send(_postRequest);

}

void RankingAccessor::getRankingsFromServer(CCSprite * loadingIcon, UIListView * globalList) {
    
//    CCHttpClient * httpClient = CCHttpClient::getInstance();

    _clientInClass->setTimeoutForConnect(10);
    _clientInClass->send(_getRequest);
    
    _loadingIcon = loadingIcon;
    _loadingIcon->setVisible(true);
    
    _globalList = globalList;
    _waitingForResponse = true;
    _destroyClient = true;
    
}

void RankingAccessor::callbackForGetRankingsFromServer(CCNode *node, CCObject *obj) {
//    CCHttpClient * httpClient = (CCHttpClient *) node;
//    if(httpClient != _clientInClass || _clientInClass == NULL || node == NULL) return;
//    if (! node->isEqual(_clientInClass) ) {
//        return;
//    }
    
    if (_loadingIcon) {
        _loadingIcon->setVisible(false);
    }
    
    CCHttpResponse * response = (CCHttpResponse *) obj;
    if (! response->isSucceed()) {
        CCLog("Receive Error! %s", response->getErrorBuffer());
        return;
    }
    
    std::vector<char> * data = response->getResponseData();
    std::string dataStr = "";
    for (int i = 0, len = data->size(); i < len ; i ++) {
        dataStr += (*data)[i];
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
    _waitingForResponse = false;
}

int RankingAccessor::getGlobalRankingCount() const {
    return _globalRankingCount;
}

const char * RankingAccessor::getGlobalUserAtIndex(unsigned int i) const {
    return _globalRankings[i]["name"].GetString();
}

int RankingAccessor::getGlobalTimeAtIndex(unsigned int i) const {
    return _globalRankings[i]["time"].GetInt();
}

bool RankingAccessor::isWaitingForResponse() {
    return _waitingForResponse;
}

void RankingAccessor::clearUnfinishedRequests() {
    _getRequest->setResponseCallback(this, httpresponse_selector(RankingAccessor::blankCallback));
}

void RankingAccessor::blankCallback(cocos2d::CCNode *node, cocos2d::CCObject *obj) {
    CCLog("Blank callback is executed");
}

RankingAccessor::~RankingAccessor() {
    CCLog("Ranking Accessor Destructor called");
    _getRequest->release();
    _postRequest->release();
}