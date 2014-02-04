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


    //authors text
    CCLabelTTF * author_diana = CCLabelTTF::create(_("diana"),
                                               "fonts/Fredoka One.ttf",
                                               72);
    author_diana->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                              ORIGIN.y + VISIBLE_SIZE.height*0.8));
    author_diana->setColor(GameInfo::getInstance()->getNegativeColor());
    this->addChild(author_diana);

    CCLabelTTF * author_andrii = CCLabelTTF::create(_("andrii"),
                                               "fonts/Fredoka One.ttf",
                                               72);
    author_andrii->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                              ORIGIN.y + VISIBLE_SIZE.height*0.7));
    author_andrii->setColor(GameInfo::getInstance()->getNegativeColor());
    this->addChild(author_andrii);

    //thank to logo
    CCLabelTTF * thank = CCLabelTTF::create(_("thank"),
                                               "fonts/Fredoka One.ttf",
                                               50);
    thank->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                              ORIGIN.y + VISIBLE_SIZE.height*0.5));
    thank->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(thank);

    //thank list - thank1
    CCLabelTTF * thank1 = CCLabelTTF::create(_("thank1"),
                                               "fonts/Fredoka One.ttf",
                                               50);
    thank1->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.25,
                              ORIGIN.y + VISIBLE_SIZE.height*0.4));
    thank1->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(thank1);

    //////////////////////////////////////
    CCLabelTTF * thank2 = CCLabelTTF::create(_("thank2"),
                                               "fonts/Fredoka One.ttf",
                                               50);
    thank2->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.75,
                              ORIGIN.y + VISIBLE_SIZE.height*0.4));
    thank2->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(thank2);

    ////////////////////////////////////////
    CCLabelTTF * thank3 = CCLabelTTF::create(_("thank2"),
                                               "fonts/Fredoka One.ttf",
                                               50);
    thank3->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.25,
                              ORIGIN.y + VISIBLE_SIZE.height*0.3));
    thank3->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(thank3);

    ////////////////////////////////////////
    CCLabelTTF * thank4 = CCLabelTTF::create(_("thank2"),
                                               "fonts/Fredoka One.ttf",
                                               50);
    thank4->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.75,
                              ORIGIN.y + VISIBLE_SIZE.height*0.3));
    thank4->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(thank4);


    //x4enjoy logo
    CCSprite* x4enjoy_logo = CCSprite::create("universal/x4Enjoy.png");
    CCSize x4enjoy_size = x4enjoy_logo->getContentSize();
    x4enjoy_logo->setScale(x4enjoy_size.width/x4enjoy_size.width*0.25);

    x4enjoy_logo->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                                  ORIGIN.y + 200/SCALE));
    this->addChild(x4enjoy_logo);

    //ukraine flag

    //made in ukraine
    CCLabelTTF * made = CCLabelTTF::create(_("madeInUkraine"),
                                               "fonts/Fredoka One.ttf",
                                               30);
    made->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + 100/SCALE));
    made->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(made);


    //copy right

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
