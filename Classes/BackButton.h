#ifndef BACKBUTTON_H
#define BACKBUTTON_H
#include "cocos2d-A.h"
#include "Core/Screen.h"

class BackButton
{
public:
    typedef void functionPtr(CCObject*);
    BackButton(CCLayer* layer, functionPtr function, CCMenu* menu);
private:
    void start(CCLayer *layer, functionPtr function, CCMenu *menu);
};

#endif // BACKBUTTON_H
