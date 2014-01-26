#include "Joiny.h"

#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <stack>
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;

bool FLAGS_color = false;


class NumberLink {
public:
    // The type of the number written in a cell
    typedef char CellNumber;
    // The type of the ID number of a cell
    typedef short CellKey;
    // The type of a coordinate represented by (y * width + x)
    typedef short CellPosition;
    // The type of a distance such as width, height, x and y
    typedef int Distance;
    // The size of the NumberLink task
    Distance width_;
    Distance height_;

    typedef map<unsigned int, unsigned int> Scores;
    Scores _scores;

    NumberLink(const Distance width, const Distance height):
        width_(width), height_(height), size_((CellKey)width * height),
        cell_x_(size_), cell_y_(size_), table_(size_),
        keys_(size_), mates_(size_), start_(size_ + 1),
        connected_x_(size_), connected_y_(size_), memo_(size_) {}

    void Initialize() {
        // Initialize mates
        for (CellKey cell_key = 0; cell_key < size_; cell_key++) {
            mates_[cell_key] = cell_key;
        }

        // Generates references: (x, y) <=> CellKey
        Distance x = 0, y = 0;
        CellKey cell_key = 0;
        while (true) {
            CellPosition position = (CellPosition)y * width_ + x;
            cell_x_[cell_key] = x;
            cell_y_[cell_key] = y;
            keys_[position] = cell_key;
            cell_key++;
            if (cell_key == size_) break;
            do {
                x--;
                y++;
                if (x < 0) {
                    x = y;
                    y = 0;
                }
            } while (x < 0 || width_ <= x || y < 0 || height_ <= y);
        }

        // Pre-compute CellKey to look back for every cell
        for (CellKey i = 0; i < size_; i++) {
            Distance x = cell_x_[i], y = cell_y_[i];
            start_[i] = 0 < y ? GetCellKey(x, y - 1) :
                                (0 < x ? GetCellKey(x - 1, y) : 0);
        }
        // For a guard
        start_[size_] = size_;
    }

    // Returns the reference of the number written in the cell (x,y).
    CellNumber& Cell(const Distance x, const Distance y)
    { return table_[GetCellKey(x, y)]; }
    // Returns the key of the special order for the coordinate (x,y).
    CellKey GetCellKey(const Distance x, const Distance y) const
    { return keys_[(CellPosition)y * width_ + x]; }

    double Solve(const CellKey cell_key = 0) {
        if (cell_key == 0) solved_ = false;
        // See the newly fixed cells
        if (0 < cell_key) {
            for (CellKey hidden = start_[cell_key - 1];
                 hidden < start_[cell_key]; hidden++) {
                if (table_[hidden] == 0) {
                    // Return if the empty cell has an end
                    if (mates_[hidden] != -1 && mates_[hidden] != hidden) return 0.0;
                } else {
                    // Return if the numbered cell has no line
                    if (mates_[hidden] == hidden) return 0.0;
                }
            }
        }

        // If all the cells are filled
        if (cell_key == size_) {
            //if (!solved_) {
            Print();
            //    solved_ = true;
            //}
            return 1.0;
        }
        else
        {
            Print();
        }
        //Print();
        // Connect successive cells if this sequence of mates has not been seen
        const vector<CellKey> mate_tuple(mates_.begin() + start_[cell_key],
                                         mates_.begin() + cell_key);
        const Hash mate_hash = GetHash(mate_tuple);
        if (!memo_[cell_key].count(mate_hash)) {
            memo_[cell_key][mate_hash] = Connect(cell_key);
        }
        return memo_[cell_key][mate_hash];
    }

    double Connect(const CellKey cell_key) {
        double solution_count = 0.0;
        Distance x = cell_x_[cell_key], y = cell_y_[cell_key];
        CellKey left_cell_key = -1, up_cell_key = -1;
        if (0 < x) left_cell_key = GetCellKey(x - 1, y);
        if (0 < y) up_cell_key = GetCellKey(x, y - 1);
        size_t revert_point = mate_stack_.size();
        // Connect the cell with nothing
        solution_count += Solve(cell_key + 1);
        // Connect the cell with the upper cell
        if (0 <= up_cell_key) {
            if (UniteMates(cell_key, up_cell_key)) {
                connected_y_[cell_key] = true;
                solution_count += Solve(cell_key + 1);
                connected_y_[cell_key] = false;
            }
            RevertMates(revert_point);
        }
        // Connect the cell with the left cell
        if (0 <= left_cell_key) {
            if (UniteMates(cell_key, left_cell_key)) {
                connected_x_[cell_key] = true;
                solution_count += Solve(cell_key + 1);
                // Connect the cell with the upper and the left cells
                if (0 <= up_cell_key) {
                    if (UniteMates(cell_key, up_cell_key)) {
                        connected_y_[cell_key] = true;
                        solution_count += Solve(cell_key + 1);
                        connected_y_[cell_key] = false;
                    }
                }
                connected_x_[cell_key] = false;
            }
            RevertMates(revert_point);
        }
        return solution_count;
    }

