#include "Tutorial.h"
#include "Logic/RW.h"

Tutorial* Tutorial::_instance = nullptr;
unsigned int Tutorial::_tutorial_path_id = 0;

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

bool Tutorial::hasTutorial()
{
    std::vector < std::vector<FlowPoint> > hint_path =
    _level->getPuzzle().getJoinyInfo().getPathes();

    return (_tutorial_path_id < hint_path.size());
}

void Tutorial::showTutorial()
{
    if (_level!=nullptr && _flow_game!=nullptr)
    {
        std::vector < std::vector<FlowPoint> > hint_path =
        _level->getPuzzle().getJoinyInfo().getPathes();

        std::vector<FlowPoint> curr_path = hint_path[_tutorial_path_id];
        FlowColor color = _flow_game->getCellColor(curr_path[0]);


        for(unsigned int j=1; j<curr_path.size(); ++j)
        {
            //CCLog("Tutorial Hints::(%d, %d) -> (%d, %d)",
            //      curr_path[j-1].x(),curr_path[j-1].y(),
            //       curr_path[j].x(),curr_path[j].y());

            _flow_game->connectHintPoints(curr_path[j-1],
                    curr_path[j],
                    color);
        }

        ++_tutorial_path_id;
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
