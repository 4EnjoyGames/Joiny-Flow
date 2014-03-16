#ifndef MAINSCENE_H
#define MAINSCENE_H
#include "cocos2d-A.h"
#include "Layers/PopUpWindow.h"
#include "Core/DrawLayer.h"

class MainScene: public DrawLayer //CCLayerColor //CCLayer
{
public:
    MainScene();
    static CCScene* scene();
    static MainScene* create();

private:
    CCSprite* _main_logo;
    AnimatedMenuItem* _play_button;
    AnimatedMenuItem* _settings_button;
    AnimatedMenuItem* _facebook_button;

    PopUpWindowManager _pop_up_manager;
    class GoOutPopUp;
    bool init();

    void onPlayClicked(CCObject*);
    void doOnPlayClicked();

    void onFacebookClicked(CCObject*);

    void onSettingsClicked(CCObject*);
    void doOnSettingsClicked();

    void hideEverything(CCCallFunc *callback);
    void doGoBack();
    void keyBackClicked();

    void onKeyBackClicked()
    {
        keyBackClicked();
    }
};

#endif // MAINSCENE_H
