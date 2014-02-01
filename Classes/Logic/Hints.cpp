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
    std::vector<uint32_t> hint_path = _level->getPuzzle().getJoinyInfo().getPathes();
    uint32_t curr_hint_num = 1;


    unsigned int height = _level->getBoardSize().x();
    unsigned int width =  _level->getBoardSize().y();

    std::vector<FlowPoint> points;
    points.push_back(FlowPoint(0,2));
    points.push_back(FlowPoint(1,2));
    points.push_back(FlowPoint(2,2));
    points.push_back(FlowPoint(2,1));
    points.push_back(FlowPoint(2,0));
    points.push_back(FlowPoint(1,0));

    FlowColor color = 3;

    for(unsigned int i=1; i<points.size(); ++i)
    {
        _flow_game->connectHintPoints(points[i-1], points[i], color);
    }

    //std::vector<FlowPointState> states;
//    for (unsigned int y = 0; y < height; y++)
//    {
//        for (unsigned int x = 0; x < width; x++)
//        {
////            if((CellPosition)y * width_ >= keys_.size())
////                return keys_[keys_.size()-1];
////            else
////                return keys_[(CellPosition)y * width_ + x];

//            if(hint_path.size()>0)
//            {
//                if(hint_path[y*width + x] == curr_hint_num)
//                {
//                    std::string log = "x = "
//                            + AD_to_string(x)
//                            +' '
//                            + "y = "
//                            + AD_to_string(y);
//                    CCLog(log.c_str());


//                    FlowPointState curr_state = _flow_game->getFlowPointState(FlowPoint(x,y));
//                    curr_state.setHint(true);
//                    curr_state.setCordinates(FlowPoint(x,y));

//                    states.push_back(curr_state);

//                }
//            }
//        }
//    }

//    for(unsigned int i=0; i<states.size()-1; ++i)
//    {
//        FlowPointState curr = states[i];
//        FlowPointState next = states[i+1];

//        states[i].setHintNextCordinate(next.getCordinates());

//        states[i+1].setHintPreviousCordinate(curr.getCordinates());

//    }
//    //_flow_gam
//    _flow_game->connectHintPoints(states[0],states[1]);
}
