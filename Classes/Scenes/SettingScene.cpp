#include "SettingScene.h"
#include "MainScene.h"
#include "Localization/CCLocalizedString.h"
#include "GameInfo.h"
#include "Core/MusicSettings.h"
#include "Logic/RW.h"
#include "DeveloperScene.h"
#include <ADLib/Device/ADInApp.h>
#include "Core/Fonts.h"

class SettingScene::ResetPopUp : public PopUpWindow::Content
{
public:

    ResetPopUp()
    {}

private:
    typedef ResetPopUp Me;
    SettingScene* _parent;

    void onNo(CCObject*)
    {
        this->closeWindow();
    }

    void onYes(CCObject*)
    {
        RW::getLevelManager().resetProgress();
        this->closeWindow();
    }


    void onCreate(CCNode *parent)
    {

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;

        CCLabelTTF* label = CCLabelTTF::create(_("Sure_reset"),
                                               Fonts::getFontName(),
                                               40);
        label->setColor(ccc3(255,255,255));
        label->setPosition(ccp(x_middle, size.height*0.7f));
        parent->addChild(label);


        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                               "level-end/level_end.plist",
                                                               "level-end/level_end.png");
        MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
        menu->setPosition(ccp(0,0));
        menu->setAnchorPoint(ccp(0,0));
        menu->setContentSize(size);
        parent->addChild(menu);

        CCSprite* parent_rgb = (CCSprite*)parent->getChildByTag(123);
        if(parent_rgb)
            parent_rgb->setColor(GameInfo::getInstance()->getTitleColor());



        float vertical = size.height * 0.18f;

        ///////////////////////////////////////////////////////

        CCSprite* button0 = menu_spl->loadSprite("level_end_button.png");
        button0->setColor(ccc3(255,255,255));

        AnimatedMenuItem *yes_reset = AnimatedMenuItem::create(
                    button0,
                    this, menu_selector(Me::onYes));
        yes_reset->setPosition(ccp(size.width*0.25, vertical));


        CCLabelTTF * yes_reset_text = CCLabelTTF::create(_("Yes"),
                                                Fonts::getFontName(),
                                                48);
        yes_reset_text->setColor(ccc3(255,255,255));
        yes_reset_text->setPosition(ccp(yes_reset->getContentSize().width/2,
                               yes_reset->getContentSize().height/2));


        yes_reset->addChild(yes_reset_text);
        menu->menu()->addChild(yes_reset);

        //////////////////////////////////////////////////////

        CCSprite* button1 = menu_spl->loadSprite("level_end_button.png");
        button1->setColor(ccc3(255,255,255));

        AnimatedMenuItem *no_reset = AnimatedMenuItem::create(
                    button1,
                    this, menu_selector(Me::onNo));
        no_reset->setPosition(ccp(size.width*0.75,vertical));


        CCLabelTTF * no_reset_text = CCLabelTTF::create(_("No"),
                                                Fonts::getFontName(),
                                                48);
        no_reset_text->setColor(ccc3(255,255,255));
        no_reset_text->setPosition(ccp(no_reset->getContentSize().width/2,
                               no_reset->getContentSize().height/2));


        no_reset->addChild(no_reset_text);
        menu->menu()->addChild(no_reset);

    }
};



SettingScene::SettingScene():_pop_up_manager(this)
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
    if (!DrawLayer::init())
    {
        return false;
    }
    this->setKeypadEnabled(true);

    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    _main_menu = CCMenu::create();
    _main_menu->setPosition(ORIGIN);

    //Back Button
    CCMenu* menu = _back.start(this, [this](){this->onButtonBackClicked(0);});
    this->addChild(menu);

