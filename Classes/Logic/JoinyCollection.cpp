#include "JoinyCollection.h"

JoinyCollection::JoinyCollection()
    : _id(0), _levels(nullptr)
{
}

const unsigned int JoinyCollection::getLevelsNumber() const
{
    return _levels->size();
}

const JoinyLevel* JoinyCollection::getLevel(const JoinyLevelID id) const
{
    const JoinyLevel* res = 0;
    if(id >= 1 && id <= _levels->size())
        res = &_levels->at(id - 1);
    return res;
}

JoinyLevel* JoinyCollection::getLevel(const JoinyLevelID id)
{
    JoinyLevel* res = 0;
    if(id >= 1 && id <= _levels->size())
        res = &_levels->at(id - 1);
    return res;
}

const JoinyCollectionID JoinyCollection::getCollectionID() const
{
    return _id;
}