    std::vector<bool> _traversed;

    unsigned int traverse(const unsigned int x, const unsigned int y, unsigned int path_lenth)
    {
        _traversed[GetCellKey(x, y)] = true;
        if(path_lenth > 0 && table_[GetCellKey(x, y)])
        {
            unsigned int path = path_lenth;
            unsigned int score = path*path*100 + 200;
            //cout << "IScore: " << score << endl;
            return score;
        }


        //Next go to left?
        if(x > 0 && connected_x_[GetCellKey(x, y)] && !_traversed[GetCellKey(x-1, y)])
            return traverse(x-1, y, path_lenth+1);

        //Next go to top?
        if(y > 0 && connected_y_[GetCellKey(x, y)] && !_traversed[GetCellKey(x, y-1)])
            return traverse(x, y-1, path_lenth+1);

        //Next go to right?
        if(x+1 < width_ && connected_x_[GetCellKey(x+1, y)] && !_traversed[GetCellKey(x+1, y)])
            return traverse(x+1, y, path_lenth+1);

        //Next go bottom?
        if(y+1 < height_ && connected_y_[GetCellKey(x, y+1)] && !_traversed[GetCellKey(x, y+1)])
            return traverse(x, y+1, path_lenth+1);

        return 0;
        //assert(false);
    }

    void DoPrint()
    {
        for (Distance y = 0; y <= height_; y++)
        {
            for (Distance x = 0; x < width_; x++)
            {
                cout << "+";
                if((y % height_ == 0))
                    cout << "---";
                else
                {
                    //Чи пішли ми з клітинки вгору
                    if(connected_y_[GetCellKey(x, y)])
                        cout << " # ";
                    else
                        cout <<  "   ";
                }
            }
            cout << "+" << endl;
            if (height_ <= y)
                break;

            for (Distance x = 0; x < width_; x++)
            {
                if(x)
                {
                    if(connected_x_[GetCellKey(x, y)])
                        cout << "#";
                    else
                        cout << " ";
                }
                else
                {
                    cout << "|";
                }

                if (table_[GetCellKey(x, y)])
                {
                    cout << "00" << int(table_[GetCellKey(x, y)]);
                }
                else
                {
                    //Чи прийшли ми зліва в клітинку (x y)
                    if(connected_x_[GetCellKey(x, y)])
                        cout << "#";
                    else
                        cout << " ";


                    //Перевірка чи проходить пряма через клітинку (x y)
                    if(mates_[GetCellKey(x, y)] == GetCellKey(x, y))
                        cout << " ";
                    else
                        cout << "#";

                    //Чи пішли ми враво із цієї клітинки
                    if(x + 1 < width_ && connected_x_[GetCellKey(x + 1, y)])
                        cout << "#";
                    else
                        cout << " ";

                }
            }
            cout << "|" << endl;

        }
    }

    void Print() {


        //connected_x_[GetCellKey(x, y)]; true якщо в клітинку (x, y) шлях іде зліва
        //connected_y_[GetCellKey(x, y)]; true якщо в клітинку (x, y) шлях іде згори

        _traversed = std::vector<bool>(height_ * width_, false);
        unsigned int score = 0;

        bool stop = false;
        for (Distance y = 0; y < height_ && !stop; y++)
            for (Distance x = 0; x < width_ && !stop; x++)
            {
                if (table_[GetCellKey(x, y)] && !_traversed[GetCellKey(x, y)])
                {
                    unsigned int sc = traverse(x,y,0);
                    if(sc == 0)
                    {
                        score = 0;
                        stop = true;
                    }
                    else
                    {
                        score += sc;
                    }
                }
            }



        if(score > 0)
        {
            if(_scores.find(score) == _scores.end())
                _scores[score]=1;
            else
                _scores[score]++;
        }

        if(score > 0 && false)
        {


            cout << "Score: " << score << endl;

        }

    }

private:
    // Hash key to identify a sequence of CellKeys
    typedef pair<long long, long long> Hash;
    // The number of cells on the board
    CellKey size_;
    // History of modification
    stack< pair<CellKey, CellKey> > mate_stack_;
    // Map from a CellKey to a coordinate
    vector<Distance> cell_x_, cell_y_;
    // Store the numbers in the cells
    vector<CellNumber> table_;
    vector<CellKey> keys_, mates_, start_;
    // Description of links on the board
    vector<bool> connected_x_, connected_y_;
    // Hash table to identify a sequence of CellKeys
    vector< map<Hash, double> > memo_;
    // Flag to know whether the problem has been solved
    bool solved_;

