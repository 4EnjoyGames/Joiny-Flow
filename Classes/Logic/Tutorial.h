#ifndef TUTORIAL_H
#define TUTORIAL_H
#include "Flow/FlowGame.h"
#include "Logic/JoinyLevel.h"

class Tutorial
{
public:
    static Tutorial* getInstance( );
    void setInfo(FlowGame* game);
    bool hasTutorial();
    void showTutorial();
    const unsigned int getCurrentTutorialPathId();
    const std::vector<FlowPoint> getCurrentTutorialHintPath();

private:
    static Tutorial* _instance;
    static unsigned int _tutorial_path_id;

    FlowGame* _flow_game;
    const JoinyLevel *_level;

    Tutorial();
};

#endif // TUTORIAL_H
