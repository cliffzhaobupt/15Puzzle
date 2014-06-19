//
//  GameLayer.h
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-11.
//
//

#ifndef ___5Puzzle__GameLayer__
#define ___5Puzzle__GameLayer__

#include "cocos2d.h"
#include "RankingAccessor.h"
#include "RankingLayer.h"
#include "cocos-ext.h"
#include "UserNameInputField.h"

using namespace cocos2d;

enum ZIndex {kPuzzleBox, kTimeLabel, kPrompt, kFinishLabel, kMenuInPrompt, kNameInput, kNameBtn};
enum Buttons {kEditName, kNameOK, kPlayAgain, kSeeRanking};
const float timesArr[] = {-1.5, -0.5, 0.5, 1.5};

class GameLayer : public CCLayer {
    //screen size
    CCSize _screenSize;
    //puzzle boxes array
    CCArray * _boxes;
    //use to diable touch when the animation is running
    bool _canClickBox;
    //game finish label
    CCLabelTTF * _finishLabel;
    //time label
    CCLabelTTF * _timeLabel;
    
    CCSprite * _prompt;
    
    CCMenuItemFont * _playAgain;
    
    CCMenuItemFont * _ranking;
    
    CCMenu * _gameMenu;
    
    CCMenuItemFont * _nameEneter;
    CCMenuItemFont * _nameOK;
    
    CCMenu * _nameMenu;
    
//    CCTextFieldTTF * _nameInput;
    UserNameInputField * _nameInput;
    
    CCMenu * _upperBtns;
    CCMenuItemFont * _restartBtn;
    
    bool _gameFinished;
    
    float _timeAdder;
    int _timeToDisplay;
    
    RankingAccessor * _localRankingAccessor;

    
public:
    virtual bool init();
    static CCScene * scene();
    CREATE_FUNC(GameLayer);
    void update(float dt);
    ~GameLayer();
    virtual void ccTouchesBegan(CCSet * pTouches, CCEvent * event);
    void setCanClickBox();
    void resetGame();
    std::vector<int> * generateRandomNumbers();
    void menuInPromptCallback(CCObject * pSender);
    void restartBtnCallback(CCObject * pSender);
};

#endif /* defined(___5Puzzle__GameLayer__) */
