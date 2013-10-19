#ifndef CCNODEWITHOPACITY_H
#define CCNODEWITHOPACITY_H
#include "cocos2d.h"
class CCNodeWithOpacity : public cocos2d::CCNode,
        public cocos2d::CCRGBAProtocol
{
public:
    CCNodeWithOpacity() : _opacity(0)
    {

    }

    static CCNodeWithOpacity * create(void)
    {
        CCNodeWithOpacity * pRet = new CCNodeWithOpacity();
        pRet->autorelease();
        return pRet;
    }

    virtual void setColor(const cocos2d::ccColor3B&)
    {}
    virtual cocos2d::ccColor3B getColor(void)
    {
        return cocos2d::ccc3(0,0,0);
    }

    virtual GLubyte getOpacity(void)
    {
        return _opacity;
    }

    virtual void setOpacity(GLubyte op)
    {
        _opacity = op;
    }

    virtual void setOpacityModifyRGB(bool)
    {}
    virtual bool isOpacityModifyRGB(void)
    {
        return false;
    }
private:
    GLubyte _opacity;
};

#endif // CCNODEWITHOPACITY_H
