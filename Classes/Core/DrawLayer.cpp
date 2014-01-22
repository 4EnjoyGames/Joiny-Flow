#include "DrawLayer.h"
#include "Core/Screen.h"


DrawLayer::DrawLayer()
{
    //const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    //const float SCALE = Screen::getScaleFactor();


    _background = CCSprite::create("universal/game_background.png");


    _main_node = CCNode::create();
    _main_node->retain();
    _main_node->setContentSize(VISIBLE_SIZE);

    this->scheduleUpdate();
    //CCScheduler::scheduleUpdateForTarget(this,1,false);
    //CCScheduler::scheduleSelector(schedule_selector(DrawLayer::update), this, 0.5f, false);
    //CCScheduler.scheduleUpdateForTarget(this,1,false);
//    CCDirector* pDirector = CCDirector::sharedDirector();
//    CCScheduler *scheduler = pDirector->getScheduler();

//    scheduler->scheduleSelector(
//    schedule_selector(DrawLayer::update),
//    this,0,false);

    CCSize win_size = Screen::getRealSize();
    _render = CCRenderTexture::create(win_size.width,
                                      win_size.height,
                                      kTexture2DPixelFormat_RGBA8888);

    _render->retain();

    CCTexture2D *texture = _background->getTexture();
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT};
    texture->setTexParameters(&params);

    //float add_scale = (float)_stripes_texture_size / _base_stripes_texture_size;
    //if(add_scale < 1)
    //    add_scale = 1;

    //size.width/add_scale
    //size.height/add_scale
    CCSprite *noise = CCSprite::createWithTexture(texture,
                                                  CCRectMake(0,
                                                             0,
                                                             win_size.width,
                                                             win_size.height));


    ccBlendFunc function = {GL_DST_COLOR, GL_ZERO};
    noise->setBlendFunc(function);
    noise->setAnchorPoint(ccp(0,0));
    noise->setPosition(ccp(0,0));
    CCLayer::addChild(noise);
}
DrawLayer::~DrawLayer()
{
    _main_node->release();
    _render->release();
}

void DrawLayer::update(float a)
{
    _render->beginWithClear(1,1,1,1);
    _main_node->visit();
    _render->end();
}

void DrawLayer::addChild(CCNode * child)
{
    _main_node->addChild(child);
}

void DrawLayer::addChild(CCNode * child, int zOrder)
{
    _main_node->addChild(child, zOrder);
}

void DrawLayer::addChild(CCNode* child, int zOrder, int tag)
{
    _main_node->addChild(child, zOrder, tag);
}
