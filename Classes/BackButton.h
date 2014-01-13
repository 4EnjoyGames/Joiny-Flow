#ifndef BACKBUTTON_H
#define BACKBUTTON_H
#include "cocos2d-A.h"
#include "Core/Screen.h"
#include <functional>

class SelectCollection;
class BackButton
{
public:
    typedef std::function<void()> functionPtr;
    BackButton(CCLayer* layer, functionPtr function, CCMenu* menu);
private:
    void start(CCLayer *layer, functionPtr function, CCMenu *menu);
};

#endif // BACKBUTTON_H
