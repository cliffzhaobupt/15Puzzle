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
    //get current screen size
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    //initialize json reader / writer
    _localRankingAccessor = RankingAccessor::createRankingAccessor("LocalRanking.json");
    //initialize scroll view
    //UIListView in UILayer
    _uiLayer = UILayer::create();
    _localRankingList = UIListView::create();
    //add all the ranking items as UILabel to UIListView
    for (int i = 0, size = _localRankingAccessor->getRankingCount() ; i < size ; ++ i) {
        //create UILabel for one item
        UILabel * rankingLabel = UILabel::create();
        CCString * text = CCString::createWithFormat("%d  -  %s  -  %ds",
            (i + 1), _localRankingAccessor->getUserAtIndex(i),
                                   _localRankingAccessor->getTimeAtIndex(i));
        rankingLabel->setText(text->getCString());
        rankingLabel->setFontSize(40);
        rankingLabel->setFontName("Marker Felt");
        //push the UILabel to UIListView
        _localRankingList->pushBackCustomItem(rankingLabel);

    }
    //set UIListView options
    _localRankingList->setGravity(LISTVIEW_GRAVITY_CENTER_HORIZONTAL);
    _localRankingList->setSize(CCSize(600, 500));
    _localRankingList->setItemsMargin(10);
    _localRankingList->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    _localRankingList->setAnchorPoint(ccp(0.5, 0.5));
    //add UIListView to UILayer
    _uiLayer->addWidget(_localRankingList);
    
    this->addChild(_uiLayer);
    
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
    } else {
        _localRankingTab->setColor(ccc3(255, 255, 255));
    }
    target->setColor(ccc3(150, 0, 0));
}

//Back to Menu button callback
void RankingLayer::backToMenuHandler(CCObject *pSender) {
    CCScene * menuScene = CCTransitionFade::create(1.0f, MenuLayer::scene());
    CCDirector::sharedDirector()->replaceScene(menuScene);
}