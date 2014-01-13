#include "BackButton.h"

BackButton::BackButton():
    _function(nullptr),
    _layer(nullptr)
{
}

CCMenu* BackButton::start(CCLayer *layer, functionPtr function)
{
    _function = function;
    _layer = layer;

    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    //Back Button
    CCSprite* back_logo = CCSprite::create("back_button.png");
    AnimatedMenuItem* back_button= AnimatedMenuItem::create(
                back_logo, this, menu_selector(BackButton::doAction));
    // menu_selector(BackButton::doAction)
    back_logo = CCSprite::create("back_button.png");

    CCPoint position_bb(ORIGIN.x + 120/SCALE,
                        ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE);
    back_button->setPosition(position_bb);
    back_button->setAnchorPoint(ccp(1, 0.5));

    back_button->addChild(back_logo);
    back_logo->setPosition(ccp(back_logo->getContentSize().width/2,
                              back_logo->getContentSize().height/2));

    CCMenu* main_menu = CCMenu::create();
    main_menu->setPosition(ccp(0,0));

    main_menu->addChild(back_button);

    return main_menu;
}
void BackButton::doAction(CCObject* obj)
{
    //_function;
    _function();
}
