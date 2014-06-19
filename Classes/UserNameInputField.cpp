//
//  UserNameInputField.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-19.
//
//

#include "UserNameInputField.h"

UserNameInputField::UserNameInputField() {
//    CCLOG("initialized!!!");
    _screenSize = CCDirector::sharedDirector()->getWinSize();
}

bool UserNameInputField::onTextFieldAttachWithIME(CCTextFieldTTF *pSender) {
//    CCLOG("attached!");
    CCSprite * prompt = (CCSprite *) pSender->getUserObject();
    prompt->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f + 220));
    setAttachWithIME(true);
    return false;
}

bool UserNameInputField::onTextFieldDetachWithIME(CCTextFieldTTF *pSender) {
//    CCLOG("dettached!");
    CCSprite * prompt = (CCSprite *) pSender->getUserObject();
    prompt->setPosition(ccp(_screenSize.width * 0.5f, _screenSize.height * 0.5f));
    setDetachWithIME(true);
    return false;
}

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