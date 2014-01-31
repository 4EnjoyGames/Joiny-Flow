#ifndef HINTS_H
#define HINTS_H
#include "Flow/FlowGame.h"
#include "Logic/JoinyLevel.h"


class Hints
{
public:
    Hints(const JoinyLevel* level, FlowGame* flow_game);

private:
    FlowGame* _flow_game;
    const JoinyLevel* _level;

    void showHint();
};

#endif // HINTS_H
