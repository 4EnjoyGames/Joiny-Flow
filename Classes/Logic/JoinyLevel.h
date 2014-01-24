#ifndef JOINYLEVEL_H
#define JOINYLEVEL_H
#include "Flow/FlowPoint.h"
#include "Flow/JoinyPair.h"
class JoinyCollection;
class LevelManager;
typedef unsigned int JoinyLevelID;
typedef unsigned int StarsNumber;
class JoinyLevel
{
public:


    const JoinyPuzzle& getPuzzle() const;
    const JoinyLevelID getLevelId() const;
    const StarsNumber getStarsNumber(const Score score) const;

    const FlowPoint getBoardSize() const;
    const JoinyCollection* getCollection() const;
    const Score getHighScore() const;
    const StarsNumber getHighStar() const;
    bool  isCompleted() const;
private:
    JoinyLevel();
    friend class LevelManager;
    friend class JoinyCollection;

    JoinyPuzzle _puzzle;
    JoinyLevelID _level_id;
    JoinyCollection* _parent;
    FlowPoint _size;
    Score _highscore;
};

#endif // JOINYLEVEL_H
