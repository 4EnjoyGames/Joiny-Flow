#ifndef SETTINGSCENE_H
#define SETTINGSCENE_H
#include "cocos2d-A.h"
#include "Core/Screen.h"
#include "BackButton.h"
#include "Core/DrawLayer.h"
#include "Layers/YesNoDialog.h"

class SettingScene: public DrawLayer //CCLayer
{
public:
    SettingScene();
    static CCScene* scene();
    static SettingScene* create();

private:
    PopUpWindowManager _pop_up_manager;

    class ResetPopUp;
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
    void doDevelopers();
    void hideEverything(cocos2d::CCCallFunc *callback);

    void onMusicClicked(CCObject*);
    void onSoundClicked(CCObject*);
    void onResetClicked(CCObject*);
    void onRestorClicked(CCObject*);
    void onDevelopersClicked(CCObject*);
    void onRateMeClicked(CCObject*);

    void showAnimation();

    AnimatedMenuItem *createButton(CCSprite *sprite,
                      SEL_MenuHandler ptr,
                      CCPoint &position,
                      const char* button_text, const ccColor3B &color);

    const ccColor3B getSoundColor() const;
    const ccColor3B getMusicColor() const;

    CCMenu* _main_menu;

    //buttons vector
    std::vector<AnimatedMenuItem*> _buttons;
    AnimatedMenuItem* _sound_button;
    AnimatedMenuItem* _music_button;
};

#endif // SETTINGSCENE_H
