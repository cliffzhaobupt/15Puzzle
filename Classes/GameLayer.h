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

using namespace cocos2d;

enum zIndex {kPuzzleBox, kFinishLabel};
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
};

#endif /* defined(___5Puzzle__GameLayer__) */
