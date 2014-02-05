#ifndef HINTS_H
#define HINTS_H
#include "Flow/FlowGame.h"
#include "Logic/JoinyLevel.h"


class Hints
{
public:
    static Hints* getInstance( );

    void newLevel();

    //return we show the hint succesfully or not
    bool showHint(const JoinyLevel* level, FlowGame* flow_game);
    void deleteHint(const JoinyLevel *level, FlowGame *flow_game);

    const unsigned int getHintNumber();
    void useHint();
    bool hasHint();


    // hint pathes, which were used
    typedef std::vector < std::vector<FlowPoint> > UsedPathes;
    bool findSubVector(const UsedPathes &showed_ids,
                       const std::vector<FlowPoint>& vec) const;

private:
    Hints();
    Hints(Hints const&);              // don't Implement
    void operator=(Hints const&); // don't implement



    static Hints* _instance;

    const JoinyLevel *_level;
    FlowGame *_flow_game;

    unsigned int _hint_number;

    //save for each level id the hint id, which we must show next time
    typedef unsigned int PathID;
    std::map<JoinyLevelID, PathID> _order;


    //save all pathes, which we hinted with their LevelID
    std::map< JoinyLevelID, UsedPathes > _saves;

    friend class LevelManager;
    void setHintNumber(const unsigned int num);
};

#endif // HINTS_H
