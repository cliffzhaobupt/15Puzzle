//
//  PuzzleBoxSprite.h
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-11.
//
//

#ifndef ___5Puzzle__PuzzleBoxSprite__
#define ___5Puzzle__PuzzleBoxSprite__

#include "cocos2d.h"

USING_NS_CC;

class PuzzleBoxSprite : public CCSprite {
public:
    //actual id of the puzzle box
    CC_SYNTHESIZE(int, _id, Id);
    //the original position of the puzzle box
    CC_SYNTHESIZE(CCPoint, _positionShouldBe, PositionShouldBe);
    //the position id of the puzzle box
    CC_SYNTHESIZE(int, _positionId, PositionId);
    
    //generate a puzzle box sprite object by actual id and screen size
    static PuzzleBoxSprite * initPuzzleBoxSpriteWithId(int id, const CCSize & screenSize);
    //whether the box is a blank box
    bool isBlankBox();
    //get the vector of adjacent puzzle boxes' position id
    std::vector<int> * getAdjacentPosId ();
};

#endif /* defined(___5Puzzle__PuzzleBoxSprite__) */
