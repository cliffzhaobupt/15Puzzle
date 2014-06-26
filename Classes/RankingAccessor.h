//
//  RankingAccessor.h
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-18.
//
//

#ifndef ___5Puzzle__RankingAccessor__
#define ___5Puzzle__RankingAccessor__

#include "cocos2d.h"
#include "CocosGUI.h"
#include "HttpClient.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "prettywriter.h"
#include "reader.h"
#include "document.h"
#include "stringbuffer.h"

 
USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
using namespace rapidjson;

#define GET_RANKING_URL "http://0.0.0.0:3000/rankings/get"
#define POST_ADD_RANKING_URL "http://0.0.0.0:3000/rankings/add"

class RankingAccessor : public CCObject {
    //rapidjson::Document class
    //to hold local ranking
    Document _localRankings;
    //the absolute path of the json file
    const char * _fullJsonFilePath;
    //count of ranking items
    int _localRankingCount;
    //whether the ranking file is parsed successfully
    bool _localRankingValid;
    
    //document to hold global ranking
    Document _globalRankings;
    //count of global ranking
    int _globalRankingCount;
    //loading icon
    CCSprite * _loadingIcon;
    //network error icon
    CCSprite * _networkErrorIcon;
    //UIListView of global ranking list
    UIListView * _globalList;
    
    //is waiting for response to come
    bool _waitingForResponse;
    
    //http objects
    CCHttpClient * _client;
    CCHttpRequest * _getRankingRequest;
    CCHttpRequest * _addRankingRequest;
    
public:
    RankingAccessor(const char * const fileName);
    ~RankingAccessor();
    bool isLocalRankingValid() const;
    
    int getLocalRankingCount() const;
    const char * getLocalUserAtIndex(unsigned int i) const;
    int getLocalTimeAtIndex(unsigned int i) const;
    
    void insertRankingItem(const char * name, int time);
    
    static RankingAccessor * createRankingAccessor(const char * const fileName);
    
    void getRankingsFromServer(CCSprite * loadingIcon, CCSprite * networkError, UIListView * globalList);
    void callbackForGetRankingsFromServer(CCNode * node, CCObject * obj);
    
    int getGlobalRankingCount() const;
    const char * getGlobalUserAtIndex(unsigned int i) const;
    int getGlobalTimeAtIndex(unsigned int i) const;
    bool isWaitingForResponse();

    void clearUnfinishedRequests();
    void blankCallback(CCNode * node, CCObject * obj);
};

#endif /* defined(___5Puzzle__RankingAccessor__) */
