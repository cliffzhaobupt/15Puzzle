//
//  RankingLayer.h
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-18.
//
//

#ifndef ___5Puzzle__RankingLayer__
#define ___5Puzzle__RankingLayer__

#include "cocos2d.h"
#include "RankingAccessor.h"
#include "CocosGUI.h"
#include "MenuLayer.h"

USING_NS_CC;
using namespace gui;

class RankingLayer : public CCLayer {
    
    RankingAccessor * _localRankingAccessor;
    UIListView * _localRankingList;
    UILayer * _uiLayer;
    CCSize _screenSize;
    CCMenu * _rankingTabs;
    CCMenuItemFont * _localRankingTab;
    CCMenuItemFont * _globalRankingTab;
    CCMenu * _bottomBtns;
    CCMenuItemFont * _backToMenuBtn;

public:
    virtual bool init();
    CREATE_FUNC(RankingLayer);
    static CCScene * scene();
    
    ~RankingLayer();
    
    void rankingTabHandler(CCObject * pSender);
    void backToMenuHandler(CCObject * pSender);
};

#endif /* defined(___5Puzzle__RankingLayer__) */
