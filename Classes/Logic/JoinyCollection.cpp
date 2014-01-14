#include "JoinyCollection.h"

JoinyCollection::JoinyCollection()
    : _id(0), _levels(nullptr), _coll_name("no_name"),
      _coll_color(ccc3(250,250,250))
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
const std::string JoinyCollection::getCollectionName() const
{
    return _coll_name;
}
//ccColor3B JoinyCollection::getCollectionColor() const
//{
//    return ccc3(_coll_color.red(), _coll_color.green(), _coll_color.blue());

//}

const ccColor3B &JoinyCollection::getCollectionColor() const
{
    return _coll_color;
}
void JoinyCollection::setCollectionName(const std::string name)
{
    _coll_name = name;
}

//void JoinyCollection::setCollectionColor(const Color color)
//{
//    _coll_color = color;
//}
void JoinyCollection::setCollectionColor(const ccColor3B color)
{
    _coll_color = color;
}
