#include "DeveloperScene.h"
#include "SettingScene.h"
#include "Localization/CCLocalizedString.h"
#include "GameInfo.h"
#include "Core/MusicSettings.h"
#include "Logic/RW.h"
#include "Core/Fonts.h"

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
    _author_diana = CCLabelTTF::create(_("diana"),
                                               Fonts::getFontName(),
                                               72);
    _author_diana->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                              ORIGIN.y + VISIBLE_SIZE.height*0.85));
    _author_diana->setColor(GameInfo::getInstance()->getNegativeColor());
    this->addChild(_author_diana);

    _author_andrii = CCLabelTTF::create(_("andrii"),
                                               Fonts::getFontName(),
                                               72);
    _author_andrii->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                              ORIGIN.y + VISIBLE_SIZE.height*0.75));
    _author_andrii->setColor(GameInfo::getInstance()->getNegativeColor());
    this->addChild(_author_andrii);

    //thank to logo
    _thank = CCLabelTTF::create(_("thank"),
                                               Fonts::getFontName(),
                                               30);
    _thank->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                              ORIGIN.y + VISIBLE_SIZE.height*0.6));
    _thank->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(_thank);

    //thank list - thank1
    _thank1 = CCLabelTTF::create(_("test_list"),
                                               Fonts::getFontName(),
                                               30);
    _thank1->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                              ORIGIN.y + VISIBLE_SIZE.height*0.45));
    _thank1->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(_thank1);


    //x4enjoy logo
//    _x4enjoy_logo = CCSprite::create("universal/x4Enjoy.png");
//    CCSize x4enjoy_size = _x4enjoy_logo->getContentSize();
//    _x4enjoy_logo->setScale(x4enjoy_size.width/x4enjoy_size.width*0.4);

//    _x4enjoy_logo->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
//                                  ORIGIN.y + 220/SCALE));
//    this->addChild(_x4enjoy_logo);


    CCMenu* logo_menu = CCMenu::create();
    logo_menu->setPosition(ORIGIN);

    //animated item
    CCSprite* x4enjoy_logo = CCSprite::create("settings-menu/settings_4Enjoy.png");
    //CCSize x4enjoy_size = x4enjoy_logo->getContentSize();

    _x4enjoy_button = AnimatedMenuItem::create(
                x4enjoy_logo,
                this,
                menu_selector(DeveloperScene::onSiteClick));

    _x4enjoy_button->addChild(x4enjoy_logo);
    _x4enjoy_button->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                                     ORIGIN.y + 220/SCALE));
    x4enjoy_logo->setPosition(ccp(x4enjoy_logo->getContentSize().width/2,
                               x4enjoy_logo->getContentSize().height/2));

    logo_menu->addChild(_x4enjoy_button);
    this->addChild(logo_menu);



    //ukraine flag
    _flag = CCSprite::create("settings-menu/flag.png");
    _flag->setScale(_flag->getContentSize().width/_flag->getContentSize().width*0.25);

    _flag->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                                  ORIGIN.y + 100/SCALE));
    this->addChild(_flag);

    //copy right
    _reight = CCLabelTTF::create(_("copy"),
                                               Fonts::getFontName(),
                                               30);
    _reight->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + 50/SCALE));
    _reight->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(_reight);
    //© 2014

    showAnimation();
    return true;
}
#include <ADLib/Device/ADBrowser.h>
void DeveloperScene::onSiteClick(CCObject*)
{
    ADBrowser::openWebURL("http://4enjoy.com/");
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
    auto button_hide = [](){return CCFadeTo::create(0.15f, 0);};
    _author_diana->runAction(button_hide());
    _author_andrii->runAction(button_hide());

    _thank->runAction(button_hide());
    _thank1->runAction(button_hide());

    //_x4enjoy_button->runAction(button_hide());
    _flag->runAction(button_hide());
    _reight->runAction(button_hide());


    float delay = 0.2;
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    callback,
                    NULL));
}
void DeveloperScene::showAnimation()
{
    float author_diana_scale = _author_diana->getScale();
    _author_diana->setScale(author_diana_scale*0.9);
    _author_diana->setAnchorPoint(ccp(0.5, 0.5));
    _author_diana->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.7f, author_diana_scale),
                              0.4f));

    float author_andrii_scale = _author_andrii->getScale();
    _author_andrii->setScale(author_andrii_scale*0.9);
    _author_andrii->setAnchorPoint(ccp(0.5, 0.5));
    _author_andrii->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.7f, author_andrii_scale),
                              0.4f));


    auto button_show = [](){return CCFadeTo::create(0.1f, 255);};

    _thank->setOpacity(0);
    _thank->runAction(button_show());

    _thank1->setOpacity(0);
    _thank1->runAction(button_show());

    //x4enjoy logo
    float x4enjoy_logo_scale = _x4enjoy_button->getScale();
    _x4enjoy_button->setScale(x4enjoy_logo_scale*0.9);
    _x4enjoy_button->setAnchorPoint(ccp(0.5, 0.5));
    _x4enjoy_button->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.7f, x4enjoy_logo_scale),
                              0.4f));

    //ukraine flag
    float flag_scale = _flag->getScale();
    _flag->setScale(flag_scale*0.9);
    _flag->setAnchorPoint(ccp(0.5, 0.5));
    _flag->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.7f, flag_scale),
                              0.4f));

    //copy right
    float reight_scale = _reight->getScale();
    _reight->setScale(reight_scale*0.9);
    _reight->setAnchorPoint(ccp(0.5, 0.5));
    _reight->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.7f, reight_scale),
                              0.4f));
}
