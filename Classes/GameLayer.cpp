//
//  GameLayer.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-11.
//
//

#include "GameLayer.h"
#include "PuzzleBoxSprite.h"
#include <algorithm>

#define EASY_GAME

USING_NS_CC;

//generate random number for placing the puzzle boxes
std::vector<int> * GameLayer::generateRandomNumbers() {
    //generate random numbers
    std::vector<int> * randomNumbers = new std::vector<int>;
    
#ifdef EASY_GAME
    for (int i = 1 ; i < 15 ; ++ i) {
        randomNumbers->push_back(i);
    }
    randomNumbers->push_back(16);
    randomNumbers->push_back(15);
#else
    for (int i = 1 ; i < 17; ++ i) {
        randomNumbers->push_back(i);
    }
    srand(static_cast<unsigned int>(time(0)));
    std::random_shuffle(randomNumbers->begin(), randomNumbers->end());
#endif
    
    return randomNumbers;
}

//initialize the GameLayer
bool GameLayer::init() {
    //run the init() method of the base class - CCLayer
    if (! CCLayer::init()) {
        return false;
    }
    
    //get the screen size from CCDirector
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    //create CCArray by capacity
    _boxes = CCArray::createWithCapacity(15);
    
    
    
    //create prompt
    _prompt = CCSprite::create("PromptBackground.png");
    _prompt->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    _prompt->setVisible(false);
    
    //create game finish CCLabelTTF
    _finishLabel = CCLabelTTF::create("You Win", "Arial", 40);
    _finishLabel->setColor(ccc3(0, 0, 0));
    _finishLabel->setPosition(ccp(300, 300));
    _prompt->addChild(_finishLabel, kFinishLabel);
    
    //add "New Game" and "Ranking" buttons to the prompt
    //they'll be displayed after user enter his name
    CCMenuItemFont::setFontSize(40);
    _playAgain = CCMenuItemFont::create("New Game", this, menu_selector(GameLayer::menuInPromptCallback));
    _playAgain->setColor(ccc3(255, 0, 0));
    _playAgain->setPosition(0, 30);
    _ranking = CCMenuItemFont::create("Ranking", this, menu_selector(GameLayer::menuInPromptCallback));
    _ranking->setColor(ccc3(0, 0, 255));
    _ranking->setPosition(0, -30);
    _gameMenu = CCMenu::create();
    _gameMenu->addChild(_playAgain, kMenuInPrompt, kPlayAgain);
    _gameMenu->addChild(_ranking, kMenuInPrompt, kSeeRanking);
    _gameMenu->setPosition(ccp(300, 180));
    
    _gameMenu->setVisible(false);
    _prompt->addChild(_gameMenu, kMenuInPrompt);
    
    //create name input text field in the prompt
    _nameInput = UserNameInputField::createUserNameInputField("Your name...", "Arial", 40);
    _nameInput->setColor(ccc3(0, 0, 0));
    _nameInput->setColorSpaceHolder(ccc3(150, 150, 150));
    _nameInput->setPosition(ccp(300, 225));
    _nameInput->setMaxLength(10);
    _nameInput->setMaxLengthEnabled(true);
    //set prompt as "UserObject" to fix the position of it
    //when the keyboard is shown / hidden
    _nameInput->setUserObject(_prompt);
    _prompt->addChild(_nameInput, kNameInput);
    
    
    //add "Edit Name" and "OK" button in the prompt
    _nameEneter = CCMenuItemFont::create("Edit Name", this, menu_selector(GameLayer::menuInPromptCallback));
    _nameEneter->setColor(ccc3(0, 255, 0));
    _nameEneter->setPosition(-100, 0);
    _nameOK = CCMenuItemFont::create("OK", this, menu_selector(GameLayer::menuInPromptCallback));
    _nameOK->setColor(ccc3(0, 0, 255));
    _nameOK->setPosition(100, 0);
    _nameMenu = CCMenu::create();
    _nameMenu->addChild(_nameEneter, kNameBtn, kEditName);
    _nameMenu->addChild(_nameOK, kNameBtn, kNameOK);
    _nameMenu->setPosition(ccp(300, 150));
    _prompt->addChild(_nameMenu, kNameBtn);
    
    this->addChild(_prompt, kPrompt);
    
    //create restart button
    _restartBtn = CCMenuItemFont::create("Restart Game", this, menu_selector(GameLayer::restartBtnCallback));
    _restartBtn->setFontSizeObj(40);
    _restartBtn->setAnchorPoint(ccp(0, 0));
    _restartBtn->setPosition(40, 20);
    _upperBtns = CCMenu::createWithItem(_restartBtn);
    _upperBtns->setAnchorPoint(ccp(0, 0));
    _upperBtns->setPosition(ccp(0, _screenSize.height * 0.5 + 300));
    this->addChild(_upperBtns);
    
    //create time label
    _timeAdder = 0.0;
    _timeToDisplay = 0;
    _timeLabel = CCLabelTTF::create("0", "Arial", 80);
    _timeLabel->setPosition(ccp(_screenSize.width - 30, _screenSize.height * 0.5 - 300));
    _timeLabel->setAnchorPoint(ccp(1, 1));
    this->addChild(_timeLabel, kTimeLabel);
    
    //generate random numbers
    std::vector<int> * randomNumbers = generateRandomNumbers();
    
    //add 16 PuzzleBoxSprites to current GameLayer
    for (int i = 0 ; i < 16 ; ++i) {
        PuzzleBoxSprite * box = PuzzleBoxSprite::initPuzzleBoxSpriteWithId((*randomNumbers)[i], _screenSize);
        box->setPositionId(i + 1);
        box->setPosition(ccp(
            _screenSize.width * 0.5 +  box->getContentSize().width * timesArr[i % 4],
            _screenSize.height * 0.5 + box->getContentSize().width * timesArr[3 - i / 4]));
        
        this->addChild(box, kPuzzleBox, (*randomNumbers)[i]);
        //save puzzle box sprites to CCArray
        _boxes->addObject(box);
    }
    
    delete randomNumbers;
    
    //initialize the JSON reader-writer
    _localRankingAccessor = RankingAccessor::createRankingAccessor("LocalRanking.json");
    _localRankingAccessor->retain();
    
    //retain the CCArray for future use
    _boxes->retain();
    
    //enable user to touch the boxes(illustrating no animation running)
    _canClickBox = true;
    
    _gameFinished = false;
    
    //enable touch
    this->setTouchEnabled(true);
    
    //create main loop
    this->schedule(schedule_selector(GameLayer::update));
    
    return true;
}

