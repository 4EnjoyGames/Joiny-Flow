#ifndef DEVELOPERSCENE_H
#define DEVELOPERSCENE_H
#include "cocos2d-A.h"
#include "Core/Screen.h"
#include "BackButton.h"
#include "Core/DrawLayer.h"

class DeveloperScene: public DrawLayer //CCLayer
{
public:
    DeveloperScene();
    static CCScene* scene();
    static DeveloperScene* create();

private:
    CCLabelTTF * _author_diana;
    CCLabelTTF * _author_andrii;

    CCLabelTTF * _thank;
    CCLabelTTF * _thank1;

    //x4enjoy logo
    AnimatedMenuItem* _x4enjoy_button;

    //ukraine flag
    CCSprite* _flag;

    //copy right
    CCLabelTTF * _reight;

    BackButton _back;
    bool init();

    void keyBackClicked();

    void onKeyBackClicked()
    {
        keyBackClicked();
    }
    void onButtonBackClicked(CCObject*)
    {
        keyBackClicked();
    }
    void doGoBack();
    void onSiteClick(CCObject*);

    void hideEverything(cocos2d::CCCallFunc *callback);

    void showAnimation();

};

#endif // DEVELOPERSCENE_H
