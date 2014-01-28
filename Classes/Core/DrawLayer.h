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
    static void registerUpdateDrawingNodes();
private:
    static void updateDrawingNodes();
    static void createDrawingNodes();
    static void attachDrawingNodesToLayer(DrawLayer* layer);
    void redrawMainNode();
    static const int NO_DRAW = 911;

    static CCRenderTexture* _render;
    static CCSprite* _sprite;
    static CCSprite* _background;
    static CCNode* _transform;

    CCNodeRGBA* _main_node;
    static DrawLayer* _current_layer;
    static bool _update_needed;
};

#endif // DRAWLAYER_H
