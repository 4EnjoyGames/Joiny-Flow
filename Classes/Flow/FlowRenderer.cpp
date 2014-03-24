#include "FlowRenderer.h"
#include "Core/Screen.h"
using namespace cocos2d;

FlowRenderer::FlowRenderer(const FlowTable& table)
    : _table(table),
      _nodes_renderers(table.getWidth(), Row(table.getHeight())),
      _active_circle(0)
{

    _spl_layer1 = GraphicsManager::getLoaderFor(this,
                                                "level-scene/flow.plist",
                                                "level-scene/flow.png");


    //hints sprite loader
    _spl_layer05 = GraphicsManager::getLoaderFor(this,
                                                 "level-scene/flow.plist",
                                                 "level-scene/flow.png");






    //Get the sprites loader
    _spl_layer0 = GraphicsManager::getLoaderFor(this,
                                                "level-scene/flow.plist",
                                                "level-scene/flow.png");
    _spl_layer0->inject();










    CCSprite* test_texture = _spl_layer0->loadSprite("background.png");
    _textures_size = test_texture->getContentSize().width;
    test_texture->removeFromParent();

    this->setContentSize(CCSize(_textures_size * table.getWidth(),
                                _textures_size * table.getHeight()));



    createNodes();

    //drawTable();

    initNodes();
    createBackground();

    _spl_layer05->inject();
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

void FlowRenderer::doDrawTable()
{
    float scale = _table_scale;
    CCSize tablo_size = _table_size;

    //const CCPoint ORIGIN = Screen::getOrigin();
    //const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    //const float SCALE = Screen::getScaleFactor();

    unsigned int width = _table.getWidth();
    unsigned int height = _table.getHeight();

    //CCSprite* tablo = CCSprite::create("level-scene/tablo.png");
    //tablo->setPosition(ORIGIN);

    const float DESIGN_SCALE = Screen::getDesignResourceScale();
    const CCSize FRAME_SIZE = Screen::getFrameSize();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();
    float cocos_to_screen_coef = FRAME_SIZE.width / (VISIBLE_SIZE.width);


    float line_width = 1.0f;


    //CCSize tablo_size = tablo->getContentSize();

    CCLog("Table size = ");
    CCSize tablo_size_real = tablo_size * scale * cocos_to_screen_coef;

    tablo_size_real.width = (int)(tablo_size_real.width);
    tablo_size_real.height = (int)(tablo_size_real.height);

    float base_mutiply = 420;
    if(tablo_size_real.width > base_mutiply)
    {
        line_width = ((int)(tablo_size_real.width*2 / base_mutiply + 0.5));
        line_width /= 2;
    }
    CCLog("Line width: %f", line_width);
    CCLog("Tablo size: %f, Screen_width: %f, Design scale: %f, Scale: %f",
          tablo_size_real.width,
          FRAME_SIZE.width, DESIGN_SCALE, SCALE);

    CCRenderTexture* render = CCRenderTexture::createNoScale(
                tablo_size_real.width,
                tablo_size_real.height,
                kTexture2DPixelFormat_RGBA8888);

    render->beginWithClear(0,0,0,0);

    float cell_width = tablo_size_real.width / width;



    CCSprite* line = CCSprite::create("level-scene/line.png");
    line->setScale(line_width/SCALE);
    line->setScaleX(tablo_size_real.width / line->getContentSize().width);

    for(unsigned int y=0; y<=width; ++y)
    {
        float anchor = 0.0f;

        if(y == width)
            anchor = 1;

        line->setAnchorPoint(ccp(0,anchor));

        line->setPositionY((int)(y*cell_width/SCALE));
        line->visit();
    }

    line = CCSprite::create("level-scene/line_v.png");
    line->setPositionY(0);

    line->setScale(line_width/SCALE);
    line->setScaleY(tablo_size_real.height / line->getContentSize().height);
    //line->setScaleY(tablo_size.height * scale / line->getContentSize().height);


    for(unsigned int y=0; y<=height; ++y)
    {
        float anchor = 0.0f;

        if(y == height)
            anchor = 1;

        line->setAnchorPoint(ccp(anchor,0));

        line->setPositionX((int)(y*cell_width/SCALE));
        line->visit();
    }



    //Finish rendering
    render->end();

    const int TABLE_TAG = 10055;
    CCNode* old = this->getChildByTag(TABLE_TAG);
    if(old)
        old->removeFromParent();

    //Create sprite with drawing result
    CCSprite* res = CCSprite::createWithTexture(render->getSprite()->getTexture());
    res->setAnchorPoint(ccp(0,1-DESIGN_SCALE));
    res->setPosition(ccp(0,0));
    res->setScale(tablo_size.width / tablo_size_real.width * SCALE);
    this->addChild(res, -1, TABLE_TAG);
}
#include <ADLib/Device/ADDeviceEvents.h>
void FlowRenderer::drawTable(float scale, CCSize tablo_size)
{
    _table_scale = scale;
    _table_size = tablo_size;
    doDrawTable();
    CONNECT(ADDeviceEvents::signalOnResume,
            this, &FlowRenderer::doDrawTable);
}

void FlowRenderer::createBackground()
{

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
cocos2d::CCSprite* FlowRenderer::createHintSprite( const char* name)
{
    return _spl_layer05->loadSprite(name);
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

    v[0] = ccc3(211, 0, 255); //Violet
    v[1] = ccc3(255, 252, 0); //Yellow

    v[2] = ccc3(76,255,0); //Green --

    v[3] = ccc3(255,16,72); //Red

    v[4] = ccc3(123,28,253); //Blue

    v[5] = ccc3(255,142,0); //Orange

    //v[6] = ccc3(255,0,233); //Pink
    v[6] = ccc3(234, 255, 0); //Lime Yellow

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
