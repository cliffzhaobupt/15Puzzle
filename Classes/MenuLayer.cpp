//
//  MenuLayer.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-17.
//
//

#include "MenuLayer.h"
#include "GameLayer.h"
#include "RankingLayer.h"

CCScene * MenuLayer::scene() {
    CCScene * scene = CCScene::create();
    scene->addChild(MenuLayer::create());
    return scene;
}


bool MenuLayer::init() {
    if (! CCLayer::init()) {
        return false;
    }
    
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
    CCMenuItemFont::setFontSize(60);
    _startButton = CCMenuItemFont::create("Start", this, menu_selector(MenuLayer::menuCallback));
    _startButton->setColor(ccc3(0, 255, 0));
    _startButton->setPosition(0, 50);
    
    _rankingButton = CCMenuItemFont::create("Ranking", this, menu_selector(MenuLayer::menuCallback));
    _rankingButton->setColor(ccc3(0, 0, 255));
    _rankingButton->setPosition(0, -50);
    
    _menu = CCMenu::create();
    _menu->addChild(_startButton, kButton, kStart);
    _menu->addChild(_rankingButton, kButton, kRanking);
    _menu->setPosition(_screenSize.width * 0.5, _screenSize.height * 0.5 - 200);
    this->addChild(_menu, kButton);
    
    _gameTitle = CCSprite::create("GameTitle.png");
    _gameTitle->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 + 200));
    this->addChild(_gameTitle, kGameTitle);
    
    return true;
}

void MenuLayer::menuCallback(CCObject *pSender) {
    CCMenuItemFont * target = (CCMenuItemFont *) pSender;
    if (target->getTag() == kStart) {
        CCScene * gameScene = CCTransitionFade::create(1.0f, GameLayer::scene());
        CCDirector::sharedDirector()->replaceScene(gameScene);
    } else if (target->getTag() == kRanking) {
        CCScene * rankingScene = CCTransitionFade::create(1.0f, RankingLayer::scene());
        CCDirector::sharedDirector()->replaceScene(rankingScene);
    }
}