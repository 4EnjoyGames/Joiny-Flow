#include "FlowRenderer.h"
#include "Core/Screen.h"
using namespace cocos2d;

FlowRenderer::FlowRenderer(const FlowTable& table)
    : _table(table), _nodes_renderers(table.getWidth(), Row(table.getHeight())),
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

   // _spl2 = GraphicsManager::getLoaderFor(this,
     //                                                  "level-scene/flow2.plist",
     //                                                  "level-scene/flow2.png");
     //_spl2->inject();

     //CCSprite* probe = _spl2->loadSprite("cell_active.png");
     //CCSize probe_size = probe->getContentSize();
     //_probe_size = probe_size.width;
     //_textures_size = _probe_size;
     //probe->removeFromParentAndCleanup(true);
     //this->setContentSize(CCSize(_probe_size * table.getWidth(),
     //                            _probe_size * table.getHeight()));

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

//void FlowRenderer::drawBorder(CCSprite* img,
//                int x, int y,
//                int rotate, bool flip_y)
//{
//    img->setColor(ccc3(0,0,0));
//    img->setPosition(this->getNodePosition(FlowPoint(x, y)));
//    //img->setPosition(ccp(cell_width * (x+1), cell_width * (y+1)));
//    img->setRotation(static_cast<float>(rotate));
//    img->setFlipY(flip_y);
//    //img->visit();
//}

//void FlowRenderer::drawTable()
//{
//    //Open image of active cell
//    CCSprite* c_filled = _spl2->loadSprite("cell_active.png");
//    //c_filled->setAnchorPoint(ccp(0.5f,0.5f));

//    CCSprite* c_border = _spl2->loadSprite("cell_top.png");
//    //c_border->setAnchorPoint(ccp(0.5f, 0.5f));

//    CCSprite* c_corner_straight = _spl2->loadSprite("cell_corner_straight.png");
//    //c_corner_straight->setAnchorPoint(ccp(0.5f, 0.5f));

//    CCSprite* c_corner_round = _spl2->loadSprite("cell_corner_round.png");
//    //c_corner_round->setAnchorPoint(ccp(0.5f, 0.5f));

//    CCSprite* c_corner_outer = _spl2->loadSprite("cell_corner_outer.png");
//    //c_corner_outer->setAnchorPoint(ccp(0.5f, 0.5f));

//    //Placement of the border
////    auto drawBorder = [cell_width](
////            CCSprite* img,
////            int x, int y,
////            int rotate, bool flip_y)
////    {
////        img->setPosition(this->getNodePosition(FlowPoint(cell_width * (x+1), cell_width * (y+1))));
////        //img->setPosition(ccp(cell_width * (x+1), cell_width * (y+1)));
////        img->setRotation(rotate);
////        img->setFlipY(flip_y);
////        img->visit();
////    };


//    unsigned int width = _table.getWidth();
//    unsigned int height = _table.getHeight();

//    auto isShowableCell = [this](int x, int y)
//    {
//        unsigned int width = _table.getWidth();
//        unsigned int height = _table.getHeight();

//        if(x>=0 && x<width && y>=0 && y<height)
//            return true;
//        else
//            return false;
//    };


//    for(int x=0; x<width; ++x)
//        for(int y=0; y<height; ++y)
//        {
//            //TablePoint cord(x,y);
//            //const Cell* cell = _table_info->getCell(x, y);
////            if(!holes_inside &&
////                    (y != -1 &&
////                     x != -1 &&
////                     y != height &&
////                     x != width))
////            {
////                drawBorder(c_filled, x, y, 0, false);
////            }
////            else
//            {
//                drawBorder(c_filled, x, y, 0, false);

//                bool neig_top = isShowableCell(x, y+1);
//                bool neig_bottom = isShowableCell(x, y-1);
//                bool neig_right = isShowableCell(x+1, y);
//                bool neig_left = isShowableCell(x-1, y);
//                if(neig_top)
//                {
//                    drawBorder(c_border, x, y, 0, false);
//                    if(neig_left)
//                        drawBorder(c_corner_round, x, y, 0, false);

//                    if(!neig_left)
//                        drawBorder(c_corner_straight, x, y, 0, false);
//                    if(!neig_right)
//                        drawBorder(c_corner_straight, x, y, 180, true);
//                }
//                if(neig_left)
//                {
//                    drawBorder(c_border, x, y, 90, true);
//                    if(neig_bottom)
//                        drawBorder(c_corner_round, x, y, 0, true);

