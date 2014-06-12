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
    
    _finishLabel = CCLabelTTF::create("You Win", "Arial", 150);
    ccColor3B textColor;
    textColor.r = 0;
    textColor.g = 0;
    textColor.b = 0;
    _finishLabel->setColor(textColor);
    _finishLabel->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    _finishLabel->setVisible(false);
    this->addChild(_finishLabel, kFinishLabel);
    
    //generate random numbers
    std::vector<int> randomNumbers;

#ifdef EASY_GAME
    for (int i = 1 ; i < 15 ; ++ i) {
        randomNumbers.push_back(i);
    }
    randomNumbers.push_back(16);
    randomNumbers.push_back(15);
#else
    for (int i = 1 ; i < 17; ++ i) {
        randomNumbers.push_back(i);
    }
    srand(static_cast<unsigned int>(time(0)));
    std::random_shuffle(randomNumbers.begin(), randomNumbers.end());
#endif

    
    //add 16 PuzzleBoxSprites to current GameLayer
    const float timesArr[] = {-1.5, -0.5, 0.5, 1.5};
    for (int i = 0 ; i < 16 ; ++i) {
        PuzzleBoxSprite * box = PuzzleBoxSprite::initPuzzleBoxSpriteWithId(randomNumbers[i], _screenSize);
        box->setPositionId(i + 1);
        box->setPosition(ccp(
            _screenSize.width * 0.5 +  box->getContentSize().width * timesArr[i % 4],
            _screenSize.height * 0.5 + box->getContentSize().width * timesArr[3 - i / 4]));
        
        this->addChild(box, kPuzzleBox);
        //save puzzle box sprites to CCArray
        _boxes->addObject(box);
    }
    //retain the CCArray for future use
    _boxes->retain();
    
    //enable user to touch the boxes(illustrating no animation running)
    _canClickBox = true;
    
    //enable touch
    this->setTouchEnabled(true);
    
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
                            CCAction * currentBoxAction = CCSequence::create(
                                CCMoveTo::create(0.3f, tempPoint),
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
                                _finishLabel->setVisible(true);
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
    
}

GameLayer::~GameLayer() {
    CC_SAFE_RELEASE(_boxes);
}