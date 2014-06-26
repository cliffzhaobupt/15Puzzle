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

enum {kUIScrollListWidgetTag, kGlobalIconsHolderZOrder};

class RankingLayer : public CCLayer {
    
    CCSize _screenSize;
    
    //JSON reader/writer
    RankingAccessor * _rankingAccessor;
    
    //scroll view displaying the local ranking items
    UIListView * _localRankingList;
    //scroll view displaying the global ranking items
    UIListView * _globalRankingList;
    //UILayer to contain the scroll views
    UILayer * _uiLayer;

    //local ranking / global ranking
    //tab buttons
    CCMenu * _rankingTabs;
    CCMenuItemFont * _localRankingTab;
    CCMenuItemFont * _globalRankingTab;
    
    //back to menu button
    CCMenu * _bottomBtns;
    CCMenuItemFont * _backToMenuBtn;
    
    //loading icon and network error icon
    //in global ranking
    CCSprite * _loadingIcon;
    CCLabelTTF * _networkErrorIcon;
    CCSprite * _globalIconsHolder;
    




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
