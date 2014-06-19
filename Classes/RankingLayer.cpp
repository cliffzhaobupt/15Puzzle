//
//  RankingLayer.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-18.
//
//

#include "RankingLayer.h"

RankingLayer::~RankingLayer() {
    delete _localRankingAccessor;
}

bool RankingLayer::init() {
    if (! CCLayer::init()) {
        return false;
    }
    
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
    _localRankingAccessor = RankingAccessor::createRankingAccessor("LocalRanking.json");
    _uiLayer = UILayer::create();
    _localRankingList = UIListView::create();
    
    for (int i = 0, size = _localRankingAccessor->getRankingCount() ; i < size ; ++ i) {
        UILabel * rankingLabel = Label::create();
        CCString * text = CCString::createWithFormat("%d  -  %s  -  %ds",
            (i + 1), _localRankingAccessor->getUserAtIndex(i),
                                   _localRankingAccessor->getTimeAtIndex(i));
        
        rankingLabel->setText(text->getCString());
        rankingLabel->setFontSize(40);
        rankingLabel->setFontName("Marker Felt");

        _localRankingList->pushBackCustomItem(rankingLabel);
        _localRankingList->setGravity(LISTVIEW_GRAVITY_CENTER_HORIZONTAL);
        _localRankingList->setSize(CCSize(600, 500));
        _localRankingList->setItemsMargin(10);
        _localRankingList->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
        _localRankingList->setAnchorPoint(ccp(0.5, 0.5));
        
    }
    
    _uiLayer->addWidget(_localRankingList);
    
    this->addChild(_uiLayer);
    
    CCMenuItemFont::setFontSize(40);
    _localRankingTab = CCMenuItemFont::create("Local Ranking", this, menu_selector(RankingLayer::rankingTabHandler));
    _localRankingTab->setPosition(-150, 0);
    _localRankingTab->setColor(ccc3(150, 0, 0));

    _globalRankingTab = CCMenuItemFont::create("Global Ranking", this, menu_selector(RankingLayer::rankingTabHandler));
    _globalRankingTab->setPosition(150, 0);
    _rankingTabs = CCMenu::create(_localRankingTab, _globalRankingTab, NULL);
    _rankingTabs->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5 + 300));
    this->addChild(_rankingTabs);
    
    _backToMenuBtn = CCMenuItemFont::create("Back to Menu", this, menu_selector(RankingLayer::backToMenuHandler));
    _backToMenuBtn->setAnchorPoint(ccp(1, 0));
    _backToMenuBtn->setPosition(_screenSize.width - 40, 0);
    _bottomBtns = CCMenu::createWithItem(_backToMenuBtn);
    _bottomBtns->setPosition(0, _screenSize.height * 0.5 - 330);
    this->addChild(_bottomBtns);
    
    return true;
}

CCScene * RankingLayer::scene() {
    CCScene * scene = CCScene::create();
    scene->addChild(RankingLayer::create());
    return scene;
}

void RankingLayer::rankingTabHandler(CCObject * pSender) {
    CCMenuItemFont * target = (CCMenuItemFont *) pSender;
    if (target == _localRankingTab) {
        _globalRankingTab->setColor(ccc3(255, 255, 255));
    } else {
        _localRankingTab->setColor(ccc3(255, 255, 255));
    }
    target->setColor(ccc3(150, 0, 0));
}

void RankingLayer::backToMenuHandler(CCObject *pSender) {
    CCScene * menuScene = CCTransitionFade::create(1.0f, MenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(menuScene);
}