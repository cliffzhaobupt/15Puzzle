//
//  RankingLayer.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-18.
//
//

#include "RankingLayer.h"

RankingLayer::~RankingLayer() {
//    delete _rankingAccessor;
    CC_SAFE_RELEASE(_globalRankingList);
    CC_SAFE_RELEASE(_localRankingList);
    CC_SAFE_RELEASE(_rankingAccessor);
}

bool RankingLayer::init() {
    if (! CCLayer::init()) {
        return false;
    }
    //get current screen size
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    //initialize json reader / writer
    _rankingAccessor = RankingAccessor::createRankingAccessor("LocalRanking.json");
    _rankingAccessor->retain();
    //initialize scroll view
    //UIListView in UILayer
    _uiLayer = UILayer::create();
    _localRankingList = UIListView::create();
    //add all the ranking items as UILabel to UIListView
    for (int i = 0, size = _rankingAccessor->getLocalRankingCount() ; i < size ; ++ i) {
        //create UILabel for one item
        UILabel * rankingLabel = UILabel::create();
        CCString * text = CCString::createWithFormat("%d  -  %s  -  %ds",
            (i + 1), _rankingAccessor->getLocalUserAtIndex(i),
                                   _rankingAccessor->getLocalTimeAtIndex(i));
        rankingLabel->setText(text->getCString());
        rankingLabel->setFontSize(40);
        rankingLabel->setFontName("Marker Felt");
        //push the UILabel to UIListView
        _localRankingList->pushBackCustomItem(rankingLabel);

    }
    //set UIListView options
    setOptionsForUIListView(_localRankingList);
    _uiLayer->addWidget(_localRankingList);
    this->addChild(_uiLayer);
    
//    _uiLayer->setZOrder(kUpperZOrder);
//    this->addChild(_uiLayer, kUpperZOrder);
    //
    _globalRankingList = UIListView::create();
    
    //set UIListView options
    setOptionsForUIListView(_globalRankingList);
//    _globalUILayer->setVisible(false);
    
//    _globalUILayer->setZOrder(kLowerZOrder);
//    this->addChild(_globalUILayer, kLowerZOrder);
    
    
    //create Local Ranking and Global Ranking buttons
    CCMenuItemFont::setFontSize(40);
    _localRankingTab = CCMenuItemFont::create("Local Ranking", this, menu_selector(RankingLayer::rankingTabHandler));
    _localRankingTab->setPosition(-150, 0);
    _localRankingTab->setColor(ccc3(150, 0, 0));

    _globalRankingTab = CCMenuItemFont::create("Global Ranking", this, menu_selector(RankingLayer::rankingTabHandler));
    _globalRankingTab->setPosition(150, 0);
    _rankingTabs = CCMenu::create(_localRankingTab, _globalRankingTab, NULL);
    _rankingTabs->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5 + 300));
    this->addChild(_rankingTabs);
    
    //create Back to Menu button
    _backToMenuBtn = CCMenuItemFont::create("Back to Menu", this, menu_selector(RankingLayer::backToMenuHandler));
    _backToMenuBtn->setAnchorPoint(ccp(1, 0));
    _backToMenuBtn->setPosition(_screenSize.width - 40, 0);
    _bottomBtns = CCMenu::createWithItem(_backToMenuBtn);
    _bottomBtns->setPosition(0, _screenSize.height * 0.5 - 330);
    this->addChild(_bottomBtns);
    
    //
    _loadingIcon = CCSprite::create("loading.gif");
    _loadingIcon->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    _loadingIcon->setVisible(false);
    this->addChild(_loadingIcon, 3);
    
    return true;
}

CCScene * RankingLayer::scene() {
    CCScene * scene = CCScene::create();
    scene->addChild(RankingLayer::create());
    return scene;
}

//Local Ranking and Global Ranking buttons
//click callback function
void RankingLayer::rankingTabHandler(CCObject * pSender) {
    CCMenuItemFont * target = (CCMenuItemFont *) pSender;
    if (target == _localRankingTab) {
        _globalRankingTab->setColor(ccc3(255, 255, 255));
        if(_uiLayer->getWidgetByTag(0) == _localRankingList)
            return;
        _uiLayer->removeWidget(_globalRankingList);
        _uiLayer->addWidget(_localRankingList);
        _loadingIcon->setVisible(false);
        
        
    } else {
        _localRankingTab->setColor(ccc3(255, 255, 255));
        if(_uiLayer->getWidgetByTag(0) == _globalRankingList)
            return;
        _uiLayer->removeWidget(_localRankingList);
        _uiLayer->addWidget(_globalRankingList);
        
//        CCLog("global count: %d", _rankingAccessor->getGlobalRankingCount());
        if ((_rankingAccessor->getGlobalRankingCount() == 0) &&
            (! _rankingAccessor->isWaitingForResponse())) {
            _rankingAccessor->getRankingsFromServer(_loadingIcon, _globalRankingList);
        }
        
        if (_rankingAccessor->isWaitingForResponse()) {
            _loadingIcon->setVisible(true);
        }
    }
    target->setColor(ccc3(150, 0, 0));
}

//Back to Menu button callback
void RankingLayer::backToMenuHandler(CCObject *pSender) {
    CCScene * menuScene = CCTransitionFade::create(1.0f, MenuLayer::scene());
    _rankingAccessor->clearUnfinishedRequests();
    CCDirector::sharedDirector()->replaceScene(menuScene);
}

//
void RankingLayer::setOptionsForUIListView(UIListView *uiListView) const {
    uiListView->setGravity(LISTVIEW_GRAVITY_CENTER_HORIZONTAL);
    uiListView->setSize(CCSize(600, 500));
    uiListView->setAnchorPoint(ccp(0.5, 0.5));
    uiListView->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    uiListView->setItemsMargin(10);
    uiListView->setTag(0);
    
    uiListView->retain();
}