#include "SettingScene.h"
#include "MainScene.h"
#include "Localization/CCLocalizedString.h"
#include "GameInfo.h"
#include "Core/MusicSettings.h"
#include "Logic/RW.h"

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

    CCLabelTTF * settings = CCLabelTTF::create( CCLocalizedString("Settings").c_str(),"fonts/Fredoka One.ttf",72);
    settings->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + VISIBLE_SIZE.height - 80/SCALE));
    settings->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(settings);

    //CCSize size = settings->getContentSize();

    ///////////////////////////////////////////////////////////////////
    //Music Button
    _music_logo = CCSprite::create("settings-menu/music.png");

    AnimatedMenuItem* music_button = AnimatedMenuItem::create(
                _music_logo, this, menu_selector(SettingScene::onMusicClicked));

    music_button->addChild(_music_logo);

    CCPoint position_music(ccp(VISIBLE_SIZE.width*0.35,
                                  VISIBLE_SIZE.height - 300/SCALE));
    music_button->setPosition(position_music);
    _music_logo->setPosition(ccp(_music_logo->getContentSize().width/2,
                            _music_logo->getContentSize().height/2));
    _music_logo->setColor(getMusicColor());

    main_menu->addChild(music_button);

    //////////////////////////////////////////////////////////////////

    //Sound Button
    _sound_logo = CCSprite::create("settings-menu/sound.png");

    AnimatedMenuItem* sound_button = AnimatedMenuItem::create(
                _sound_logo, this, menu_selector(SettingScene::onSoundClicked));

    sound_button->addChild(_sound_logo);

    CCPoint position_sound(ccp(VISIBLE_SIZE.width*0.65,
                                  VISIBLE_SIZE.height - 300/SCALE));
    sound_button->setPosition(position_sound);
    _sound_logo->setPosition(ccp(_sound_logo->getContentSize().width/2,
                            _sound_logo->getContentSize().height/2));
    _sound_logo->setColor(getSoundColor());

    main_menu->addChild(sound_button);

    //////////////////////////////////////////////////////////////////

    CCSprite* reset_logo = CCSprite::create("settings-menu/node.png");
    AnimatedMenuItem* reset_button = AnimatedMenuItem::create(
                reset_logo, this, menu_selector(SettingScene::onResetClicked));

    reset_button->addChild(reset_logo);

    CCPoint position_reset(ccp(VISIBLE_SIZE.width*0.5,
                                  VISIBLE_SIZE.height - 520/SCALE));
    reset_button->setPosition(position_reset);
    reset_logo->setPosition(ccp(reset_logo->getContentSize().width/2,
                               reset_logo->getContentSize().height/2));


    CCLabelTTF * reset = CCLabelTTF::create(CCLocalizedString("Reset").c_str(),
                                            "fonts/Fredoka One.ttf",
                                            72);
    reset->setPosition(ccp(reset_logo->getContentSize().width/2,
                           reset_logo->getContentSize().height/2));
    reset->setColor(GameInfo::getInstance()->getTitleColor());
    reset_button->addChild(reset);

    main_menu->addChild(reset_button);

    //////////////////////////////////////////////////////////////////

    CCSprite* restor_logo = CCSprite::create("settings-menu/node.png");
    AnimatedMenuItem* restor_button = AnimatedMenuItem::create(
                restor_logo, this, menu_selector(SettingScene::onRestorClicked));

    restor_button->addChild(restor_logo);

    CCPoint position_restor(ccp(VISIBLE_SIZE.width*0.5,
                                  VISIBLE_SIZE.height - 620/SCALE));
    restor_button->setPosition(position_restor);
    restor_logo->setPosition(ccp(restor_logo->getContentSize().width/2,
                               restor_logo->getContentSize().height/2));


    CCLabelTTF * restor = CCLabelTTF::create(CCLocalizedString("Restore").c_str(),
                                            "fonts/Fredoka One.ttf",
                                            72);
    restor->setPosition(ccp(restor_logo->getContentSize().width/2,
                           restor_logo->getContentSize().height/2));
    restor->setColor(GameInfo::getInstance()->getTitleColor());
    restor_button->addChild(restor);

    main_menu->addChild(restor_button);

    //////////////////////////////////////////////////////////////////

    CCSprite* dev_logo = CCSprite::create("settings-menu/node.png");
    AnimatedMenuItem* dev_button = AnimatedMenuItem::create(
                dev_logo, this, menu_selector(SettingScene::onDevelopersClicked));

    dev_button->addChild(dev_logo);

    CCPoint position_dev(ccp(VISIBLE_SIZE.width*0.5,
                                  VISIBLE_SIZE.height - 720/SCALE));
    dev_button->setPosition(position_dev);
    dev_logo->setPosition(ccp(dev_logo->getContentSize().width/2,
                               dev_logo->getContentSize().height/2));


    CCLabelTTF * dev = CCLabelTTF::create(CCLocalizedString("Developers").c_str(),
                                            "fonts/Fredoka One.ttf",
                                            72);
    dev->setPosition(ccp(dev_logo->getContentSize().width/2,
                           dev_logo->getContentSize().height/2));
    dev->setColor(GameInfo::getInstance()->getTitleColor());
    dev_button->addChild(dev);

    main_menu->addChild(dev_button);


    this->addChild(main_menu);
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
void SettingScene::onMusicClicked(CCObject*)
{
    if (MusicSettings::isMusicOn())
    {
        MusicSettings::turnOffMusic();
    }
    else
    {
        MusicSettings::turnOnMusic();
    }

    _music_logo->setColor(getMusicColor());


    RW::getLevelManager().saveSettings();

}
void SettingScene::onSoundClicked(CCObject*)
{
    if (MusicSettings::isSoundEffectOn())
    {
        MusicSettings::turnOffSoundEffect();
    }
    else
    {
        MusicSettings::turnOnSoundEffect();
    }

    _sound_logo->setColor(getSoundColor());

    RW::getLevelManager().saveSettings();
}
void SettingScene::onResetClicked(CCObject*)
{
    CCLog("onResetClicked clicked");
}
void SettingScene::onRestorClicked(CCObject*)
{
    CCLog("onRestorClicked!!");
}
void SettingScene::onDevelopersClicked(CCObject*)
{
    CCLog("onDevelopersClicked!!");
}

const ccColor3B SettingScene::getSoundColor() const
{
    if (MusicSettings::isSoundEffectOn())
        return GameInfo::getInstance()->getPositiveColor();
    else
        return GameInfo::getInstance()->getNegativeColor();
}
const ccColor3B SettingScene::getMusicColor() const
{
    if(MusicSettings::isMusicOn())
        return GameInfo::getInstance()->getPositiveColor();
    else
        return GameInfo::getInstance()->getNegativeColor();
}