//                    if(!neig_bottom)
//                        drawBorder(c_corner_straight, x, y, -90, false);
//                    if(!neig_top)
//                        drawBorder(c_corner_straight, x, y, 90, true);
//                }
//                if(neig_bottom)
//                {
//                    drawBorder(c_border, x, y, 0, true);
//                    if(neig_right)
//                        drawBorder(c_corner_round, x, y, 180, false);

//                    if(!neig_right)
//                        drawBorder(c_corner_straight, x, y, 180, false);
//                    if(!neig_left)
//                        drawBorder(c_corner_straight, x, y, 0, true);
//                }
//                if(neig_right)
//                {
//                    drawBorder(c_border, x, y, 90, false);
//                    if(neig_top)
//                        drawBorder(c_corner_round, x, y, 90, false);

//                    if(!neig_top)
//                        drawBorder(c_corner_straight, x, y, 90, false);
//                    if(!neig_bottom)
//                        drawBorder(c_corner_straight, x, y, -90, true);
//                }

//                if(!neig_bottom && !neig_left && isShowableCell(x-1, y-1))
//                    drawBorder(c_corner_outer, x, y, 180, false);

//                if(!neig_bottom && !neig_right && isShowableCell(x+1, y-1))
//                    drawBorder(c_corner_outer, x, y, 0, true);

//                if(!neig_top && !neig_left && isShowableCell(x-1, y+1))
//                    drawBorder(c_corner_outer, x, y, 180, true);

//                if(!neig_top && !neig_right && isShowableCell(x+1, y+1))
//                    drawBorder(c_corner_outer, x, y, 0, false);
//            }
//        }
//}

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
//    for(unsigned int x=1; x<width+1; ++x)
//    {
//            line->setPosition(this->getNodePosition(FlowPoint(x,0)));
//            line->setRotation(90.0f);
//            curr_width+=cell_width;
//            line->visit();

//    }



    tablo->setPosition(ccp(0,0));
    tablo->setAnchorPoint(ccp(0,0));
    //tablo->visit();

    //Finish rendering
    render->end();

    //Create sprite with drawing result
    CCSprite* res = CCSprite::createWithTexture(render->getSprite()->getTexture());
    //res->setFlipY(true);
    //res->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width/2 + 100/SCALE,
    //                     ORIGIN.y + VISIBLE_SIZE.height/2 - 50/SCALE));
    res->setAnchorPoint(ccp(0,0));
    res->setPosition(ccp(0,0));
    res->setScale(this->getContentSize().width / tablo_size.width);
    this->addChild(res);





//    for(unsigned int x=0; x<width; ++x)
//    {
//        for(unsigned int y=0; y<height; ++y)
//        {
//            CCSprite* border = nullptr;

//            if(y==0 && x==0)
//            {
//                border = _spl->loadSprite("border_angle.png");
//                border->setRotation(270.0f);
//            }
//            else if(y==0 && x==width-1)
//            {
//                border = _spl->loadSprite("border_angle.png");
//                border->setRotation(180.0f);
//            }
//            else if(x==0 && y==height-1)
//            {
//                border = _spl->loadSprite("border_angle.png");
//            }
//            else if(x==width-1 && y==height-1)
//            {
//                border = _spl->loadSprite("border_angle.png");
//                border->setRotation(90.0f);
//            }
//            else if(y==0 ||x==0 || y==0 || x==width-1 || y==height-1)
//            {
//                border = _spl->loadSprite("border_double.png");
//                if(y==0)
//                {
//                    border->setRotation(180.0f);
//                }
//                else if(x==0)
//                {
//                    border->setRotation(270.0f);
//                }
//                else if(x==width-1)
//                {
//                    border->setRotation(90.0f);
//                }

//            }


//            if(x!=height-1)
//            {
//                CCSprite* border_right = _spl->loadSprite("border_normal.png");
//                border_right->setRotation(90.0f);
//                border_right->setPosition(this->getNodePosition(FlowPoint(x, y)));
//            }
//            if(y!=height-1)
//            {
//                CCSprite* border_upper = _spl->loadSprite("border_normal.png");
//                //border_right->setRotation(90.0f);
//                border_upper->setPosition(this->getNodePosition(FlowPoint(x, y)));
//            }

//            if(border!=nullptr)
//                border->setPosition(this->getNodePosition(FlowPoint(x, y)));
//        }
//    }
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

    v[4] = ccc3(0,72,255); //Blue

    v[5] = ccc3(255,142,0); //Orange

    v[6] = ccc3(255,0,233); //Pink

    v[7] = ccc3(0,155,255); //Light blue

    return v;
}

const FlowTable& FlowRenderer::getTable() const
{
    return _table;
}

void FlowRenderer::showActiveCircle(const cocos2d::CCPoint& p, const FlowColor color)
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
