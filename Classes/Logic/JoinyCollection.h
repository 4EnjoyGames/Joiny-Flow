#ifndef JOINYCOLLECTION_H
#define JOINYCOLLECTION_H
#include <vector>
#include "JoinyLevel.h"
#include <memory>
class LevelManager;
typedef unsigned int JoinyCollectionID;
class JoinyCollection
{
public:
    const unsigned int getLevelsNumber() const;
    const JoinyLevel* getLevel(const JoinyLevelID id) const;
    const JoinyCollectionID getCollectionID() const;
private:
    JoinyLevel* getLevel(const JoinyLevelID id);

    JoinyCollection();
    typedef std::vector<JoinyLevel> LevelsVector;
    std::shared_ptr<LevelsVector> _levels;
    JoinyCollectionID _id;
    friend class LevelManager;
};

#endif // JOINYCOLLECTION_H
