//
//  MenuLayer.h
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-17.
//
//

#ifndef ___5Puzzle__MenuLayer__
#define ___5Puzzle__MenuLayer__

#include "cocos2d.h"

USING_NS_CC;

enum Z_INDEX {kGameTitle, kButton};
enum BUTTON_NAME {kStart, kRanking};

class MenuLayer : public CCLayer {
    CCSize _screenSize;
    
    CCMenu * _menu;
    CCMenuItemFont * _startButton;
    CCMenuItemFont * _rankingButton;
    CCSprite * _gameTitle;
    
public:
    static CCScene * scene();
    virtual bool init();
    CREATE_FUNC(MenuLayer);
    void menuCallback(CCObject *pSender);
};

#endif /* defined(___5Puzzle__MenuLayer__) */
