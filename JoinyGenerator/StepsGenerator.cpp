#include "StepsGenerator.h"
#include <sstream>
#include <string>
#include <fstream>
using namespace std;


bool areNeighboors(const FlowPoint& a, const FlowPoint& b)
{
    if(a.x() == b.x() && (a.y()+1 == b.y() || b.y()+1 == a.y()))
        return true;
    if(a.y() == b.y() && (a.x()+1 == b.x() || b.x()+1 == a.x()))
        return true;
    return false;
}



bool operator<(const FlowTask& a, const FlowTask& b)
{
//    bool result = true;
//    if(a.size()>b.size())
//        result = false;
//    else if(a.size()<b.size())
//        result = false;
//    else
//    {
//        for(unsigned int i=0; i<a.size(); ++i)
//        {
//            FlowStartEnd a_curr = a[i];
//            FlowStartEnd b_curr = b[i];

//            if(a_curr < b_curr)
//            {
//                result = true;
//            }
//            else
//            {
//                result = false;
//                break;
//            }
//        }
//    }
//    return result;
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
    Score precision = info.getGold()/20;
    if(info.getBronze() + precision <= info.getSilver() &&
            info.getSilver() <= info.getGold())
    {
        return true;
    }
    else
        return false;
}

bool isGooTask(const FlowTask& task)
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
void GenerateLevels(const unsigned int tablo_size,
                const unsigned int level_number,
                const unsigned int min_colors,
                const unsigned int max_colors)
{

    srand(time(0));
    unsigned int joiny_size = tablo_size;
    unsigned int N = level_number;
    unsigned int generated = 0;
    unsigned int good = 0;
    unsigned int max = max_colors;
    unsigned int min = min_colors;
    std::set<JoinyPuzzle> good_tasks;


    while(good_tasks.size() < N)
    {

        FlowTask t = generate(joiny_size,joiny_size);
        std::sort(t.begin(), t.end());

        if(isGooTask(t))
        {
            unsigned int colors = max;

            if(t.size() == 2)
                colors = 2;
            else if(t.size() == 3)
                colors = 3;
            else
            {
                //TODO: correct
                colors = min + (rand() % (unsigned int)(max - min + 1));
            }

            JoinyTask task = flowToJoiny(t, colors, true);
            //std::sort(task.begin(), task.end());


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

                //std::sort(task.begin(), task.end());
                JoinyPuzzle puzzle(task, info);

                good_tasks.insert(puzzle);
                ++good;

            }
        }
        generated++;
        if(generated % 10 == 0)
            tabulate(generated, good, good_tasks.size());
    }


    //write in file all ell from the set
    unsigned int num = 0;
    for(std::set<JoinyPuzzle>::iterator it = good_tasks.begin();
        it != good_tasks.end();
        ++it)
    {
        JoinyPuzzle puzzle = *it;

        std::stringstream fname;
        fname << joiny_size << "x" << joiny_size << "_" << num << ".ad";
        ++num;

        std::ofstream file;
        file.open(fname.str().c_str(), ios::out | ios::binary);
        OutputBinaryStream os(file, BinaryStream::MaxProtocolVersion);

        os << puzzle;
    }

    tabulate(generated, good, good_tasks.size());

}
void SaveCollection(std::string coll_save_name)
{
    //get info about main settings for collection
    std::ifstream infile("collection.txt");
    std::string line;

    std::getline(infile, line);
    std::string tablo_size = line;
    int tablo_size_i = std::stoi(tablo_size);

    std::getline(infile, line);
    std::string level_num = line;
    int level_num_i = std::stoi(level_num);

    std::getline(infile,line);
    std::string collection_color = line;

    std::vector<std::string> level_files_name;

    for(unsigned int i=0; i<level_num_i; ++i)
    {
        std::getline(infile, line);
        level_files_name.push_back(line);
    }


    //open level files and save info to std::vector<JoinyPuzzle>
    std::vector<JoinyPuzzle> good_tasks;
    for(unsigned int i=0; i<level_files_name.size(); ++i)
    {
        JoinyPuzzle puzzle;
        std::ifstream file;
        file.open(level_files_name[i], ios::in | ios::binary);
        InputBinaryStream is(file);

        is >> puzzle;

        good_tasks.push_back(puzzle);

    }

    //TODO: recolor puzzle
    //2 rules:
    //two neighbour level can not have the same Color Palete
    //mix Color Paletes


    //TODO: then write all other important info to collection binary file
    //create collection
    std::stringstream fname;
    fname << coll_save_name;
    std::ofstream file;
    file.open(fname.str().c_str(), ios::out | ios::binary);
    OutputBinaryStream os(file, BinaryStream::MaxProtocolVersion);
    os << good_tasks;
}
void RecolorJoiny(JoinyPuzzle& puzzle)
{

}
