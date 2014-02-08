#include "Hints.h"
#include "cocos2d-A.h"
#include <ADLib/ADString.h>
#include "Logic/RW.h"
#include <algorithm>    // std::sort

Hints::Hints(const JoinyLevel *level, FlowGame *flow_game):
    _hint_number(RW::getLevelManager().getHintNumber()),
    _level(level),
    _flow_game(flow_game)
{
}
Hints::Hints():
    _hint_number(RW::getLevelManager().getHintNumber())
{
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

bool Hints::showHint()
{
    //return - succesfully showed the hint : true
    //       - we havn`t hints to show : false
    bool succesful = false;

    std::vector < std::vector<FlowPoint> > hint_path =
    _level->getPuzzle().getJoinyInfo().getPathes();


    UsedPathes showed_ids = _saves;//(*it).second;
    int j=0;
    for(j=0; j<hint_path.size(); ++j)
    {
        if (findSubVector(showed_ids,hint_path[j]))
        {
            //we find hint[j] in showed ides
            //delete this ellement
            hint_path.erase(hint_path.begin()+j);
            --j;

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
        //CCLog("Hint color: %d", color);

        //4. save the path in used path map
        //_saves[_level->getLevelId()].push_back(hint_path_i);
        _saves.push_back(hint_path_i);

        //5. delete path ellements which intersected with our hint path
        _flow_game->deleteInterferePathes(hint_path_i);

        //6. show the hint
        CCLog("Hints::level id = %d",_level->getLevelId());

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
void Hints::deleteHint()
{
    for(unsigned int i=0; i<_saves.size(); ++i)
    {
        _flow_game->deleteHintPath(_saves[i]);
    }
    _saves.clear();
}

const unsigned int Hints::getHintNumber()
{
    _hint_number = RW::getLevelManager().getHintNumber();
    return _hint_number;
}

void Hints::useHint()
{
    --_hint_number;
    RW::getLevelManager().decreaseHintNumber();
}
void Hints::decreaseHintNumber(const unsigned int num)
{
    if(_hint_number-num >= 0)
    {
        _hint_number-=num;
        RW::getLevelManager().decreaseHintNumber(num);
    }
}

void Hints::increaseHintNumber(const unsigned int num)
{
    _hint_number+=num;
    RW::getLevelManager().increaseHintNumber(num);
}

bool Hints::hasHint()
{
    _hint_number = RW::getLevelManager().getHintNumber();
    return (_hint_number!=0);
}

