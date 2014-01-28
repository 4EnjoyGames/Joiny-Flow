#include "JoinyLevel.h"

JoinyLevel::JoinyLevel()
    : _level_id(0), _size(FlowPoint::UNDEFINED), _parent(nullptr), _puzzle(),
      _highscore(0)
{
}

const Score JoinyLevel::getHighScore() const
{
    return _highscore;
}
const StarsNumber JoinyLevel::getHighStar() const
{
    return getStarsNumber(getHighScore());
}
bool JoinyLevel::isCompleted() const
{
    if (getStarsNumber(getHighScore()) == 0)
        return false;
    else
        return true;
}
const JoinyPuzzle& JoinyLevel::getPuzzle() const
{
    return _puzzle;
}

const JoinyLevelID JoinyLevel::getLevelId() const
{
    return _level_id;
}
const StarsNumber JoinyLevel::getStarsNumber(const Score score) const
{
    unsigned int stars = 0;
    if(score < _puzzle._info.getBronze())
        stars = 0;
    else
    {
        stars = 1;
        if(score >= _puzzle._info.getGold())
            stars = 3;
        else if(score >= _puzzle._info.getSilver())
            stars = 2;

    }
    return stars;
}

const FlowPoint JoinyLevel::getBoardSize() const
{
    return _size;
}
const JoinyCollection* JoinyLevel::getCollection() const
{
    return _parent;
}
