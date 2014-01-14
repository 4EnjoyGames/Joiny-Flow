#ifndef JOINYCOLLECTION_H
#define JOINYCOLLECTION_H
#include "cocos2d-A.h"
#include <vector>
#include "JoinyLevel.h"
#include <memory>

class LevelManager;
typedef unsigned int JoinyCollectionID;
class Color
{
public:
    Color(){}
    Color(int r, int b, int g):_red(r), _blue(b), _green(g){}
    const int& red() {return _red;}
    const int& blue() {return _blue;}
    const int& green() {return _green;}
private:
    int _red;
    int _blue;
    int _green;
};

class JoinyCollection
{
public:
    const unsigned int getLevelsNumber() const;
    const JoinyLevel* getLevel(const JoinyLevelID id) const;
    const JoinyCollectionID getCollectionID() const;
    const std::string getCollectionName() const;
    const ccColor3B& getCollectionColor() const;

    void setCollectionName(const std::string name);
    void setCollectionColor(const ccColor3B color);

    bool isOpenCollection() const;
    void openCollection();
private:
    JoinyLevel* getLevel(const JoinyLevelID id);

    JoinyCollection();
    typedef std::vector<JoinyLevel> LevelsVector;
    std::shared_ptr<LevelsVector> _levels;
    JoinyCollectionID _id;
    std::string _coll_name;
    //Color _coll_color;
    ccColor3B _coll_color;
    bool _open;
    friend class LevelManager;
};

#endif // JOINYCOLLECTION_H
