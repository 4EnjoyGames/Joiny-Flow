#ifndef DRAWLAYER_H
#define DRAWLAYER_H
#include "cocos2d-A.h"


class DrawLayer: public cocos2d::CCLayer
{
public:
    DrawLayer();
    ~DrawLayer();
    void update(float a);
    void addChild(CCNode * child);
    void addChild(CCNode * child, int zOrder);
    void addChild(CCNode* child, int zOrder, int tag);
    bool init();
    void visit();
private:
    static const int NO_DRAW = 911;

    static CCRenderTexture* _render;
    static CCSprite* _sprite;
    static CCSprite* _background;
    static CCNode* _transform;

    CCNodeRGBA* _main_node;

};

#endif // DRAWLAYER_H
