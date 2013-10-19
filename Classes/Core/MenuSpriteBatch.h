#ifndef ANIMATEDMENU_H
#define ANIMATEDMENU_H
#include "cocos2d.h"
#include "GraphicsManager.h"
class MenuSpriteBatch : public cocos2d::CCNode
{
    MenuSpriteBatch(cocos2d::CCMenu* menu, const SpritesLoader &spl);
public:
    static MenuSpriteBatch* create(const SpritesLoader &spl);
    cocos2d::CCMenu* menu() const
    {
        return _menu;
    }
    /*virtual void addChild(CCNode * child)
    {
        CCNode::addChild(child);
    }*/
private:


    cocos2d::CCMenu* _menu;
};

#endif // ANIMATEDMENU_H
