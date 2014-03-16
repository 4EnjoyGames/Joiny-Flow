#include "MainScene.h"
#include "Core/Screen.h"
#include "SelectCollection.h"
#include "SettingScene.h"
#include "Layers/BackgroundHolder.h"
#include "Localization/CCLocalizedString.h"
#include "GameInfo.h"
#include "Core/Fonts.h"


class MainScene::GoOutPopUp : public PopUpWindow::Content
{
public:

    GoOutPopUp(MainScene* parent): _parent(parent)
    {}

private:
    typedef GoOutPopUp Me;
    MainScene* _parent;

    void onNo(CCObject*)
    {
        this->closeWindow();
    }

    void onYes(CCObject*)
    {
        CCDirector::sharedDirector()->end();
        //TODO: leave the game
    }


    void onCreate(CCNode *parent)
    {

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;

        CCLabelTTF* label = CCLabelTTF::create(_("Sure"),
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

MainScene::MainScene():_pop_up_manager(this)
{
}

CCScene* MainScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MainScene *layer = MainScene::create();

    // add layer as a child to scene
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(MainScene::onKeyBackClicked));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}


MainScene* MainScene::create()
{

    MainScene *pRet = new MainScene();
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
bool MainScene::init()
{
    if(!DrawLayer::init())
        return false;
    this->setKeypadEnabled(true);

    //Get the screen start of cordinates
    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    //game logo
    _main_logo = CCSprite::create("main-menu/main-logo.png");

    //animation
    CCPoint logo_target_position = ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                                       ORIGIN.y + VISIBLE_SIZE.height*0.80);
    _main_logo->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                           2*VISIBLE_SIZE.height));

    CCMoveTo* logo_move = CCMoveTo::create(0.35f, logo_target_position);
    _main_logo->runAction(logo_move);

    //this->addChild(main_logo);
    this->addChild(_main_logo);
    float logo_y_coordinate = ORIGIN.y + VISIBLE_SIZE.height*0.80;

    CCMenu* main_menu = CCMenu::create();
    main_menu->setPosition(ORIGIN);

    //Play Button
    CCSprite* play_logo = CCSprite::create("main-menu/play-button.png");

    _play_button = AnimatedMenuItem::create(
                play_logo, this, menu_selector(MainScene::onPlayClicked));

    _play_button->addChild(play_logo);

    CCPoint position(VISIBLE_SIZE.width*0.5,
                     logo_y_coordinate - 400/SCALE );

    _play_button->setPosition(position);
    play_logo->setPosition(ccp(play_logo->getContentSize().width/2,
                               play_logo->getContentSize().height/2));

    //animation
    float scale_play_button = _play_button->getScale();
    _play_button->setScale(scale_play_button*0.9);
    _play_button->setAnchorPoint(ccp(0.5, 0.5));
    _play_button->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.5f, scale_play_button),
                              0.4f));


    main_menu->addChild(_play_button);

    //Settings Button
    CCSprite* settings_logo = CCSprite::create("main-menu/settings-button.png");

    _settings_button = AnimatedMenuItem::create(
                settings_logo, this, menu_selector(MainScene::onSettingsClicked));

    _settings_button->addChild(settings_logo);

    CCPoint position_settings(VISIBLE_SIZE.width*0.5,
                              logo_y_coordinate - 650/SCALE  );
    _settings_button->setPosition(position_settings);
    settings_logo->setPosition(ccp(settings_logo->getContentSize().width/2,
                               settings_logo->getContentSize().height/2));

    //animation
    float scale_setting_button = _play_button->getScale();
    _settings_button->setScale(scale_setting_button*0.9);
    //_play_button->setScaleY(0);
    _settings_button->setAnchorPoint(ccp(0.5, 0.5));
    _settings_button->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.5f, scale_setting_button),
                              0.4f));
    main_menu->addChild(_settings_button);

    //facebook logo
    CCSprite* _facebook = CCSprite::create("main-menu/facebook_logo.png");
    _facebook_button = AnimatedMenuItem::create(
                _facebook,
                this,
                menu_selector(MainScene::onFacebookClicked));
    _facebook_button->addChild(_facebook);

    CCPoint position_facebook(VISIBLE_SIZE.width - 100/SCALE,
                              100/SCALE);
    _facebook_button->setPosition(position_facebook);
    _facebook->setPosition(ccp(_facebook->getContentSize().width/2,
                               _facebook->getContentSize().height/2));
    //animation
    float scale_facebook_button = _facebook_button->getScale();
    _facebook_button->setScale(scale_facebook_button*0.9);
    _facebook_button->setAnchorPoint(ccp(0.5, 0.5));
    _facebook_button->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.5f, scale_facebook_button),
                              0.4f));
    main_menu->addChild(_facebook_button);


    this->addChild(main_menu);
    return true;
}

void MainScene::onPlayClicked(CCObject*)
{
    hideEverything(
                CCCallFunc::create(
                    this,
                    callfunc_selector(MainScene::doOnPlayClicked)));
}
void MainScene::doOnPlayClicked()
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
    CCLog("Play clicked");
}
#include "ADLib/Device/ADBrowser.h"
void MainScene::onFacebookClicked(CCObject*)
{
    CCLog("On facebook clicked");
    ADBrowser::openWebURL("http://4enjoy.com/r/auto/100/");
}

void MainScene::onSettingsClicked(CCObject*)
{
    hideEverything(
                CCCallFunc::create(
                    this,
                    callfunc_selector(MainScene::doOnSettingsClicked)));
}
void MainScene::doOnSettingsClicked()
{
    CCDirector::sharedDirector()->replaceScene(SettingScene::scene());
    CCLog("Settings clicked");
}

void MainScene::doGoBack()
{
    _pop_up_manager.openWindow(new GoOutPopUp(this));

}
void MainScene::keyBackClicked()
{
    if(!_pop_up_manager.backAction())
    {
        doGoBack();
    }
}
void MainScene::hideEverything(CCCallFunc *callback)
{

    float duration = 0.15f;
    CCFadeTo* settings_move = CCFadeTo::create(duration, 0);
    CCFadeTo* play_move = CCFadeTo::create(duration, 0);
    CCFadeTo* logo_move = CCFadeTo::create(duration, 0);
    CCFadeTo* facebook_move = CCFadeTo::create(duration, 0);
    _settings_button->runAction(settings_move);
    _play_button->runAction(play_move);
    _main_logo->runAction(logo_move);
    _facebook_button->runAction(facebook_move);


    float delay = 0.2;
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    callback,
                    NULL));

}