    // Get a hash key based on the XorShift algorithm
    Hash GetHash(const vector<CellKey> &cell_keys) {
        unsigned int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
        for (int i = 0; i < (int)cell_keys.size(); i++) {
            unsigned int t = (x ^ (x << 11)); x = y; y = z; z = w;
            w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)) + (unsigned int)cell_keys[i];
        }
        Hash h;
        h.first = ((unsigned long long)x << 32) | y;
        h.second = ((unsigned long long)z << 32) | w;
        return h;
    }

    // Change the table of mates in adding the history
    int ChangeMates(const CellKey cell_key, const CellKey cell_value) {
        int last_stack_size = mate_stack_.size();
        CellKey last_value = mates_[cell_key];
        if (last_value != cell_value) {
            mate_stack_.push(make_pair(cell_key, last_value));
            mates_[cell_key] = cell_value;
        }
        return last_stack_size;
    }

    // Revert the table of mates using the history
    void RevertMates(const size_t stack_size) {
        for (; stack_size < mate_stack_.size(); mate_stack_.pop())
            mates_[mate_stack_.top().first] = mate_stack_.top().second;
    }

    // Connects cell_key1 and cell_key2 by a line. Returns false if it cannot
    // connect the cells because of constraints. The table of mates must be
    // reverted even if the cells cannot be connect correctly.
    bool UniteMates(const CellKey cell_key1, const CellKey cell_key2) {
        CellKey end1 = mates_[cell_key1], end2 = mates_[cell_key2];
        // Cannot connect any branch
        if (end1 == -1 || end2 == -1) return false;
        // Avoid making a cycle
        if (cell_key1 == end2 && cell_key2 == end1) return false;
        // Change states of mates to connect cell_key1 and cell_key2
        ChangeMates(cell_key1, -1);
        ChangeMates(cell_key2, -1);
        ChangeMates(end1, end2);
        ChangeMates(end2, end1);
        // Check three constraints:
        //   1. cell_key1 must not be a branch if cell_key1 is numbered,
        //   2. cell_key2 must not be a branch if cell_key2 is numbered,
        //   3. Different numbers cannot be connected.
        if (mates_[cell_key1] == -1 && 0 < table_[cell_key1]) return false;
        if (mates_[cell_key2] == -1 && 0 < table_[cell_key2]) return false;
        if (0 < table_[end1] && 0 < table_[end2] &&
                table_[end1] != table_[end2]) return false;
        return true;
    }
};

JoinyTask flowToJoinyStaightforward(const FlowTask& task)
{
    JoinyTask res;
    for(unsigned int i=0; i<task.size(); ++i)
    {
        //unsigned int color = rand() % colors;
        res.push_back(JoinyPair(task[i], i));
    }
    return res;
}

const unsigned int getColor(std::map<unsigned int,unsigned int>& mymap)
{
    // 5 -1
    //6 -0
    //7 -0
    //8 -1
    //int min_rand = mymap.begin()->second;
    std::map<unsigned int,unsigned int>::iterator min_it = mymap.begin();

    for (std::map<unsigned int,unsigned int>::iterator it = mymap.begin();
         it != mymap.end();
         ++it)
    {
        if(min_it->second > it->second)
            min_it = it;

    }

    min_it->second = ++ (min_it->second);

    return min_it->first;
}
JoinyTask relocorJoiny(const JoinyTask& joiny_task,
                       const Palete& bad_palete)
{
    //select new palete
    Palete palete = getColorSchemeExceptBadPalete(bad_palete);


    JoinyTask result;
    for(unsigned int i=0; i<joiny_task.size(); ++i)
    {
        unsigned int old_color = joiny_task[i].getColor();
        unsigned int new_color = old_color;

        for(unsigned int j=0; j<bad_palete.size(); ++j)
        {
            if (old_color == bad_palete[j])
            {
                new_color = palete[j];
                break;
            }
        }
        result.push_back(JoinyPair(joiny_task[i].getPoints(), new_color));
    }
    return result;
}

