#ifndef DEBUGLAYER_H
#define DEBUGLAYER_H
#include "cocos2d.h"
#include <vector>
class DebugLayer : public cocos2d::CCNode
{
    DebugLayer();
public:
    static DebugLayer* create();
    void addChild(cocos2d::CCNode* node);
    ~DebugLayer();
private:
    void draw();
    typedef std::vector<cocos2d::CCNode*> CCNodeArr;
    CCNodeArr _nodes;
};

#endif // DEBUGLAYER_H
