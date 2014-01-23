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
    float _real_scale;
    CCPoint _neg_origin;
    CCRenderTexture* _render;
    CCSprite* _sprite;
    CCSprite* _background;

    CCNodeRGBA* _main_node;
};

#endif // DRAWLAYER_H
