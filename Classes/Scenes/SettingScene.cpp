#include "SettingScene.h"

SettingScene::SettingScene()
{
}

CCScene* SettingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SettingScene *layer = SettingScene::create();

    scene->addChild(layer);
    // return the scene
    return scene;
}
SettingScene* SettingScene::create()
{
    SettingScene *pRet = new SettingScene();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool SettingScene::init()
{
//    if (!CCLayer::init() )
//    {
//        return false;
//    }
    if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
    {
        return false;
    }
    return true;
}
