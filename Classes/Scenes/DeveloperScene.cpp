#include "DeveloperScene.h"
#include "SettingScene.h"
#include "Localization/CCLocalizedString.h"
#include "GameInfo.h"
#include "Core/MusicSettings.h"
#include "Logic/RW.h"

DeveloperScene::DeveloperScene()
{
}

CCScene* DeveloperScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    DeveloperScene *layer = DeveloperScene::create();
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(DeveloperScene::onKeyBackClicked));
    BackgroundHolder::backgroundSwitchTo(scene,back);

    scene->addChild(layer);
    // return the scene
    return scene;
}
DeveloperScene* DeveloperScene::create()
{
    DeveloperScene *pRet = new DeveloperScene();
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

bool DeveloperScene::init()
{

    if (!DrawLayer::init())
    {
        return false;
    }
    this->setKeypadEnabled(true);

    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    CCMenu* main_menu = CCMenu::create();
    main_menu->setPosition(ORIGIN);

    //Back Button
    CCMenu* menu = _back.start(this, [this](){this->onButtonBackClicked(0);});
    this->addChild(menu);

    return true;
}

void DeveloperScene::doGoBack()
{
    CCDirector::sharedDirector()->replaceScene(SettingScene::scene());
}
void DeveloperScene::keyBackClicked()
{
    hideEverything(
                CCCallFunc::create(
                    this,
                    callfunc_selector(DeveloperScene::doGoBack)));
}
void DeveloperScene::hideEverything(cocos2d::CCCallFunc *callback)
{
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(0),
                    callback,
                    NULL));
}