//create CCScene by CCLayer of GameLayer
CCScene * GameLayer::scene() {
    CCScene *scene = CCScene::create();
    GameLayer *layer = GameLayer::create();
    scene->addChild(layer);
    return scene;
}

//touch handler
void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *event) {
    
    //if there's CCAction running, return directly
    if (! _canClickBox) return;
    
    //if the game has finished, return directly
    if (_gameFinished) return;
    
    //get one CCTouch object from CCSet - anyObject() method
    CCTouch * touch = (CCTouch *) pTouches->anyObject();
    
    //if there's a valid touch object
    if (touch) {
        for (int i = 0 ; i < _boxes->count() ; ++ i) {
            PuzzleBoxSprite * currentBox = (PuzzleBoxSprite *)  _boxes->objectAtIndex(i);
            //check whether the touch location is in one of the puzzle box
            //boundingBox().containsPoint(CCPoint) - check method
            //CCTouch->getLocation() - get touch location
            if (currentBox->boundingBox().containsPoint(touch->getLocation())) {
                //if touch the blank box, return directly
                if (currentBox->isBlankBox()) return;
                //get vector of adjacent position id of the box being touched
                std::vector<int> * adjacentPosIds = currentBox->getAdjacentPosId();
                
                for (std::vector<int>::const_iterator iter = adjacentPosIds->begin(); iter != adjacentPosIds->end(); ++iter) {
                    for (int j = 0 ; j < _boxes->count() ; j ++) {
                        PuzzleBoxSprite * adjacentCheck = (PuzzleBoxSprite *) _boxes->objectAtIndex(j);
                        //if the box is adjacent to touching box and the box is blank box, move the touching box to the position of the blank box
                        if (adjacentCheck->getPositionId() == *iter && adjacentCheck->isBlankBox()) {
                            //due to the running of animation, user may not touch the box any more
                            _canClickBox = false;
                            //directly set the position of the blank box to the position of the box being touched
                            CCPoint tempPoint = adjacentCheck->getPosition();
                            adjacentCheck->setPosition(currentBox->getPosition());
                            //create CCAction of the touching box - moving to the position of the blank box and after running animation, call function to enable touching boxes
                            CCActionInterval * currentBoxAction = CCSequence::create(
                                CCEaseInOut::create(CCMoveTo::create(0.3f, tempPoint), 0.5f),
                                CCCallFunc::create(this, callfunc_selector(GameLayer::setCanClickBox)), NULL);
                            currentBox->runAction(currentBoxAction);
                            //update position id of the 2 boxes for future use
                            int tempPosId = adjacentCheck->getPositionId();
                            adjacentCheck->setPositionId(currentBox->getPositionId());
                            currentBox->setPositionId(tempPosId);
                            //check whether the player has finished the game
                            bool finish = true;
                            for (int q = 0 ; q < _boxes->count() ; ++q) {
                                PuzzleBoxSprite * finishCheck = (PuzzleBoxSprite *) _boxes->objectAtIndex(q);
                                finish = finish && (finishCheck->getId() == finishCheck->getPositionId());
                                if (! finish) {
                                    break;
                                }
                            }
                            if (finish) {
                                _finishLabel->setString(CCString::createWithFormat("Clear in %d seconds", _timeToDisplay)->getCString());

                                if (_nameInput->getCharCount() == 0) {
                                    std::string nameCacheStr = CCUserDefault::sharedUserDefault()->getStringForKey("user_name_cache");
                                    CCLog("user_name_cache: %s", nameCacheStr.c_str());
                                    if (nameCacheStr.size() > 0) {
                                        _nameInput->setString(nameCacheStr.c_str());
                                    } else {
                                        _nameInput->openIME();
                                    }
                                }
                                
                                
                                
                                _gameFinished = true;
                                _prompt->setVisible(true);
                            }
                            
                            delete adjacentPosIds;
                            return;
                        }
                    }
                }

                delete adjacentPosIds;
            }
        }
    }
}

