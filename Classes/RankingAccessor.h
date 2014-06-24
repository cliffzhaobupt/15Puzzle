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

class RankingAccessor : public CCObject {
    //rapidjson::Document class
    //to hold the whole JSON content
    Document _localRankings;
    //the absolute path of the json file
    const char * _fullJsonFilePath;
    //count of ranking items
    int _localRankingCount;
    //whether the ranking file is parsed successfully
    bool _localRankingValid;
    

    
    Document _globalRankings;
    
    int _globalRankingCount;
    
    CCSprite * _loadingIcon;
    
    UIListView * _globalList;
    
    bool _waitingForResponse;
    
    bool _destroyClient;
    
    CCHttpClient * _clientInClass;
    CCHttpRequest * _getRequest;
    CCHttpRequest * _postRequest;
    
public:
    RankingAccessor(const char * const fileName);
    ~RankingAccessor();
    bool isLocalRankingValid() const;
    
    int getLocalRankingCount() const;
    const char * getLocalUserAtIndex(unsigned int i) const;
    int getLocalTimeAtIndex(unsigned int i) const;
    
    void insertRankingItem(const char * name, int time);
    
    static RankingAccessor * createRankingAccessor(const char * const fileName);
    
    void getRankingsFromServer(CCSprite * loadingIcon, UIListView * globalList);
    void callbackForGetRankingsFromServer(CCNode * node, CCObject * obj);
    
    int getGlobalRankingCount() const;
    const char * getGlobalUserAtIndex(unsigned int i) const;
    int getGlobalTimeAtIndex(unsigned int i) const;
    bool isWaitingForResponse();

    void clearUnfinishedRequests();
    void blankCallback(CCNode * node, CCObject * obj);
};

#endif /* defined(___5Puzzle__RankingAccessor__) */
