#include "MainScene.h"
#include "Core/Screen.h"
#include "SelectCollection.h"
#include "SettingScene.h"
#include "Layers/BackgroundHolder.h"

MainScene::MainScene()
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
    BackgroundHolder::backgroundSwitchTo(scene,back,true);
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
//    if (!CCLayer::init() )
//    {
//        return false;
//    }
//    if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
//    {
//        return false;
//    }

    this->setKeypadEnabled(true);

    //Get the screen start of cordinates
    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    //game logo
    CCSprite* main_logo = CCSprite::create("main-menu/main-logo.png");
    main_logo->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + VISIBLE_SIZE.height*0.80));
    //this->addChild(main_logo);
    this->addChild(main_logo);
    float logo_y_coordinate = main_logo->getPositionY();

    CCMenu* main_menu = CCMenu::create();
    main_menu->setPosition(ORIGIN);

    //Play Button
    //CCSize logo_size = main_logo->getContentSize();
    CCSprite* play_logo = CCSprite::create("main-menu/play-button.png");
    //float play_logo_y_coordinate = play_logo->getPositionY();

    AnimatedMenuItem* play_button = AnimatedMenuItem::create(
                play_logo, this, menu_selector(MainScene::onPlayClicked));

    play_button->addChild(play_logo);

    CCPoint position(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                     logo_y_coordinate - 400/SCALE );

    play_button->setPosition(position);
    play_logo->setPosition(ccp(play_logo->getContentSize().width/2,
                               play_logo->getContentSize().height/2));

    main_menu->addChild(play_button);

    //Settings Button
    CCSprite* settings_logo = CCSprite::create("main-menu/settings-button.png");

    AnimatedMenuItem* settings_button = AnimatedMenuItem::create(
                settings_logo, this, menu_selector(MainScene::onSettingsClicked));

    settings_button->addChild(settings_logo);

    CCPoint position_settings(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                              logo_y_coordinate - 650/SCALE  );
    settings_button->setPosition(position_settings);
    settings_logo->setPosition(ccp(settings_logo->getContentSize().width/2,
                               settings_logo->getContentSize().height/2));

    main_menu->addChild(settings_button);


    this->addChild(main_menu);
    return true;
}

void MainScene::onPlayClicked(CCObject*)
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
    CCLog("Play clicked");
}
void MainScene::onSettingsClicked(CCObject*)
{
    CCDirector::sharedDirector()->replaceScene(SettingScene::scene());
    CCLog("Settings clicked");
}

void MainScene::doGoBack()
{
    CCDirector::sharedDirector()->end();
}
void MainScene::keyBackClicked()
{

    hideEverything(
                CCCallFunc::create(
                    this,
                    callfunc_selector(MainScene::doGoBack)));

}
void MainScene::hideEverything(CCCallFunc *callback)
{
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(0),
                    callback,
                    NULL));

}
