//
//  PuzzleBoxSprite.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-11.
//
//

#include "PuzzleBoxSprite.h"

//generate a puzzle box sprite object by actual id and screen size
PuzzleBoxSprite * PuzzleBoxSprite::initPuzzleBoxSpriteWithId(int id, const CCSize & screenSize) {
    //generate filename by actual id
    char filename [10];
    sprintf(filename, "box%d.png", id);
    //use new keyword to create one instance
    //and use initWithFile() funcation to initialize CCSprite
    PuzzleBoxSprite * newPBS = new PuzzleBoxSprite;
    newPBS->initWithFile(filename);
    newPBS->setId(id);
    //calculate the original position of puzzle box
    const float timesArr[] = {-1.5, -0.5, 0.5, 1.5};
    newPBS->setPositionShouldBe(ccp(
        screenSize.width * 0.5 + newPBS->getContentSize().width
                                      * timesArr[(id - 1) % 4],
        screenSize.height * 0.5 + newPBS->getContentSize().width * timesArr[3 - (id - 1) / 4]));
    //autorelease the new instance
    newPBS->autorelease();
    
    return newPBS;
}

//whether the box is a blank box
bool PuzzleBoxSprite::isBlankBox() {
    return (_id == 16);
}

//get the vector of adjacent puzzle boxes' position id
std::vector<int> * PuzzleBoxSprite::getAdjacentPosId() {
    std::vector<int> * adjacentPosIds = new std::vector<int>;
    if ((_positionId - 1) / 4 != 0)
        adjacentPosIds->push_back(_positionId - 4);
    if (_positionId % 4 != 0)
        adjacentPosIds->push_back(_positionId + 1);
    if ((_positionId - 1) / 4 != 3)
        adjacentPosIds->push_back(_positionId + 4);
    if (_positionId % 4 != 1)
        adjacentPosIds->push_back(_positionId - 1);
    return adjacentPosIds;
}