#include "GameInfo.h"

GameInfo* GameInfo::instance = nullptr;

GameInfo::GameInfo()
{
    setInfo();
}

void GameInfo::setInfo()
{
    _coll_names.push_back("puzzle_5x5.ad");
    _coll_names.push_back("puzzle_6x6.ad");

    _flow_points.push_back(FlowPoint(5,5));
    _flow_points.push_back(FlowPoint(6,6));

    _colors.push_back(ccc3(248,105,159));
    _colors.push_back(ccc3(100,243,248));
    _colors.push_back(ccc3(64,255,68));

    _level_number = 150;

    _close_color = ccc3(138,141,142);

    _title_color = ccc3(11,216,224);
}

GameInfo* GameInfo::getInstance( )
{
      if (instance == nullptr)
        instance = new GameInfo();
     return instance;
}
const std::vector<FlowPoint>& GameInfo::getFlowPoints()
{
    return _flow_points;
}

const std::vector<std::string>& GameInfo::getCollectionNames()
{
    return _coll_names;
}
const std::vector<ccColor3B>& GameInfo::getCollectionColors()
{
    return _colors;
}
const ccColor3B& GameInfo::getCloseColor()
{
    return _close_color;
}
const ccColor3B &GameInfo::getTitleColor()
{
    return _title_color;
}
