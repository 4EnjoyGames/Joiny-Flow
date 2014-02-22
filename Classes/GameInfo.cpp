#include "GameInfo.h"
#include "Localization/CCLocalizedString.h"
#include <sstream>
#include <ADLib/ADString.h>
GameInfo* GameInfo::instance = nullptr;

GameInfo::GameInfo()
{
    setInfo();
}

void GameInfo::setInfo()
{
    _coll_files.push_back("levels/puzzle_5x5_1.ad");
    _coll_files.push_back("levels/puzzle_5x5_2.ad");
    _coll_files.push_back("levels/puzzle_6x6_1.ad");
    _coll_files.push_back("levels/puzzle_7x7_1.ad");
    _coll_files.push_back("levels/puzzle_8x8_1.ad");


    //_coll_files.push_back("levels/puzzle_9x9_1.ad");
    //_coll_files.push_back("levels/puzzle_10x10_1.ad");
    _coll_files.push_back("levels/puzzle_6x6_2.ad");
    _coll_files.push_back("levels/puzzle_7x7_2.ad");
    _coll_files.push_back("levels/puzzle_8x8_2.ad");

    _close_color = ccc3(170,170,170);

    _title_color = ccc3(11,216,224);

    _negative_color = ccc3(255,30,99);

    _positive_color = ccc3(117,253,51);
    //_positive_color = ccc3(0,255,0);

    _gray_color = ccc3(134,203,226);
}

GameInfo* GameInfo::getInstance( )
{
      if (instance == nullptr)
        instance = new GameInfo();
     return instance;
}
const std::vector<std::string>& GameInfo::getCollectionFiles()
{
    return _coll_files;
}

unsigned int GameInfo::getCollectionNumber()
{
    return _coll_files.size();
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
 const ccColor3B& GameInfo::getGrayColor()
 {
     return _gray_color;
 }
#include <ADLib/Device/ADInfo.h>

std::string doGetPackageName()
{
    if(ADInfo::getPlatform() == ADPlatform::iOS)
        return "id657095501";
    else
        return "com.x4enjoy.joiny";
}

const std::string& GameInfo::getPackageName()
{
    static std::string package_name = doGetPackageName();
    return package_name;
}
