#include "JoinyCollection.h"

JoinyCollection::JoinyCollection()
    : _id(0), _levels(nullptr), _coll_name("no_name"),
      _coll_color(ccc3(250,250,250)), _open(false), _min_star_number(3)
{
}

const unsigned int JoinyCollection::getLevelsNumber() const
{
    return _levels->size();
}

const unsigned int JoinyCollection::getCompletedLevelsNumber() const
{
    unsigned int result = 0;
    unsigned int l_num = getLevelsNumber()+1;
    for(unsigned int i=1; i<l_num; ++i)
    {
        const JoinyLevel* level = getLevel(i);
        if(level->isCompleted())
            result+=1;
    }
    return result;
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
const StarsNumber JoinyCollection::getStarSum() const
{
    //never delete this row
    _min_star_number = 3;
    StarsNumber result = 0;

    unsigned int l_num = getLevelsNumber()+1;
    for(unsigned int i=1; i<l_num; ++i)
    {
        StarsNumber stars = getLevel(i)->getHighStar();

        if(stars < _min_star_number)
            _min_star_number = stars;
        result+=stars;
    }
    return result;
}

const StarsNumber JoinyCollection::getMinStarsNumber() const
{
    getStarSum();
    return _min_star_number;
}
const JoinyCollectionID JoinyCollection::getCollectionID() const
{
    return _id;
}
const std::string JoinyCollection::getCollectionName() const
{
    return _coll_name;
}
const ccColor3B &JoinyCollection::getCollectionColor() const
{
    return _coll_color;
}
void JoinyCollection::setCollectionName(const std::string name)
{
    _coll_name = name;
}
void JoinyCollection::setCollectionColor(const ccColor3B color)
{
    _coll_color = color;
}
bool JoinyCollection::isOpenCollection() const
{
    return _open;
}

void JoinyCollection::openCollection()
{
    _open = true;
}
