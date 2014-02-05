#include "Hints.h"
#include "cocos2d-A.h"
#include <ADLib/ADString.h>
#include <algorithm>    // std::sort

Hints::Hints():_hint_number(5),
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

bool Hints::findSubVector(const UsedPathes& showed_ids,
                          const std::vector<FlowPoint>& vec) const
{
    bool result = false;
    for(unsigned int i=0; i<showed_ids.size(); ++i)
    {
        std::vector<FlowPoint> curr = showed_ids[i];

        //ellements are unique - so it is enought to verify the equalty of first ellement
        if(vec.size()== curr.size() && vec[0]==curr[0])
        {
            result = true;
            break;
        }
    }
    return result;
}
void Hints::newLevel()
{
    _saves.clear();
}

bool Hints::showHint(const JoinyLevel *level, FlowGame *flow_game)
{
    //return - succesfully showed the hint : true
    //       - we havn`t hints to show : false
    bool succesful = false;


    //save current hint
    _level = level;
    _flow_game = flow_game;

    std::vector < std::vector<FlowPoint> > hint_path =
    _level->getPuzzle().getJoinyInfo().getPathes();


    //find which path index we showed
    std::map<JoinyLevelID, UsedPathes>::iterator it =
                _saves.find(_level->getLevelId());


    //1. we showed hints at this level ->
    //do not show same pathes again
    if(it != _saves.end())
    {
        //we showed hints at this level ->
        //do not show same pathes again
        UsedPathes showed_ids = (*it).second;
        int j=0;
        for(j=0; j<hint_path.size(); ++j)
        {
            //find jID in showed_ids
            //std::find(showed_ids.begin(),
            //showed_ids.end(),
            //hint_path[j])!= showed_ids.end()

            if (findSubVector(showed_ids,hint_path[j]))
            {
                //we find hint[j] in showed ides
                //delete this ellement
                hint_path.erase(hint_path.begin()+j);
                --j;

            }
        }

    }

    //2. find which pathes the user do not drowed
    int usr_ps = 0;
    for(usr_ps=0; usr_ps < hint_path.size(); ++usr_ps)
    {
        //if the user HAS DROW this path on the table
        if(_flow_game->hasUserThisPath(hint_path[usr_ps]))
        {
            //delete this ellement
            hint_path.erase(hint_path.begin()+usr_ps);
            --usr_ps;
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

    //have we hints to show
    if(!hint_path.empty())
    {

        //return, that we really show the hint
        succesful = true;


        //decrease hint number, only if we have what hint to show
        useHint();

        std::vector<FlowPoint> hint_path_i = hint_path[middle];
        FlowColor color = _flow_game->getCellColor(hint_path_i[0]);
        CCLog("Hint color: %d", color);

        //4. save the path in used path map
        _saves[_level->getLevelId()].push_back(hint_path_i);

        //5. delete path ellements which intersected with our hint path
        _flow_game->deleteInterferePathes(hint_path_i);

        //6. show the hint
        CCLog("Hints::level id = %d",level->getLevelId());

        _flow_game->showPath(hint_path_i);

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

    return succesful;

}
void Hints::deleteHint(const JoinyLevel *level, FlowGame *flow_game)
{
    //find which path index we showed
    std::map<JoinyLevelID, UsedPathes>::iterator it =
                _saves.find(level->getLevelId());

    if(it!=_saves.end())
    {
        //get all hint, showed on this level
        UsedPathes showed_ids = (*it).second;

        for(unsigned int i=0; i<showed_ids.size(); ++i)
        {
            flow_game->deleteHintPath(showed_ids[i]);
        }

        //delete saves of hints and level id from map
        _saves.erase(it);
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
