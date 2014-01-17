#ifndef SETTINGSCENE_H
#define SETTINGSCENE_H
#include "cocos2d-A.h"
#include "Core/Screen.h"
#include "BackButton.h"

class SettingScene: public CCLayerColor //CCLayer
{
public:
    SettingScene();
    static CCScene* scene();
    static SettingScene* create();

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

    void onMusicClicked(CCObject*);
    void onSoundClicked(CCObject*);
    void onResetClicked(CCObject*);
    void onRestorClicked(CCObject*);
    void onDevelopersClicked(CCObject*);

    const ccColor3B getSoundColor() const;
    const ccColor3B getMusicColor() const;

    CCSprite* _music_logo;
    CCSprite* _sound_logo;
};

#endif // SETTINGSCENE_H