JoinyTask flowToJoiny(const FlowTask& task_2, const unsigned int colors)
{
    assert(colors <= task_2.size());
//    std::map<unsigned int,unsigned int> mymap;
//    for (unsigned int j=0; j<colors; ++j)
//    {
//        mymap.insert ( std::pair<unsigned int,unsigned int>(j,0) );
//    }
    Palete palete = getColorScheme(colors);
    _curr_palete = palete;

    FlowTask task = task_2;
    std::random_shuffle(task.begin(), task.end());

    JoinyTask res;
    for(unsigned int i=0; i<colors; ++i)
    {
        res.push_back(JoinyPair(task[i], palete[i]));
    }

    for(unsigned int i=colors; i<task.size(); ++i)
    {
        //unsigned int color = rand() % colors;
        unsigned int color = rand() % palete.size();
        res.push_back(JoinyPair(task[i], palete[color]));
    }
    return res;
}

unsigned int clamp(const unsigned int num, const unsigned int clamper)
{
    return (num / clamper) * clamper;
}

JoinyInfo solveJoiny(const JoinyTask& task, const unsigned int width, const unsigned int height)
{
    NumberLink nl((int)width, (int)height);
    nl.Initialize();

    for(unsigned int i=0; i<task.size(); ++i)
    {
        JoinyPair pair = task[i];
        FlowPoint start = pair.getPoints()[0];
        FlowPoint end = pair.getPoints()[1];

        nl.Cell(start.x(), start.y()) = pair.getColor()+1;
        nl.Cell(end.x(), end.y()) = pair.getColor()+1;


    }

    unsigned int solution_count = nl.Solve();
    //cout << "Solutions found: " << solution_count << endl;

    if(solution_count > 0)
    {

        NumberLink::Scores& sc = nl._scores;

        typedef NumberLink::Scores::iterator It;

        It first = sc.begin();

        unsigned int min = first->first;
        unsigned int max = first->second;

        double sum = 0;
        unsigned int n = 0;
        for(It it = sc.begin(); it != sc.end(); ++it)
        {
            unsigned int score = it->first;
            if(score < min)
                min = score;
            if(score > max)
                max = score;

            sum += score * it->second;
            n += it->second;
        }

//        cout << "Min: " << min << endl;
//        cout << "Max: " << max << endl;
//        cout << "Average: " << (sum / n) << endl;
//        cout << "N: " << n << endl;

        unsigned int average = sum/n;
        return JoinyInfo(clamp(average, 500), clamp((average + max)/2, 500), clamp(max, 500));

//        cout << "Bronze: " << clamp(average, 500) << endl;
//        cout << "Silver: " << clamp((average + max)/2, 500) << endl;
//        cout << "Gold: " << clamp(max, 500) << endl;

//        nl.DoPrint();


    }
    return JoinyInfo(0,0,0);
}
//typedef unsigned int Color;

static AllPaletes _paletes;

void palete2(unsigned int c1, unsigned int c2)
{
    Palete p;
    p.push_back(c1);
    p.push_back(c2);
    _paletes[2].push_back(p);
}

void palete3(unsigned int c1, unsigned int c2, unsigned int c3)
{
    Palete p;
    p.push_back(c1);
    p.push_back(c2);
    p.push_back(c3);
    _paletes[3].push_back(p);
}
void palete4(unsigned int c1, unsigned int c2,
             unsigned int c3, unsigned int c4)
{
    Palete p;
    p.push_back(c1);
    p.push_back(c2);
    p.push_back(c3);
    p.push_back(c4);
    _paletes[4].push_back(p);
}
void JoinyColorSchemeInit()
{
    palete2(0,1);
    palete2(2,6);

    palete3(2,5,6);
    palete3(0,2,1);

    palete4(0,1,3,4);
    palete4(2,7,5,3);
}
const Palete& getColorSchemeExceptBadPalete(const Palete& bad_palete)
{
    static bool init = false;

    if(!init)
    {
        JoinyColorSchemeInit();
        init = true;
    }


    auto& paletes = _paletes[bad_palete.size()];
    for(unsigned int i=0; i<paletes.size(); ++i)
    {
        if(paletes[i]!=bad_palete)
            return paletes[i];
    }

}

const Palete &getColorScheme(unsigned int color_num)
{
    static bool init = false;

    if(!init)
    {
        JoinyColorSchemeInit();
        init = true;
    }


    auto& paletes = _paletes[color_num];
    return paletes[rand() % paletes.size()];
}
const Palete& getCurrPalete()
{
    return _curr_palete;
}
bool operator==(const Palete& p1, const Palete& p2)
{
    bool res = true;

    if (p1.size()!=p2.size())
        res = false;
    else
    {
        for(unsigned int i=0; i<p1.size(); ++i)
        {
            if(p1[i]!=p2[i])
            {
                res = false;
                break;
            }
        }
    }

    return res;
}
