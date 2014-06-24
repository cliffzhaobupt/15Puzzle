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
#include "PuzzleBoxSprite.h"
#include "MenuLayer.h"

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
    //prompt when game finishes
    CCSprite * _prompt;
    //play again button in the prompt
    CCMenuItemFont * _playAgain;
    //ranking button in the prompt
    CCMenuItemFont * _ranking;
    //CCMenu object containing "play again" and "ranking" buttons
    CCMenu * _gameMenu;
    //edit name button in the prompt
    CCMenuItemFont * _nameEneter;
    //ok button in the prompt
    CCMenuItemFont * _nameOK;
    //CCMenu object containing "edit name" and "ok" buttons
    CCMenu * _nameMenu;
    //user name input in the prompt
    UserNameInputField * _nameInput;
    //restart button at the upper part of the game
    CCMenu * _upperBtns;
    CCMenuItemFont * _restartBtn;
    CCMenuItemFont * _mainMenuBtn;
    //whether game has finished
    bool _gameFinished;
    //used to display seconds
    float _timeAdder;
    int _timeToDisplay;
    //Json writer/reader
    RankingAccessor * _localRankingAccessor;
    //sprite batch node for boxes
    CCSpriteBatchNode * _boxesBatchNode;

    
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
    void upperBtnsCallback(CCObject * pSender);
};

#endif /* defined(___5Puzzle__GameLayer__) */
