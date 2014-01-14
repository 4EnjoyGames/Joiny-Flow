#ifndef BACKBUTTON_H
#define BACKBUTTON_H
#include "cocos2d-A.h"
#include "Core/Screen.h"
#include <functional>


class BackButton : public CCObject
{
public:
    typedef std::function<void()> functionPtr;
    BackButton();
    CCMenu *start(CCLayer* layer, functionPtr function);
    CCSize getBackSize();
private:
    void doAction(CCObject* obj);
    functionPtr _function;
    CCLayer* _layer;
    CCSize _size;
};

#endif // BACKBUTTON_H
