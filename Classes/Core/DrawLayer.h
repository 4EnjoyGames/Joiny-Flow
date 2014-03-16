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
    static void createDrawingNodes();
    void redrawMainNode();
    static const int NO_DRAW = 911;

    static CCSprite* _white_background;
    static CCSprite* _background;

    CCNodeRGBA* _main_node;
    static DrawLayer* _current_layer;
    static bool _update_needed;
};

#endif // DRAWLAYER_H
