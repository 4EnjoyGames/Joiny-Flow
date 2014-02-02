#include "Hints.h"
#include "cocos2d-A.h"
#include <ADLib/ADString.h>

Hints::Hints(const JoinyLevel *level, FlowGame *flow_game):
    _flow_game(flow_game),
    _level(level)
{
    showHint();
}

void Hints::showHint()
{
    CCLog("Hint clicked");
    std::vector < std::vector<FlowPoint> > hint_path =
            _level->getPuzzle().getJoinyInfo().getPathes();
    unsigned int index=0;
    std::vector<FlowPoint> curr_path = hint_path[index];
    FlowColor color = 3;

    for(unsigned int i=1; i<curr_path.size(); ++i)
    {
        CCLog("(%d, %d) -> (%d, %d)",curr_path[i-1].x(),curr_path[i-1].y(),
              curr_path[i].x(),curr_path[i].y());
        _flow_game->connectHintPoints(curr_path[i-1], curr_path[i], color);
    }


//    std::vector<FlowPoint> points;
//    FlowColor color = 3;

//    points.push_back(FlowPoint(0,2));
//    points.push_back(FlowPoint(1,2));
//    points.push_back(FlowPoint(2,2));
//    points.push_back(FlowPoint(2,1));
//    points.push_back(FlowPoint(2,0));
//    points.push_back(FlowPoint(1,0));



//    for(unsigned int i=1; i<points.size(); ++i)
//    {
//        _flow_game->connectHintPoints(points[i-1], points[i], color);
//    }

}
