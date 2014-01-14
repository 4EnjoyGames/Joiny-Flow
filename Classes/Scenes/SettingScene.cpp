#include "SettingScene.h"
#include "MainScene.h"
#include "Localization/CCLocalizedString.h"

SettingScene::SettingScene()
{
}

CCScene* SettingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SettingScene *layer = SettingScene::create();
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(SettingScene::onKeyBackClicked));
    BackgroundHolder::backgroundSwitchTo(scene,back);

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
    this->setKeypadEnabled(true);

    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    //Back Button
    CCMenu* menu = _back.start(this, [this](){this->onButtonBackClicked(0);});
    this->addChild(menu);

    CCLabelTTF * settings = CCLabelTTF::create( CCLocalizedString("Settings").c_str(),"fonts/Fredoka One.ttf",72);
    settings->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + VISIBLE_SIZE.height - 80/SCALE));
    settings->setColor(ccc3(11,216,224));
    this->addChild(settings);

    CCSize size = settings->getContentSize();


    return true;
}
void SettingScene::doGoBack()
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}
void SettingScene::keyBackClicked()
{
    hideEverything(
                CCCallFunc::create(
                    this,
                    callfunc_selector(SettingScene::doGoBack)));
}
void SettingScene::hideEverything(cocos2d::CCCallFunc *callback)
{
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(0),
                    callback,
                    NULL));
}
