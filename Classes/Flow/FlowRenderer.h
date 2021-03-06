#ifndef FLOWRENDERER_H
#define FLOWRENDERER_H
#include "cocos2d.h"
#include "FlowTable.h"
#include "FlowRendererNode.h"
#include "Core/GraphicsManager.h"
#include <ADLib/ADSignals.h>
class FlowRenderer : public cocos2d::CCNode, public HasSlots
{
public:
    FlowRenderer(const FlowTable&);
    void updateCell(const FlowPoint cordinates,
                    const FlowPointState& new_state);
    const FlowTable& getTable() const;

    const cocos2d::ccColor3B& getRenderColor(const FlowColor);
    const cocos2d::CCSize getNodeSize();
    const cocos2d::CCPoint getNodePosition(const FlowPoint& pos);

    cocos2d::CCSprite* createSprite(const char* name);
    cocos2d::CCSprite* createHintSprite(const char* name);

    void showActiveCircle(const cocos2d::CCPoint& p, const FlowColor color);
    void hideActiveCircle();

    void drawTable(float scale, cocos2d::CCSize tablo_size);

    static FlowRenderer* create(const FlowTable&);
private:
    void createBackground();
    void createNodes();
    void initNodes();


    typedef std::vector<FlowRendererNode> Row;
    typedef std::vector<Row> Table;

    Table _nodes_renderers;

    typedef std::vector<cocos2d::ccColor3B> ColorVec;
    static ColorVec _colors;
    static ColorVec getDefaultColors();
    FlowTable _table;

    cocos2d::CCSprite* _active_circle;

    SpritesLoader _spl_layer0;

    //for hints
    SpritesLoader _spl_layer05;

    //need to show flow upper
    SpritesLoader _spl_layer1;


    void doDrawTable();
    float _table_scale;
    cocos2d::CCSize _table_size;





    float _textures_size;
};

#endif // FLOWRENDERER_H
