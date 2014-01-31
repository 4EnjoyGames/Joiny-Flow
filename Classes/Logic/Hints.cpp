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

    std::vector<FlowPointState> states;
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
//            if((CellPosition)y * width_ >= keys_.size())
//                return keys_[keys_.size()-1];
//            else
//                return keys_[(CellPosition)y * width_ + x];

            if(hint_path.size()>0)
            {
                if(hint_path[y*width + x] == curr_hint_num)
                {
                    std::string log = "x = "
                            + AD_to_string(x)
                            +' '
                            + "y = "
                            + AD_to_string(y);
                    CCLog(log.c_str());


                    FlowPointState curr_state;
                    curr_state.setNodeType(FlowPointState::Hint);
                    curr_state.setCordinates(FlowPoint(x,y));

                    states.push_back(curr_state);

                }
            }
        }
    }

    for(unsigned int i=0; i<states.size()-1; ++i)
    {
        FlowPointState curr = states[i];
        FlowPointState next = states[i+1];

        states[i].setHintNextCordinate(next.getCordinates());

        states[i+1].setHintPreviousCordinate(curr.getCordinates());

    }
    _flow_game->connectPoints(states[0],states[1]);
}
