#include "FlowRenderer.h"
#include "Core/Screen.h"
using namespace cocos2d;

FlowRenderer::FlowRenderer(const FlowTable& table)
    : _table(table),
      _nodes_renderers(table.getWidth(), Row(table.getHeight())),
      _active_circle(0)
{
    //Get the sprites loader
    _spl_layer0 = GraphicsManager::getLoaderFor(this,
                                                      "level-scene/flow.plist",
                                                      "level-scene/flow.png");
    _spl_layer0->inject();

    _spl_layer1 = GraphicsManager::getLoaderFor(this,
                                                "level-scene/flow.plist",
                                                "level-scene/flow.png");

    CCSprite* test_texture = _spl_layer0->loadSprite("background.png");
    _textures_size = test_texture->getContentSize().width;
    test_texture->removeFromParent();

   this->setContentSize(CCSize(_textures_size * table.getWidth(),
                               _textures_size * table.getHeight()));



    createNodes();

    //drawTable();

    initNodes();
    createBackground();

    _spl_layer1->inject();

}

void FlowRenderer::updateCell(const FlowPoint cordinates,
                const FlowPointState& new_state)
{
    _nodes_renderers[cordinates.x()][cordinates.y()].update(new_state);
    _table.get(cordinates) = new_state;
}

const cocos2d::ccColor3B& FlowRenderer::getRenderColor(const FlowColor c)
{
    return _colors[c % _colors.size()];
}

const cocos2d::CCSize FlowRenderer::getNodeSize()
{
    return CCSize(_textures_size, _textures_size);
}

const cocos2d::CCPoint FlowRenderer::getNodePosition(const FlowPoint& pos)
{
    float half = _textures_size / 2;
    return CCPoint(half + _textures_size * pos.x(),
                   half + _textures_size * pos.y());
//    return CCPoint( _textures_size * pos.x(),
//                    _textures_size * pos.y());
}


void FlowRenderer::createBackground()
{
    //const CCPoint ORIGIN = Screen::getOrigin();
    //const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    //const float SCALE = Screen::getScaleFactor();

    unsigned int width = _table.getWidth();
    unsigned int height = _table.getHeight();

    CCSprite* tablo = CCSprite::create("level-scene/tablo.png");
    //tablo->setPosition(ORIGIN);

    CCSize tablo_size = tablo->getContentSize();
    CCRenderTexture* render = CCRenderTexture::create(tablo_size.width,
                                                      tablo_size.height,
                                                      kTexture2DPixelFormat_RGBA8888);

    render->beginWithClear(0,0,0,0);

    CCSprite* line = CCSprite::create("level-scene/line.png");
    float cell_width = tablo_size.width / width;

    line->setPositionX(0);

    for(unsigned int y=0; y<=width; ++y)
    {
        float anchor = 0.5f;

        if(y == 0)
            anchor = 0;
        else if(y == width)
            anchor = 1;

        line->setAnchorPoint(ccp(0,anchor));

        line->setPositionY(y*cell_width);
        line->visit();
    }

    line = CCSprite::create("level-scene/line_v.png");
    line->setPositionY(0);


    for(unsigned int y=0; y<=height; ++y)
    {
        float anchor = 0.5f;

        if(y == 0)
            anchor = 0;
        else if(y == height)
            anchor = 1;

        line->setAnchorPoint(ccp(anchor,0));

        line->setPositionX(y*cell_width);
        line->visit();
    }


    tablo->setPosition(ccp(0,0));
    tablo->setAnchorPoint(ccp(0,0));
    //tablo->visit();

    //Finish rendering
    render->end();

    //Create sprite with drawing result
    CCSprite* res = CCSprite::createWithTexture(render->getSprite()->getTexture());
    res->setAnchorPoint(ccp(0,0));
    res->setPosition(ccp(0,0));
    res->setScale(this->getContentSize().width / tablo_size.width);
    this->addChild(res);
}

void FlowRenderer::createNodes()
{
    unsigned int width = _table.getWidth();
    unsigned int height = _table.getHeight();
    for(unsigned int x=0; x<width; ++x)
    {
        for(unsigned int y=0; y<height; ++y)
        {
            FlowRendererNode& node = _nodes_renderers[x][y];
            node.setParent(this);

            FlowPointState* state = &_table(x, y);
            node.setPointState(state);

            CCSprite* highlight = _spl_layer0->loadSprite("background.png");
            //CCSprite* highlight = _spl2->loadSprite("cell_active.png");
            highlight->setOpacity(50);
            highlight->setPosition(this->getNodePosition(FlowPoint(x, y)));

            node.setHighlightSprite(highlight);
        }
    }
}

