#ifndef HINTS_H
#define HINTS_H
#include "Flow/FlowGame.h"
#include "Logic/JoinyLevel.h"


class Hints
{
public:
    //static Hints* getInstance( );
    Hints(const JoinyLevel *level, FlowGame *flow_game);
    Hints();

    //return we show the hint succesfully or not
    bool showHint();
    void deleteHint();

    const unsigned int getHintNumber();
    void useHint();
    void decreaseHintNumber(const unsigned int num=1);
    void increaseHintNumber(const unsigned int num=1);
    bool hasHint();

    //tutorial
    bool hasTutorial();
    void showTutorial();


    // hint pathes, which were used
    typedef std::vector < std::vector<FlowPoint> > UsedPathes;


private:
    //Hints();
    //Hints(Hints const&);              // don't Implement
    //void operator=(Hints const&); // don't implement



    //static Hints* _instance;
    //for tutorial
    static unsigned int _tutorial_path_id;

    const JoinyLevel *_level;
    FlowGame *_flow_game;

    int _hint_number;

    //save all pathes, which we hinted with their LevelID
    UsedPathes _saves;
    //std::map< JoinyLevelID, UsedPathes > _saves;

    friend class LevelManager;
    void setHintNumber(const unsigned int num);

    bool findSubVector(const UsedPathes &showed_ids,
                       const std::vector<FlowPoint>& vec) const;
};

#endif // HINTS_H
