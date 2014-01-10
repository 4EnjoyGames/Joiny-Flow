#include "BackButton.h"

BackButton::BackButton(CCLayer *layer, functionPtr function, CCMenu *menu)
{
    start(layer, function, menu);
}

void BackButton::start(CCLayer *layer, functionPtr function, CCMenu *menu)
{
    //Back Button
//    CCSprite* back_logo = CCSprite::create("back_button.png");
//    AnimatedMenuItem* back_button= AnimatedMenuItem::create(
//                back_logo, layer, menu_selector(function));
//    back_logo = CCSprite::create("back_button.png");

//    CCPoint position_bb(ORIGIN.x + 120/SCALE,
//                        ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE);
//    back_button->setPosition(position_bb);
//    back_button->setAnchorPoint(ccp(1, 0.5));

//    back_button->addChild(back_logo);
//    back_logo->setPosition(ccp(back_logo->getContentSize().width/2,
//                              back_logo->getContentSize().height/2));

//    menu->addChild(back_button);
}
