//
//  UserNameInputField.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-19.
//
//

#include "UserNameInputField.h"

UserNameInputField::UserNameInputField() {
    _screenSize = CCDirector::sharedDirector()->getWinSize();
}

//override onTextFieldAttachWithIME member function
//to move the prompt to the upper part of the screen
//when user entering his name
bool UserNameInputField::onTextFieldAttachWithIME(CCTextFieldTTF *pSender) {
    CCSprite * prompt = (CCSprite *) pSender->getUserObject();
    prompt->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f + 220));
    setAttachWithIME(true);
    return false;
}

//override onTextFieldDetachWithIME member function
//to move the prompt to the center part of the screen
//after user has entered his name
bool UserNameInputField::onTextFieldDetachWithIME(CCTextFieldTTF *pSender) {
    CCSprite * prompt = (CCSprite *) pSender->getUserObject();
    prompt->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    setDetachWithIME(true);
    return false;
}

//static member function to create a UserNameInputField instance
UserNameInputField * UserNameInputField::createUserNameInputField(const char *placeholder, const char *fontName, float fontSize)  {
    UserNameInputField * pRet = new UserNameInputField();
    
    if (pRet && pRet->initWithString("", fontName, fontSize)) {
        pRet->autorelease();
        if (placeholder) {
            pRet->setPlaceHolder(placeholder);
        }
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    
    return NULL;
}