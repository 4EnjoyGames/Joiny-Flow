#include "Tutorial.h"
#include "Logic/RW.h"

Tutorial* Tutorial::_instance = nullptr;
unsigned int Tutorial::_tutorial_path_id = 0;
unsigned int Tutorial::_hint_ell_index = 1;

Tutorial::Tutorial():_flow_game(nullptr),_level(nullptr),
    _is_active(false)
{
    _level = RW::getLevelManager().getLevel(1,1);
}
void Tutorial::setIsActive(bool is_active)
{
    _is_active = is_active;
}
bool Tutorial::isActive()
{
    return _is_active;
}

Tutorial* Tutorial::getInstance( )
{
      if (_instance == nullptr)
        _instance = new Tutorial();
     return _instance;
}
void Tutorial::setInfo(FlowGame* game)
{
    _flow_game = game;
    _tutorial_path_id = 0;
    _hint_ell_index = 1;
}
void Tutorial::setLevel(JoinyCollectionID coll_id,
              JoinyLevelID level_id)
{
    _level = RW::getLevelManager().getLevel(coll_id,level_id);
}

bool Tutorial::hasTutorial()
{
    std::vector < std::vector<FlowPoint> > hint_path =
    _level->getPuzzle().getJoinyInfo().getPathes();

    return (_tutorial_path_id < hint_path.size());
}

//void Tutorial::showTutorial()
//{
//    if (_level!=nullptr && _flow_game!=nullptr)
//    {
//        std::vector < std::vector<FlowPoint> > hint_path =
//        _level->getPuzzle().getJoinyInfo().getPathes();

//        std::vector<FlowPoint> curr_path = hint_path[_tutorial_path_id];
//        FlowColor color = _flow_game->getCellColor(curr_path[0]);


//        for(unsigned int j=1; j<curr_path.size(); ++j)
//        {
//            _flow_game->connectHintPoints(curr_path[j-1],
//                    curr_path[j],
//                    color);
//        }

//        ++_tutorial_path_id;
//    }

//}

void Tutorial::showTutorial()
{
    bool plan_action = false;
    if (_is_active && _level!=nullptr && _flow_game!=nullptr)
    {
        std::vector < std::vector<FlowPoint> > hint_path =
        _level->getPuzzle().getJoinyInfo().getPathes();

        if(_tutorial_path_id >= hint_path.size())
        {
            //Find path which user has ignored
            for(int i = 0; i < hint_path.size(); ++i)
            {
                if(!_flow_game->hasUserThisPath(hint_path[i]))
                {
                    _tutorial_path_id = i;
                    break;
                }
            }

            if(_tutorial_path_id >= hint_path.size())
                return;
        }

        std::vector<FlowPoint> curr_path = hint_path[_tutorial_path_id];
        FlowColor color = _flow_game->getCellColor(curr_path[0]);

        if(_flow_game->hasUserThisPath(curr_path))
        {
            _flow_game->deleteHintPath(curr_path);
            ++_tutorial_path_id;
            _hint_ell_index = 1;
            plan_action = true;
        }
        else
        {


            if (_hint_ell_index < curr_path.size())
            {
                bool not_connected_found = false;
                do
                {
                    if(!_flow_game->areFlowPointsConnected(curr_path[_hint_ell_index-1],
                                                          curr_path[_hint_ell_index]))
                    {
                        not_connected_found = true;
                    }
                    _flow_game->connectHintPoints(curr_path[_hint_ell_index-1],
                                                  curr_path[_hint_ell_index],
                                                  color);
                    ++_hint_ell_index;

                } while(!not_connected_found &&
                        _hint_ell_index < curr_path.size());

                plan_action = true;

            }
            else
            {
                _flow_game->deleteHintPath(curr_path);
                _hint_ell_index = 1;
                plan_action = true;
            }
        }
    }

    if(plan_action)
    {
        //show the next tutorial
        // call hint in 1 seconds
        CCCallFunc *callAction = CCCallFunc::create(this,
                                          callfunc_selector(
                                            Tutorial::showTutorial));

        CCSequence* action = CCSequence::create(
                    CCDelayTime::create(0.2f),
                                          callAction,
                                          NULL);
        CCDirector::sharedDirector()->getRunningScene()->runAction(action);
    }

}

const unsigned int Tutorial::getCurrentTutorialPathId()
{
    if(_tutorial_path_id==0)
        return _tutorial_path_id;
    return _tutorial_path_id-1;
}
const std::vector<FlowPoint> Tutorial::getCurrentTutorialHintPath()
{
    std::vector < std::vector<FlowPoint> > hint_path =
    _level->getPuzzle().getJoinyInfo().getPathes();

    if(_tutorial_path_id==0)
        return hint_path[_tutorial_path_id];
    return hint_path[_tutorial_path_id-1];
}
void Tutorial::deleteTutorialPath()
{
    _tutorial_path_id = 0;
    _flow_game=nullptr;
}
