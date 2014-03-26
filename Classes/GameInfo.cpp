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


    _coll_files.push_back("levels/puzzle_9x9_2.ad");
    _coll_files.push_back("levels/puzzle_9x9_1.ad");
    _coll_files.push_back("levels/puzzle_8x8_2.ad");
    _coll_files.push_back("levels/puzzle_7x7_2.ad");
    _coll_files.push_back("levels/puzzle_6x6_2.ad");



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

const std::string GameInfo::doGetPackageName(const std::string game_name)
{
    if(game_name =="mif")
    {
        if(ADInfo::getPlatform() == ADPlatform::iOS)
            return "id657095501";
		else if(ADInfo::getPlatform() == ADPlatform::WindowsPhone)
			return "4a78dd6e-321f-4b21-829c-5b35490671c9"; //Junior
        else
            return "com.x4enjoy.mathisfun";
    }
    if(game_name == "junior")
    {
        if(ADInfo::getPlatform() == ADPlatform::iOS)
            return "id657096722";
		else if(ADInfo::getPlatform() == ADPlatform::WindowsPhone)
			return "4a78dd6e-321f-4b21-829c-5b35490671c9"; //Junior
        else
            return "com.x4enjoy.mathisfunjunior";
    }
    else
    {
        if(GameInfo::getVersion() == Version::Premium)
        {
            if(ADInfo::getPlatform() == ADPlatform::iOS)
                return "id842057927";
            else
                return "com.x4enjoy.joinypremium";

        }
        else
        {
            if(ADInfo::getPlatform() == ADPlatform::iOS)
                return "id816101116";
			else if(ADInfo::getPlatform() == ADPlatform::WindowsPhone)
			{
				return "013aa07d-8dfe-439c-80b9-8f3cae0f426c";
			}
            else
                return "com.x4enjoy.joiny";
        }
    }



}

const std::string GameInfo::getPackageName(std::string game_name)
{
    return doGetPackageName(game_name);
}

Version GameInfo::getVersion()
{
    return _version;
}
