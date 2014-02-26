#include "Tutorial.h"
#include "Logic/RW.h"

Tutorial* Tutorial::_instance = nullptr;
unsigned int Tutorial::_tutorial_path_id = 0;
unsigned int Tutorial::_hint_ell_index = 1;

Tutorial::Tutorial():_flow_game(nullptr),_level(nullptr)
{
    _level = RW::getLevelManager().getLevel(1,1);
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
    if (_level!=nullptr && _flow_game!=nullptr)
    {
        std::vector < std::vector<FlowPoint> > hint_path =
        _level->getPuzzle().getJoinyInfo().getPathes();

        std::vector<FlowPoint> curr_path = hint_path[_tutorial_path_id];
        FlowColor color = _flow_game->getCellColor(curr_path[0]);


        if (_hint_ell_index < curr_path.size())
        {
            _flow_game->connectHintPoints(curr_path[_hint_ell_index-1],
                                          curr_path[_hint_ell_index],
                                          color);
            ++_hint_ell_index;

            //show the next tutorial
            // call hint in 1 seconds
            CCCallFunc *callAction = CCCallFunc::create(this,
                                              callfunc_selector(
                                                Tutorial::showTutorial));

            CCSequence* action = CCSequence::create(
                        CCDelayTime::create(0.4f),
                                              callAction,
                                              NULL);
            CCDirector::sharedDirector()->getRunningScene()->runAction(action);

        }
        else
        {
            ++_tutorial_path_id;
            _hint_ell_index = 1;
        }
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
