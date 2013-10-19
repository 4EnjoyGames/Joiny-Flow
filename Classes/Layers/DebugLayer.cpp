#include "DebugLayer.h"
//#include "cocos2d.h"
using namespace cocos2d;
DebugLayer::DebugLayer()
{}

DebugLayer* DebugLayer::create()
{
    DebugLayer *pRet = new DebugLayer();
    pRet->autorelease();
    return pRet;
}
void DebugLayer::addChild(cocos2d::CCNode* node)
{
    _nodes.push_back(node);
    node->retain();
}

DebugLayer::~DebugLayer()
{
    for(unsigned int i=0; i<_nodes.size(); ++i)
    {
        _nodes[i]->release();
    }
}
void DebugLayer::draw()
{
    ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);

    for(unsigned int i=0; i<_nodes.size(); ++i)
    {
        CCNode* n=_nodes[i];
        CCSize s = n->getContentSize();
        s.height *= n->getScaleY();
        s.width *= n->getScaleX();
        CCPoint p = n->getPosition();
        CCRect bb = n->boundingBox();
        ccDrawRect(bb.origin,
                   ccpAdd(bb.origin, (ccp(bb.size.width,bb.size.height))));
        ccDrawCircle(p,5,180,5,false);
    }
}
