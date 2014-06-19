//
//  UserNameInputField.h
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-19.
//
//

#ifndef ___5Puzzle__UserNameInputField__
#define ___5Puzzle__UserNameInputField__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace gui;

class UserNameInputField : public UICCTextField {
    CCSize _screenSize;
    
public:
    virtual bool onTextFieldAttachWithIME (CCTextFieldTTF *pSender);
    virtual bool onTextFieldDetachWithIME (CCTextFieldTTF *pSender);
    
    UserNameInputField();
    
    static UserNameInputField * createUserNameInputField(const char * placeholder, const char * fontName, float fontSize);

};

#endif /* defined(___5Puzzle__UserNameInputField__) */
