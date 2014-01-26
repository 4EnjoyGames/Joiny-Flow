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
    void hideEverything(cocos2d::CCCallFunc *callback);

};

#endif // DEVELOPERSCENE_H
