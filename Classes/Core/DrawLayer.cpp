#include "DrawLayer.h"
#include "Core/Screen.h"


DrawLayer::DrawLayer()
    : _main_node(nullptr)
{

}

CCRenderTexture* DrawLayer::_render = nullptr;
CCSprite* DrawLayer::_sprite = nullptr;
CCSprite* DrawLayer::_background = nullptr;
CCNode* DrawLayer::_transform = nullptr;
DrawLayer* DrawLayer::_current_layer = nullptr;
bool DrawLayer::_update_needed = false;

void DrawLayer::registerUpdateDrawingNodes()
{
    _update_needed = true;
}

void DrawLayer::updateDrawingNodes()
{
    if(_current_layer && _render)
    {
        _sprite->removeFromParent();
        _background->removeFromParent();

        _render->release();
        _sprite->release();
        _transform->release();
        _background->release();

        _render = nullptr;
        _sprite = nullptr;
        _transform = nullptr;
        _background = nullptr;

        createDrawingNodes();
        attachDrawingNodesToLayer(_current_layer);
    }
}

void DrawLayer::createDrawingNodes()
{
    assert(_render == nullptr);
    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    //Init global rendering
    CCSize win_size = Screen::getFrameSize();

    //Create sprite for background
    CCSprite* background_piece = CCSprite::create("universal/game_background.png");
    CCTexture2D *texture = background_piece->getTexture();
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT};
    texture->setTexParameters(&params);

    const float DESIGN_SCALE = Screen::getDesignResourceScale();

    _background = CCSprite::createWithTexture(texture,
                                                  CCRectMake(0,
                                                             0,
                                                             win_size.width/DESIGN_SCALE,
                                                             win_size.height/DESIGN_SCALE));


    _background->setAnchorPoint(ccp(0,0));
    _background->setPosition(ORIGIN);
    _background->setScale(VISIBLE_SIZE.width/win_size.width*DESIGN_SCALE);
    _background->retain();

    //Prepare render node
    _render = CCRenderTexture::createNoScale(win_size.width/DESIGN_SCALE,
                                             win_size.height/DESIGN_SCALE,
                                             kTexture2DPixelFormat_RGBA8888);
    _render->retain();

    float _real_scale = win_size.width/VISIBLE_SIZE.width/SCALE/DESIGN_SCALE;
    CCPoint _neg_origin = ORIGIN * -_real_scale;

    //Prepare transform for rendering
    _transform = CCNode::create();
    _transform->setPosition(_neg_origin);
    _transform->setScale(_real_scale);
    _transform->retain();


    //Create display sprite
    _sprite = CCSprite::createWithTexture(_render->getSprite()->getTexture());
    ccBlendFunc function = {GL_DST_COLOR, GL_ZERO};
    _sprite->setBlendFunc(function);
    _sprite->setAnchorPoint(ccp(0,0));
    _sprite->setPosition(ORIGIN);
    _sprite->setFlipY(true);
    _sprite->setScale(1.0/_real_scale);
    _sprite->retain();
}
void DrawLayer::attachDrawingNodesToLayer(DrawLayer* layer)
{
    if(_background->getParent())
        _background->removeFromParent();
    if(_sprite->getParent())
        _sprite->removeFromParent();

    layer->CCLayer::addChild(_background, 0, 0);
    layer->CCLayer::addChild(_sprite, 0, 0);
}

bool DrawLayer::init()
{
    if(!CCLayer::init())
        return false;

    if(_render == nullptr)
    {
        createDrawingNodes();
    }


    _main_node = CCNodeRGBA::create();
    attachDrawingNodesToLayer(this);
    CCLayer::addChild(_main_node, 0, NO_DRAW);

    _current_layer = this;
    return true;
}

void DrawLayer::visit()
{

    redrawMainNode();

    // quick return if not visible. children won't be drawn.
    if (!m_bVisible)
    {
        return;
    }
    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
    }

    this->transform();

    CCNode* pNode = NULL;
    unsigned int i = 0;

    if(m_pChildren && m_pChildren->count() > 0)
    {
        // draw children zOrder < 0
        ccArray *arrayData = m_pChildren->data;

        // self draw
        this->draw();

        for( ; i < arrayData->num; i++ )
        {
            pNode = (CCNode*) arrayData->arr[i];
            if (pNode)
            {
                if(pNode->getTag() != NO_DRAW)
                    pNode->visit();
            }
        }
    }
    else
    {
        this->draw();
    }

    // reset for next frame
    m_uOrderOfArrival = 0;

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
}

DrawLayer::~DrawLayer()
{
}
void DrawLayer::redrawMainNode()
{
    if(_update_needed)
    {
        _update_needed = false;
        updateDrawingNodes();
    }
    _render->beginWithClear(1,1,1,1);

    kmGLPushMatrix();
    //Take transformation from dummy node
    _transform->transform();
    //Visit node with main content
    _main_node->visit();
    kmGLPopMatrix();

    _render->end();
}

void DrawLayer::update(float a)
{

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
