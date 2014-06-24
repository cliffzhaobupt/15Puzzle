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

//enum UILayerZOrder {kLowerZOrder, kUpperZOrder};

class RankingLayer : public CCLayer {
    //JSON reader/writer
    RankingAccessor * _rankingAccessor;
    
    //scroll view displaying the ranking items
    UIListView * _localRankingList;
    UILayer * _uiLayer;
    CCSize _screenSize;
    
    //local ranking / global ranking
    //tab buttons
    CCMenu * _rankingTabs;
    CCMenuItemFont * _localRankingTab;
    CCMenuItemFont * _globalRankingTab;
    
    //back to menu button
    CCMenu * _bottomBtns;
    CCMenuItemFont * _backToMenuBtn;
    
    CCSprite * _loadingIcon;
    
    UIListView * _globalRankingList;
//    UILayer * _globalUILayer;


public:
    virtual bool init();
    CREATE_FUNC(RankingLayer);
    static CCScene * scene();
    
    ~RankingLayer();
    
    void rankingTabHandler(CCObject * pSender);
    void backToMenuHandler(CCObject * pSender);
    
    void setOptionsForUIListView (UIListView * uiListView) const;

};

#endif /* defined(___5Puzzle__RankingLayer__) */
