#include "MenuSpriteBatch.h"
using namespace cocos2d;
MenuSpriteBatch::MenuSpriteBatch(cocos2d::CCMenu* menu, const SpritesLoader& spl)
    :_menu(menu)
{
    if(spl.get() != 0)
    {
        spl->setParent(this);
        spl->inject();
    }

    this->addChild(_menu);
    _menu->setCascadeOpacityEnabled(true);
    _menu->setAnchorPoint(ccp(0,0));
    _menu->setPosition(ccp(0,0));

    //spl->setSpriteSheetPosition(ccp(0,0));
}
MenuSpriteBatch* MenuSpriteBatch::create(const SpritesLoader &spl)
{
    MenuSpriteBatch *pRet = new MenuSpriteBatch(CCMenu::create(), spl);
    if (pRet)
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
