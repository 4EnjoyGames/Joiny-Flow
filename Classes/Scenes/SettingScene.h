#ifndef SETTINGSCENE_H
#define SETTINGSCENE_H
#include "cocos2d-A.h"
#include "Layers/PopUpWindow.h"

class SettingScene: public /*CCLayerColor*/ CCLayer
{
public:
    SettingScene();
    static CCScene* scene();
    static SettingScene* create();

private:
    bool init();
};

#endif // SETTINGSCENE_H
