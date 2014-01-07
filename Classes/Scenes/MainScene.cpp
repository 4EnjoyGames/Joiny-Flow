#include "MainScene.h"
#include "Core/Screen.h"
#include "SelectCollection.h"

MainScene::MainScene()
{
}

CCScene* MainScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MainScene *layer = MainScene::create();

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
    if (!CCLayer::init() )
    {
        return false;

    }
//    if (!CCLayerColor::initWithColor(ccc4(231, 232, 232, 255)))
//    {
//        return false;
//    }

    //Get the screen start of cordinates
    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();


    //Load one piece
    CCSprite* sp_noise = CCSprite::create("main-menu/back.png");

    //Take the texture from sprite
    CCTexture2D *texture = sp_noise->getTexture();

    //Set parameters GL_MIRRORED_REPEAT mean that texture should repeat one time mirrored other time not
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT};
    texture->setTexParameters(&params);

    //Final sprite
    CCSprite *noise = CCSprite::createWithTexture(texture, CCRectMake(0, 0, VISIBLE_SIZE.width, VISIBLE_SIZE.height));
    noise->setPosition(ccp(ORIGIN.x +  VISIBLE_SIZE.width/2,
                           ORIGIN.y + VISIBLE_SIZE.height/2));
    this->addChild(noise);


    //downer background
    CCSprite* background = CCSprite::create("main-menu/background.png");
    background->setPosition(ccp(ORIGIN.x +  VISIBLE_SIZE.width/2,
                                ORIGIN.y /*+ VISIBLE_SIZE.height/2*/));
    this->addChild(background);

    //upper background
    CCSprite* background_upper = CCSprite::create("main-menu/background.png");
    background_upper->setPosition(ccp(ORIGIN.x +  VISIBLE_SIZE.width/2,
                                ORIGIN.y + VISIBLE_SIZE.height));
    background_upper->setFlipY(true);
    this->addChild(background_upper);

    //game logo
    CCSprite* main_logo = CCSprite::create("main-menu/main-logo.png");
    main_logo->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + VISIBLE_SIZE.height*0.75));
    this->addChild(main_logo);


    CCMenu* main_menu = CCMenu::create();
    main_menu->setPosition(ORIGIN);

    //Play Button
    CCSprite* play_logo = CCSprite::create("main-menu/play-button.png");

    AnimatedMenuItem* play_button = AnimatedMenuItem::create(
                play_logo, this, menu_selector(MainScene::onPlayClicked));

    play_button->addChild(play_logo);

    CCPoint position(VISIBLE_SIZE.width*0.5,
                     VISIBLE_SIZE.height*0.42);
    play_button->setPosition(position);
    play_logo->setPosition(ccp(play_logo->getContentSize().width/2,
                               play_logo->getContentSize().height/2));

    main_menu->addChild(play_button);

    //Settings Button
    CCSprite* settings_logo = CCSprite::create("main-menu/settings-button.png");

    AnimatedMenuItem* settings_button = AnimatedMenuItem::create(
                settings_logo, this, menu_selector(MainScene::onSettingsClicked));

    settings_button->addChild(settings_logo);

    CCPoint position_settings(VISIBLE_SIZE.width*0.78,
                              VISIBLE_SIZE.height*0.16);
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
    CCLog("Settings clicked");
}