void FlowRenderer::initNodes()
{
    unsigned int width = _table.getWidth();
    unsigned int height = _table.getHeight();
    for(unsigned int x=0; x<width; ++x)
    {
        for(unsigned int y=0; y<height; ++y)
        {
            FlowRendererNode& node = _nodes_renderers[x][y];
            node.init();
        }
    }
}

cocos2d::CCSprite* FlowRenderer::createSprite(const char* name)
{
    return _spl_layer1->loadSprite(name);
}

FlowRenderer* FlowRenderer::create(const FlowTable& t)
{
    FlowRenderer* r = new FlowRenderer(t);
    if(r)
    {
        r->autorelease();
    }
    return r;
}

FlowRenderer::ColorVec FlowRenderer::_colors=getDefaultColors();
FlowRenderer::ColorVec FlowRenderer::getDefaultColors()
{
    ColorVec v(12);
//    v[0] = ccc3(255, 0, 0); //Red
//    v[1] = ccc3(0, 0, 255); //Blue
//    v[2] = ccc3(29,205,0); //Green
//    v[3] = ccc3(246,255,0); //Yellow
//    v[4] = ccc3(255,0,216); //Pink
//    v[5] = ccc3(255,150,0); //Orange
//    v[6] = ccc3(144,0,210); //Violet
//    v[7] = ccc3(158,0,0); //Dark red
//    v[8] = ccc3(255,255,255); //White
//    v[9] = ccc3(0,255,240); //Light blue
//    v[10] = ccc3(195,255,91); //Light green
//    v[11] = ccc3(1,10,131); //Very dark blue

//    v[0] = ccc3(255, 44, 109); //Red
//    v[1] = ccc3(44, 159, 248); //Blue
//    v[2] = ccc3(59,255,44); //Green
//    v[3] = ccc3(255,248,44); //Yellow
//    v[4] = ccc3(255,54,44); //Pink
//    v[5] = ccc3(255,150,0); //Orange
//    v[6] = ccc3(144,0,210); //Violet
//    v[7] = ccc3(158,0,0); //Dark red
//    v[8] = ccc3(255,255,255); //White
//    v[9] = ccc3(0,255,240); //Light blue
//    v[10] = ccc3(195,255,91); //Light green
//    v[11] = ccc3(1,10,131); //Very dark blue


//    v[0] = ccc3(135, 234, 0); //Green
//    v[1] = ccc3(173, 0, 159); //Violet
//    v[2] = ccc3(255,250,0); //Yellow

//    v[3] = ccc3(255,0,0); //Red
//    v[4] = ccc3(0,178,255); //Light Blue
//    v[5] = ccc3(195,255,0); //Very Light Green

//    v[6] = ccc3(169,0,255); //Violet
//    v[7] = ccc3(252,255,0); //Light Yellow

//    v[8] = ccc3(211,255,0); //Volet
//    v[9] = ccc3(255,0,45); //Red Pink
//    v[10] = ccc3(76,255,0); //Light Green
//    v[11] = ccc3(228,255,0); //Yellow

    v[0] = ccc3(211, 0, 255); //Violet
    v[1] = ccc3(255, 252, 0); //Yellow

    v[2] = ccc3(76,255,0); //Green --

    v[3] = ccc3(255,16,72); //Red

    //v[4] = ccc3(0,72,255); //Blue
    v[4] = ccc3(123,28,253); //Blue

    v[5] = ccc3(255,142,0); //Orange

    v[6] = ccc3(255,0,233); //Pink

    v[7] = ccc3(0,155,255); //Light blue

    return v;
}

const FlowTable& FlowRenderer::getTable() const
{
    return _table;
}

void FlowRenderer::showActiveCircle(const cocos2d::CCPoint& p,
                                    const FlowColor color)
{
    if(_active_circle == 0)
    {
        _active_circle = createSprite("circle.png");
        _active_circle->setZOrder(1000);
        _active_circle->setScale(2);
        _active_circle->setOpacity(100);
    }

    _active_circle->setVisible(true);
    _active_circle->setColor(getRenderColor(color));
    _active_circle->setPosition(p);
}

void FlowRenderer::hideActiveCircle()
{
    if(_active_circle != 0)
    {
        _active_circle->setVisible(false);
    }
}
