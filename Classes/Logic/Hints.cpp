#include "Hints.h"
#include "cocos2d-A.h"
#include <ADLib/ADString.h>

Hints::Hints():_hint_number(3)
{

}
Hints* Hints::_instance = nullptr;

Hints* Hints::getInstance( )
{
    if (_instance == nullptr)
        _instance = new Hints();
    return _instance;
}
void Hints::showHint(const JoinyLevel *level, FlowGame *flow_game)
{
    //decrease hint humber
    useHint();

    //save current hint
    _level = level;
    _flow_game = flow_game;

    std::vector < std::vector<FlowPoint> > hint_path =
            _level->getPuzzle().getJoinyInfo().getPathes();
    unsigned int hint_size = hint_path.size();


    //find which path index we must show
    std::map<JoinyLevelID, PathID>::iterator itor =
            _order.find(level->getLevelId());

    PathID curr_path_id = 0;
    //we never show hint for this level
    if(itor ==_order.end())
    {
        PathID path_id = 0;

        if(hint_size >1)
            path_id = 1;

        _order[level->getLevelId()] = path_id;
    }
    else
    {
        curr_path_id = (*itor).second;

        if(curr_path_id < (hint_size-1))
            (*itor).second = curr_path_id+1;
        else
            (*itor).second = 0;
    }

    std::vector<FlowPoint> curr_path = hint_path[curr_path_id];
    FlowColor color = 3;

    CCLog("level id = %d",level->getLevelId());
    CCLog("Path id = %d",curr_path_id);

    for(unsigned int i=1; i<curr_path.size(); ++i)
    {
        CCLog("(%d, %d) -> (%d, %d)",curr_path[i-1].x(),curr_path[i-1].y(),
                curr_path[i].x(),curr_path[i].y());
        _flow_game->connectHintPoints(curr_path[i-1], curr_path[i], color);
    }
}
void Hints::deleteHint()
{

}

const unsigned int Hints::getHintNumber()
{
    return _hint_number;
}

void Hints::useHint()
{
    --_hint_number;
}
bool Hints::hasHint()
{
    return (_hint_number!=0);
}
void Hints::setHintNumber(const unsigned int num)
{
    _hint_number = num;
}
