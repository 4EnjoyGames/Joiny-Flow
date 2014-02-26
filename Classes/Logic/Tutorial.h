#ifndef TUTORIAL_H
#define TUTORIAL_H
#include "Flow/FlowGame.h"
#include "Logic/JoinyLevel.h"
#include "cocos2d-A.h"
#include "JoinyCollection.h"

class Tutorial: public CCObject
{
public:
    static Tutorial* getInstance( );
    void setInfo(FlowGame* game);
    void setLevel(JoinyCollectionID coll_id,
                  JoinyLevelID level_id);
    bool hasTutorial();
    void showTutorial();
    const unsigned int getCurrentTutorialPathId();
    const std::vector<FlowPoint> getCurrentTutorialHintPath();
    void deleteTutorialPath();

private:
    static Tutorial* _instance;
    static unsigned int _tutorial_path_id;
    static unsigned int _hint_ell_index;

    FlowGame* _flow_game;
    const JoinyLevel *_level;


    Tutorial();
};

#endif // TUTORIAL_H
