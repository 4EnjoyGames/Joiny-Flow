#ifndef JOINYPAIR_H
#define JOINYPAIR_H
#include "Generator/FGenerator.h"
#include "Binary/Binary.h"
class JoinyPair
{
public:
    JoinyPair(const FlowStartEnd& points=FlowStartEnd(), unsigned int color=0)
        : _points(points), _color(color)
    {

    }

    const FlowStartEnd& getPoints() const
    {
        return _points;
    }
    const unsigned int getColor() const
    {
        return _color;
    }
    void setColor(unsigned int color)
    {
        _color = color;
    }

private:
    FlowStartEnd _points;
    unsigned int _color;
};

typedef std::vector<JoinyPair> JoinyTask;
typedef unsigned int Score;
class JoinyInfo
{
public:
    JoinyInfo(const Score bronze=0, const Score silver=0, const Score gold=0
              )
        : _bronze_score(bronze),
          _silver_score(silver),
          _gold_score(gold)
    {

    }

    const Score getBronze() const
    {
        return _bronze_score;
    }

    const Score getSilver() const
    {
        return _silver_score;
    }

    const Score getGold() const
    {
        return _gold_score;
    }

    const std::vector<int> getPathes() const
    {
        return _hint_pathes;
    }

    void setHintPathes(const std::vector<int> pathes)
    {
        _hint_pathes = pathes;
    }

private:
    Score _bronze_score;
    Score _silver_score;
    Score _gold_score;

    std::vector<int> _hint_pathes;
};

struct JoinyPuzzle
{
    JoinyPuzzle(const JoinyTask& task, const  JoinyInfo& info)
        : _task(task), _info(info)
    {}
    JoinyPuzzle()
        : _info(0,0,0)
    {}
    const JoinyTask& getJoinyTask() const
    {
        return _task;
    }
    JoinyTask& getJoinyTask()
    {
        return _task;
    }

    JoinyTask _task;
    JoinyInfo _info;

};

inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const JoinyPair& task)
{
    os << uint32_t(task.getColor()) << task.getPoints();
    return os;
}

inline InputBinaryStream& operator>>(InputBinaryStream& is, JoinyPair& task)
{
    FlowStartEnd s;
    uint32_t colors=0;

    is >> colors >> s;
    JoinyPair p(s, colors);
    task = p;
    return is;
}

inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const JoinyInfo& info)
{
    os << uint32_t(info.getBronze()) << uint32_t(info.getSilver()) << uint32_t(info.getGold());
    return os;
}

inline InputBinaryStream& operator>>(InputBinaryStream& is, JoinyInfo& info)
{
    uint32_t bronze = 0;
    uint32_t silver = 0;
    uint32_t gold = 0;

    is >> bronze >> silver >> gold;
    JoinyInfo f(bronze, silver, gold);
    info = f;

    return is;
}


inline OutputBinaryStream& operator<<(OutputBinaryStream& os, const JoinyPuzzle& puzzle)
{
    os << puzzle._info << puzzle._task;
    return os;
}


inline InputBinaryStream& operator>>(InputBinaryStream& is, JoinyPuzzle& puzzle)
{
    is >> puzzle._info >> puzzle._task;
    return is;
}


#endif // JOINYPAIR_H
