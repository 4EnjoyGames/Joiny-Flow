#ifndef MAINSCENE_H
#define MAINSCENE_H
#include "cocos2d-A.h"
#include "Layers/PopUpWindow.h"

class MainScene: public CCLayerColor//CCLayer
{
public:
    MainScene();
    static CCScene* scene();
    static MainScene* create();

    void onPlayClicked(CCObject*);
private:
    bool init();

    void onSettingsClicked(CCObject*);
};

#endif // MAINSCENE_H
