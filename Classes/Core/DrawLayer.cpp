#include "DrawLayer.h"
#include "Core/Screen.h"


DrawLayer::DrawLayer()
    : _main_node(nullptr)
{

}


CCSprite* DrawLayer::_white_background = nullptr;
CCSprite* DrawLayer::_background = nullptr;
DrawLayer* DrawLayer::_current_layer = nullptr;
bool DrawLayer::_update_needed = false;





void DrawLayer::createDrawingNodes()
{
    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    //Init global rendering
    CCSize win_size = Screen::getFrameSize();

    //Create sprite for background
    CCSprite* background_piece = CCSprite::create("universal/game_background.jpg");
    CCTexture2D *texture = background_piece->getTexture();
    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT};
    texture->setTexParameters(&params);


    CCSprite* white_background_piece = CCSprite::create("universal/white-background.png");
    CCTexture2D *white_texture = white_background_piece->getTexture();
    white_texture->setTexParameters(&params);


    const float DESIGN_SCALE = Screen::getDesignResourceScale();

    _background = CCSprite::createWithTexture(texture,
                                              CCRectMake(0,
                                                         0,
                                                         win_size.width/DESIGN_SCALE,
                                                         win_size.height/DESIGN_SCALE));


    ccBlendFunc function = {GL_DST_COLOR, GL_ZERO};
    _background->setBlendFunc(function);

    _background->setAnchorPoint(ccp(0,0));
    _background->setPosition(ORIGIN);
    _background->setScale(VISIBLE_SIZE.width/win_size.width*DESIGN_SCALE);
    _background->retain();


    _white_background = CCSprite::createWithTexture(white_texture,
                                                    CCRectMake(0,
                                                               0,
                                                               win_size.width/DESIGN_SCALE,
                                                               win_size.height/DESIGN_SCALE));

    _white_background->setAnchorPoint(ccp(0,0));
    _white_background->setPosition(ORIGIN);
    _white_background->setScale(VISIBLE_SIZE.width/win_size.width*DESIGN_SCALE);
    _white_background->retain();


}


bool DrawLayer::init()
{
    if(!CCLayer::init())
        return false;

    if(_background == nullptr)
    {
        createDrawingNodes();
    }


    _main_node = CCNodeRGBA::create();
    CCLayer::addChild(_main_node, 0, NO_DRAW);

    _current_layer = this;
    return true;
}

void DrawLayer::visit()
{

    //redrawMainNode();

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

    _white_background->visit();

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
                //if(pNode->getTag() != NO_DRAW)
                pNode->visit();
            }
        }
    }
    else
    {
        this->draw();
    }

    _background->visit();

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
    //    if(_update_needed)
    //    {
    //        _update_needed = false;
    //        updateDrawingNodes();
    //    }
    //    _render->beginWithClear(1,1,1,1);

    //    kmGLPushMatrix();
    //    //Take transformation from dummy node
    //    _transform->transform();
    //    //Visit node with main content
    //    _main_node->visit();
    //    kmGLPopMatrix();

    //    _render->end();
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
