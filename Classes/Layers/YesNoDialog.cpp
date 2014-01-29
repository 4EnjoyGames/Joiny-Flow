#include "YesNoDialog.h"
#include <ADLib/Device/ADLanguage.h>
YesNoDialog::YesNoDialog(CCNode* content,
            CCObject* yes_callback,
            SEL_CallFunc yes_fun,
            CCObject* no_callback,
            SEL_CallFunc no_fun)
    : _content(content), _yes_obj(yes_callback), _yes_fun(yes_fun),
      _no_obj(no_callback), _no_fun(no_fun)
{
    _content->retain();
}


void YesNoDialog::onCreate(CCNode *parent)
{
    CCSize size = parent->getContentSize();
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();

    parent->addChild(_content);
    _content->release();

    float x_middle = size.width / 2;

    _content->setPosition(ccp(x_middle, size.height*0.65));

    SpritesLoader menu_spl = GraphicsManager::getLoaderFor(parent,
                                                           ADLanguage::localizeFileName("yes_no_menu.plist").c_str(),
                                                           ADLanguage::localizeFileName("yes_no_menu.png").c_str());
    //Create menu items
    //Yes button
    AnimatedMenuItem *yes_button = AnimatedMenuItem::create(
                menu_spl->loadSprite("yes.png"),
                this, menu_selector(YesNoDialog::yes));


    //Settings button
    AnimatedMenuItem *no_button = AnimatedMenuItem::create(
                menu_spl->loadSprite("no.png"),
                this, menu_selector(YesNoDialog::no));

    //Create menu
    MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
    menu->setPosition(ccp(x_middle, size.height*0.3));
    menu->menu()->addChild(yes_button);
    menu->menu()->addChild(no_button);
    menu->menu()->alignItemsHorizontallyWithPadding(70/scaled);
    parent->addChild(menu);
}

void YesNoDialog::yes(CCObject*)
{
    if(_yes_obj && _yes_fun)
    {
        (_yes_obj->*_yes_fun)();
    }
    closeWindow();
}

void YesNoDialog::no(CCObject*)
{
    if(_no_obj && _no_fun)
    {
        (_no_obj->*_no_fun)();
    }
    closeWindow();
}
