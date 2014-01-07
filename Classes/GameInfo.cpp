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

    _level_number = 150;
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
