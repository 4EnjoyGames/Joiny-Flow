#include "Hints.h"
#include "cocos2d-A.h"
#include <ADLib/ADString.h>
#include <algorithm>    // std::sort

Hints::Hints():_hint_number(3),
    _level(0),
    _flow_game(0)
{

}
Hints* Hints::_instance = nullptr;

Hints* Hints::getInstance( )
{
    if (_instance == nullptr)
        _instance = new Hints();
    return _instance;
}
bool Hints::less_vectors (const std::vector<FlowPoint> &a,
                          const std::vector<FlowPoint> &b)
{
   return a.size() < b.size();
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


    //find which path index we showed
    std::map<JoinyLevelID, UsedPathes>::iterator it =
                _saves.find(_level->getLevelId());


    //1. find which pathes the user do not drowed
    for(unsigned int i=0; i<hint_path.size(); ++i)
    {
        //if the user HAS DROW this path on the table
        if(_flow_game->hasUserThisPath(hint_path[i]))
        {
            //delete this ellement
            hint_path.erase(hint_path.begin()+i);
        }
    }

    //2. we showed hints at this level ->
    //do not show same pathes again
    if(it != _saves.end())
    {
        //we showed hints at this level ->
        //do not show same pathes again
        UsedPathes showed_ids = (*it).second;
        for(unsigned int j=0; j<hint_path.size(); ++j)
        {
            //find jID in showed_ids
            if (std::find(showed_ids.begin(),
                          showed_ids.end(),
                          hint_path[j])!=
                    showed_ids.end())
            {
                //we find hint[j] in showed ides
                //delete this ellement
                hint_path.erase(hint_path.begin()+j);

            }
        }

    }

    //3.0
    //firstly - lower ellements

    std::sort(hint_path.begin(),hint_path.end(),
            [](const std::vector<FlowPoint> &a, const std::vector<FlowPoint> &b) -> bool
        {
             return a.size() < b.size();
        });

    //3.1 from formed set choose pathes with middle size
    unsigned int middle = hint_path.size()/2;
    if(middle!=0 && hint_path.size()%2==0)
        middle-=1;
    std::vector<FlowPoint> hint_path_i = hint_path[middle];
    FlowColor color = _flow_game->getCellColor(hint_path_i[0]);


    //4. save the path in used path map
    _saves[_level->getLevelId()].push_back(hint_path_i);


    //5. show the hint
    CCLog("Hints::level id = %d",level->getLevelId());

    for(unsigned int i=1; i<hint_path_i.size(); ++i)
    {
        CCLog("Hints::(%d, %d) -> (%d, %d)",hint_path_i[i-1].x(),
                hint_path_i[i-1].y(),
                hint_path_i[i].x(),hint_path_i[i].y());

        _flow_game->connectHintPoints(hint_path_i[i-1],
                hint_path_i[i],
                color);
    }

}
void Hints::deleteHint()
{
    if(_level!=0)
    {
        std::vector < std::vector<FlowPoint> > hint_path =
                _level->getPuzzle().getJoinyInfo().getPathes();
        unsigned int hint_size = hint_path.size();


        //find which path index we must show
        std::map<JoinyLevelID, PathID>::iterator itor =
                _order.find(_level->getLevelId());

        PathID curr_path_id = 0;
        //we never show hint for this level
        if(itor ==_order.end())
        {
            CCLog("Hint delete Error! We delete hint, which was not showed");
        }
        else
        {
            curr_path_id = (*itor).second;

            //if now we show the first path - the previous time we showed
            //1. or the last path
            //2. or the first path (if there is only one  hint path)

            if(curr_path_id == 0)
            {
                //gave from the end
                if(hint_size==1)
                    curr_path_id = 0;
                else
                    curr_path_id = hint_size - 1;
            }
            else
                --curr_path_id;
        }

        std::vector<FlowPoint> curr_path = hint_path[curr_path_id];

        CCLog("level id = %d",_level->getLevelId());
        CCLog("Path id = %d",curr_path_id);

        for(unsigned int i=1; i<curr_path.size(); ++i)
        {
            CCLog("(%d, %d) -> (%d, %d)",curr_path[i-1].x(),curr_path[i-1].y(),
                    curr_path[i].x(),curr_path[i].y());
            _flow_game->disconnectHintPoints(curr_path[i-1], curr_path[i]);
        }
    }
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