//animation callback function
void GameLayer::setCanClickBox() {
    _canClickBox = true;
}

void GameLayer::update(float dt) {
    if (! _gameFinished) {
        _timeAdder += dt;
        if (_timeAdder >= 1) {
            _timeAdder = 0;
            ++ _timeToDisplay;
            CCString * timeStr = CCString::createWithFormat("%d", _timeToDisplay);
            _timeLabel->setString(timeStr->getCString());
        }
    }

}

//reset game to play again
void GameLayer::resetGame() {

    _gameFinished = false;
    _timeLabel->setString("0");
    _timeToDisplay = 0;
    _timeAdder = 0;
    
    _nameMenu->setVisible(true);
    _gameMenu->setVisible(false);
    _nameInput->setVisible(true);
    
    //generate random numbers
    std::vector<int> * randomNumbers = generateRandomNumbers();

    for (int i = 0 ; i < randomNumbers->size() ; ++ i) {
        PuzzleBoxSprite * currentBox = (PuzzleBoxSprite *) this->getChildByTag((*randomNumbers)[i]);
        currentBox->setPositionId(i + 1);
        currentBox->setPosition(ccp(_screenSize.width * 0.5 +  currentBox->getContentSize().width * timesArr[i % 4],
            _screenSize.height * 0.5 + currentBox->getContentSize().width * timesArr[3 - i / 4]));
    }
    
    delete randomNumbers;
}

//deal with the click of buttons in the prompt
void GameLayer::menuInPromptCallback(CCObject *pSender) {
    CCNode * target = (CCNode *) pSender;
    switch (target->getTag()) {
        case kPlayAgain:
            _prompt->setVisible(false);
            resetGame();
            break;
            
        case kEditName:
            _nameInput->openIME();
            break;
        
        case kNameOK:
            if (_nameInput->getCharCount() == 0)
                return;
            _nameMenu->setVisible(false);
            _gameMenu->setVisible(true);
            _nameInput->setVisible(false);
            CCUserDefault::sharedUserDefault()->setStringForKey("user_name_cache", _nameInput->getString());
            //insert current user name and time into the json file
            _localRankingAccessor->insertRankingItem(_nameInput->getString(), _timeToDisplay);
            break;
        
        case kSeeRanking:
            CCScene * rankingScene = CCTransitionFade::create(1.0f, RankingLayer::scene());
            CCDirector::sharedDirector()->replaceScene(rankingScene);
            
    }
}

//restart button callback
void GameLayer::restartBtnCallback(CCObject *pSender) {
    if (_gameFinished) return;
    resetGame();
}

GameLayer::~GameLayer() {
    CC_SAFE_RELEASE(_boxes);
    
    CC_SAFE_RELEASE(_localRankingAccessor);
}

