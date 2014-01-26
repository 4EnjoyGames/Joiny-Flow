#include <iostream>
#include "FGenerator.h"
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>
#include "Binary.h"
#include "Joiny.h"
#include <fstream>
#include <sstream>
using namespace std;



bool areNeighboors(const FlowPoint& a, const FlowPoint& b)
{
    if(a.x() == b.x() && (a.y()+1 == b.y() || b.y()+1 == a.y()))
        return true;
    if(a.y() == b.y() && (a.x()+1 == b.x() || b.x()+1 == a.x()))
        return true;
    return false;
}

bool operator<(const FlowPoint& a, const FlowPoint& b)
{
    return (a.x() < b.x() || (a.x() == b.x() && a.y() < b.y()));
}

bool operator<(const FlowTask& a, const FlowTask& b)
{
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

bool operator<(const JoinyPair& a, const JoinyPair& b)
{
    if(a.getColor() < b.getColor())
        return true;
    else if(a.getColor() == b.getColor())
        return a.getPoints() < b.getPoints();
    return false;
}

bool isGoodJoiny(const JoinyTask& task, const JoinyInfo& info)
{
    //TODO: create constrains
    Score precision = info.getGold()/10;
    if(info.getBronze() + precision <= info.getSilver() &&
            info.getSilver() <= info.getGold())
    {
        return true;
    }
    else
        return false;
    //return info.getBronze() != info.getSilver() && info.getSilver() != info.getGold();
    //return true;
}

bool isGooTask(const FlowTask& task, unsigned int N)
{
    //return task.size() <= (N-1) && N > 0;
    for(FlowTask::const_iterator it = task.begin(); it!=task.end(); ++it)
    {
        const FlowStartEnd& s = *it;
        if(s.size() != 2)
            return false;
        if(areNeighboors(s[0], s[1]))
            return false;
    }
    return true;
}
void tabulate(unsigned int generated, unsigned int good, unsigned int unique)
{
    cout << "Generated: " << generated << ", \t Good: " << good << ", \t Unique: " << unique << endl;
}

bool operator<(const JoinyInfo& a, const JoinyInfo& b)
{
    if(a.getBronze() < b.getBronze())
        return true;
    else if(a.getBronze() == b.getBronze())
    {
        if(a.getSilver() < b.getSilver())
            return true;
        else if(a.getSilver() == b.getSilver())
        {
            return a.getGold() < b.getGold();
        }
    }
    return false;
}

bool operator==(const JoinyInfo& a, const JoinyInfo& b)
{
    return a.getBronze() == b.getBronze() && a.getSilver() == b.getSilver() && a.getGold() == b.getGold();

}

bool operator<(const JoinyPuzzle& a, const JoinyPuzzle& b)
{
    if(a._info < b._info)
        return true;
    else if(a._info == b._info)
    {
        return a._task < b._task;
    }
    return false;
}
JoinyInfo mergeInfo(const JoinyInfo a, const JoinyInfo b)
{
    assert(a.getSilver() >= a.getBronze());
    unsigned int step = a.getSilver() - a.getBronze();
    if(step >= 1000)
        step /= 2;
    unsigned int gold = std::max(a.getGold(), b.getGold());
    JoinyInfo res(gold - 2*step,
                  gold - step,
                  gold);
    return res;
}

int main()
{
    srand(time(0));
    unsigned int joiny_size = 5;

    unsigned int N = 100;
    unsigned int generated = 0;
    unsigned int good = 0;


    typedef std::map<unsigned int, unsigned int> ColorsMap;
    ColorsMap colors;
    for(unsigned int i=0; i<20; ++i)
    {
        colors[i] = 0;
    }
    unsigned int max = 4;
    unsigned int min = 3;

    generated = 0;
    good = 0;

    std::set<JoinyPuzzle> good_tasks;

    Palete past;
    Palete curr;
    while(good_tasks.size() < N)
    {

        FlowTask t = generate(joiny_size,joiny_size);
        if(isGooTask(t, N))
        {
            unsigned int colors = max;
            //random
            if(t.size() == 2)
                colors = 2;
            else if(t.size() == 3)
                colors = 3;
            else
            {
                colors = min + (rand() % (unsigned int)(max - min + 1));
            }

            JoinyTask task = flowToJoiny(t, colors);


            JoinyInfo info_old = solveJoiny(task,
                                            joiny_size,
                                            joiny_size);

            JoinyTask task_origin = flowToJoinyStaightforward(t);
            JoinyInfo info_origin = solveJoiny(task_origin,
                                               joiny_size,
                                               joiny_size);

            JoinyInfo info = mergeInfo(info_old, info_origin);

            if(info.getGold() > 100000)
            {
                std::cout << "Bug!!" << endl;
            }
            else if(isGoodJoiny(task, info))
            {
                past = curr;
                curr = getCurrPalete();



                if(curr == past)
                {
                    //choose new palete except curr palete
                    //recolor JoinyTask
                    task = relocorJoiny(task,curr);
                }

                std::sort(task.begin(), task.end());
                JoinyPuzzle puzzle(task, info);
                good_tasks.insert(puzzle);
                good++;


            }
        }
        generated++;
        if(generated % 10 == 0)
            tabulate(generated, good, good_tasks.size());
    }

    tabulate(generated, good, good_tasks.size());

    for(ColorsMap::iterator it = colors.begin(); it!= colors.end(); ++it)
    {
        if(it->second != 0)
            cout << "Colors " << it->first << ": " << it->second << endl;
    }

    std::vector<JoinyPuzzle> res(good_tasks.begin(), good_tasks.end());
    std::random_shuffle(res.begin(), res.end());

    std::stringstream fname;
    fname << "puzzle_" << joiny_size << "x" << joiny_size << "_1.ad";

    std::ofstream file;
    file.open(fname.str().c_str(), ios::out | ios::binary);
    OutputBinaryStream os(file, BinaryStream::MaxProtocolVersion);

    os << res;

    //tabulate(generated, good, good_tasks.size());

//    for(ColorsMap::iterator it = colors.begin(); it!= colors.end(); ++it)
//    {
//        if(it->second != 0)
//            cout << "Colors " << it->first << ": " << it->second << endl;
//    }

//    std::vector<JoinyPuzzle> res(good_tasks.begin(), good_tasks.end());
//    std::random_shuffle(res.begin(), res.end());

//    std::stringstream fname;
//    fname << "puzzle_" << width << "x" << height << ".ad";

//    std::ofstream file(fname.str().c_str());
//    OutputBinaryStream os(file, BinaryStream::MaxProtocolVersion);

//    os << res;

    return 0;
}