//    CCLabelTTF * settings = CCLabelTTF::create(_("Settings"),
//                                               Fonts::getFontName(),
//                                               60);
//    settings->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5 + 25/SCALE,
//                          ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE));
//    settings->setColor(GameInfo::getInstance()->getTitleColor());
//    this->addChild(settings);


    float up_ellements = 120/SCALE;

    CCSprite* music = CCSprite::create("settings-menu/music.png");
    CCSize music_size = music->getContentSize();
    music->removeFromParentAndCleanup(true);

    CCSprite* buttons = CCSprite::create("settings-menu/node.png");
    CCSize buttons_size = buttons->getContentSize();
    buttons->removeFromParentAndCleanup(true);

    float space_between_buttons = buttons_size.height;
    float free_space_sum_y = VISIBLE_SIZE.height -
            up_ellements -
            music_size.height -
            buttons_size.height*4 -
            (space_between_buttons-buttons_size.height)*3;
    float one_free_place = free_space_sum_y/3.0f;
    one_free_place = MIN(one_free_place, VISIBLE_SIZE.height*0.1);

    float music_position_y = VISIBLE_SIZE.height
            - up_ellements
            - one_free_place
            - music_size.height/2.0f;

    float reset_position_y = VISIBLE_SIZE.height
            - up_ellements
            - one_free_place*2
            - music_size.height
            - buttons_size.height/2.0f;

    float restore_position_y = reset_position_y - space_between_buttons;
    float dev_position_y = restore_position_y - space_between_buttons;
    float rate_position_y = dev_position_y - space_between_buttons;

    ///////////////////////////////////////////////////////////////////

    //Music Button
    CCPoint position_music(ccp(VISIBLE_SIZE.width*0.35, music_position_y));
    _music_button = createButton(CCSprite::create("settings-menu/music.png"),
                 menu_selector(SettingScene::onMusicClicked),
                 position_music,
                 "",
                 getMusicColor());

    //////////////////////////////////////////////////////////////////

    //Sound Button
    CCPoint position_sound(ccp(VISIBLE_SIZE.width*0.65, music_position_y));
    _sound_button = createButton(CCSprite::create("settings-menu/sound.png"),
                 menu_selector(SettingScene::onSoundClicked),
                 position_sound,
                 "",
                 getSoundColor());

    //////////////////////////////////////////////////////////////////

    CCPoint position_reset(ccp(VISIBLE_SIZE.width*0.5,reset_position_y));
    AnimatedMenuItem* reset_button = createButton(CCSprite::create("settings-menu/node.png"),
                 menu_selector(SettingScene::onResetClicked),
                 position_reset,
                 _("Reset"),
                 GameInfo::getInstance()->getTitleColor());
    _buttons.push_back(reset_button);


    //////////////////////////////////////////////////////////////////

    if(GameInfo::getVersion() != Version::Premium)
    {
    CCPoint position_restor(ccp(VISIBLE_SIZE.width*0.5,restore_position_y));

    AnimatedMenuItem* restor_button = createButton(CCSprite::create("settings-menu/node.png"),
                 menu_selector(SettingScene::onRestorClicked),
                 position_restor,
                 _("Restore"),
                 GameInfo::getInstance()->getTitleColor());
    _buttons.push_back(restor_button);
    }

    //////////////////////////////////////////////////////////////////

    CCPoint position_dev(ccp(VISIBLE_SIZE.width*0.5,dev_position_y));
    AnimatedMenuItem* developers_button = createButton(CCSprite::create("settings-menu/node.png"),
                 menu_selector(SettingScene::onDevelopersClicked),
                 position_dev,
                 _("Developers"),
                 GameInfo::getInstance()->getTitleColor());
    _buttons.push_back(developers_button);

    //////////////////////////////////////////////////////////////////

    CCPoint position_rate(ccp(VISIBLE_SIZE.width*0.5,rate_position_y));
    AnimatedMenuItem* rate_button = createButton(CCSprite::create("settings-menu/node.png"),
                 menu_selector(SettingScene::onRateMeClicked),
                 position_rate,
                 _("Rate"),
                 GameInfo::getInstance()->getNegativeColor());
    _buttons.push_back(rate_button);


    this->addChild(_main_menu);

    showAnimation();

    return true;
}
AnimatedMenuItem* SettingScene::createButton(CCSprite* logo,
                                SEL_MenuHandler ptr,
                                CCPoint& position,
                                const char* button_text,
                                const ccColor3B& color)
{
    AnimatedMenuItem* button = AnimatedMenuItem::create(
                logo, this, ptr);

    button->addChild(logo);

    button->setPosition(position);
    logo->setPosition(ccp(logo->getContentSize().width/2,
                               logo->getContentSize().height/2));


    if(button_text[0] != '\0')
    {
        const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
        CCLabelTTF * dev = CCLabelTTF::create(button_text,
                                                Fonts::getFontName(),
                                                60);

        if(dev->getContentSize().width >= VISIBLE_SIZE.width)
            dev->setFontSize(40);

        dev->setPosition(ccp(logo->getContentSize().width/2,
                               logo->getContentSize().height/2));
        dev->setColor(color);
        button->addChild(dev);
    }
    else
    {
        logo->setColor(color);
    }

    _main_menu->addChild(button);

    return button;
}
void SettingScene::showAnimation()
{
    auto button_show = [](){return CCFadeTo::create(0.1f, 255);};

    for(unsigned int i=0; i<_buttons.size(); ++i)
    {
        AnimatedMenuItem* curr = _buttons[i];
        curr->setOpacity(0);
        curr->runAction(button_show());
    }

    //animation
    float m_scale_button = _music_button->getScale();
    _music_button->setScale(m_scale_button*0.9);
    _music_button->setAnchorPoint(ccp(0.5, 0.5));
    _music_button->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.7f, m_scale_button),
                              0.4f));

    //animation
    float s_scale_button = _sound_button->getScale();
    _sound_button->setScale(s_scale_button*0.9);
    _sound_button->setAnchorPoint(ccp(0.5, 0.5));
    _sound_button->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.7f, s_scale_button),
                              0.4f));

}

#include <ADLib/Device/ADBrowser.h>
void SettingScene::onRateMeClicked(CCObject*)
{
    ADBrowser::openApplicationPage(GameInfo::getPackageName("joiny"));
}

void SettingScene::doGoBack()
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}
void SettingScene::doDevelopers()
{
    CCDirector::sharedDirector()->replaceScene(DeveloperScene::scene());
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
    float duration = 0.15f;
    CCFadeTo* music_move = CCFadeTo::create(duration, 0);
    _music_button->runAction(music_move);

    CCFadeTo* sound_move = CCFadeTo::create(duration, 0);
    _sound_button->runAction(sound_move);

    auto button_hide = [](){return CCFadeTo::create(0.15f, 0);};
    for(AnimatedMenuItem* item: _buttons)
    {
        item->runAction(button_hide());
    }


    float delay = 0.2;
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
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

    _music_button->setColor(getMusicColor());


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

    _sound_button->setColor(getSoundColor());

    RW::getLevelManager().saveSettings();
}

void SettingScene::onResetClicked(CCObject*)
{
    //ADInApp::buyProduct("unlock_full");
    //CCLog("onResetClicked clicked");

    //reset progress implementing
    _pop_up_manager.openWindow(new ResetPopUp());

    //show good result toast
}
#include <ADLib/Device/ADNotification.h>
void SettingScene::onRestorClicked(CCObject*)
{
    ADInApp::restorePurchases();
    //CCLog("onRestorClicked!!");
}
void SettingScene::onDevelopersClicked(CCObject*)
{
    //CCLog("onDevelopersClicked!!");

    hideEverything(
                CCCallFunc::create(
                    this,
                    callfunc_selector(SettingScene::doDevelopers)));
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
