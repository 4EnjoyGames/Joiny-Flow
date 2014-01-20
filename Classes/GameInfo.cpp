#include "GameInfo.h"
#include "Localization/CCLocalizedString.h"
#include <sstream>

GameInfo* GameInfo::instance = nullptr;

GameInfo::GameInfo()
{
    setInfo();
}

void GameInfo::setInfo()
{
    _coll_files.push_back("puzzle_5x5.ad");
    _coll_files.push_back("puzzle_6x6.ad");

    _flow_points.push_back(FlowPoint(5,5));
    _flow_points.push_back(FlowPoint(6,6));

    _colors.push_back(ccc3(248,105,159));
    _colors.push_back(ccc3(100,243,248));
    _colors.push_back(ccc3(64,255,68));

    _level_number = 150;

    _close_color = ccc3(170,170,170);

    _title_color = ccc3(11,216,224);

    _negative_color = ccc3(255,0,0);
    _positive_color = ccc3(0,255,0);
}

GameInfo* GameInfo::getInstance( )
{
      if (instance == nullptr)
        instance = new GameInfo();
     return instance;
}
const std::vector<FlowPoint>& GameInfo::getFlowPoints()
{
    return _flow_points;
}

const std::vector<std::string>& GameInfo::getCollectionFiles()
{
    return _coll_files;
}
const std::vector<ccColor3B>& GameInfo::getCollectionColors()
{
    return _colors;
}
unsigned int GameInfo::getCollectionNumber()
{
    return _coll_files.size();
}
const std::vector<std::string>& GameInfo::getCollectionNames()
{
    if(_coll_names.size()==0)
    {
        std::string base = "Col";

        unsigned int coll_num = getCollectionNumber()+1;
        for (unsigned int i=1; i<coll_num; ++i)
        {
            std::string int_converted = std::to_string(i);
            std::string coll_name = base + int_converted;
            std::string curr_coll = CCLocalizedString(coll_name.c_str());
            _coll_names.push_back(curr_coll);
        }
    }
    return _coll_names;
}

const ccColor3B& GameInfo::getCloseColor()
{
    return _close_color;
}
const ccColor3B &GameInfo::getTitleColor()
{
    return _title_color;
}
const ccColor3B& GameInfo::getNegativeColor()
{
    return _negative_color;
}

const ccColor3B& GameInfo::getPositiveColor()
{
    return _positive_color;
}
