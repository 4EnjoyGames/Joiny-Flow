
#include "Screen.h"

float Screen::getScaleFactor()
{
    return _design_scale * cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
}

cocos2d::CCPoint Screen::getOrigin()
{
    return cocos2d::CCDirector::sharedDirector()->getVisibleOrigin();
}

cocos2d::CCSize Screen::getVisibleSize()
{
    return cocos2d::CCDirector::sharedDirector()->getVisibleSize();
}
cocos2d::CCSize Screen::getRealSize()
{
    return cocos2d::CCEGLView::sharedOpenGLView()->getFrameSize();
}

void Screen::setDesignScale(float scale)
{
    _design_scale = 1.0f / scale;
}

float Screen::_design_scale = 1;
