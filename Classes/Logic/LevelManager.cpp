#include "LevelManager.h"
#include "Core/MusicSettings.h"
#include "Core/FileUtils.h"
#include <fstream>
#include "Core/Screen.h"
#include "Flow/FlowTable.h"
#include "Flow/FlowRenderer.h"
#include "ADLib/ADString.h"
#include "Logic/Hints.h"

LevelManager::LevelManager()
    : _hints_number(10),
      _full_version(0),
      _full_version_path(FileUtils::getStorageFilePath("full_version.ad")),
      _save_path(FileUtils::getStorageFilePath("save.ad")),
      _settings_path(FileUtils::getStorageFilePath("settings.ad")),
      _hints_path(FileUtils::getStorageFilePath("hints.ad"))
{
}

void LevelManager::onInit()
{
    loadLevelsInfo();
    loadSettings();
}

void LevelManager::onDestroy()
{
    saveGame();
    saveSettings();
}

void LevelManager::onPause()
{
    saveGame();
    saveSettings();
}

void LevelManager::onRun()
{
    loadGame();
}
const JoinyCollection* LevelManager::getCollection(const JoinyCollectionID id) const
{
    CollectionMap::const_iterator it = _collections.find(id);
    if(it == _collections.end())
        return nullptr;

    return it->second.get();
}

const JoinyLevel* LevelManager::getNextLevel(const JoinyLevel* level) const
{
    const JoinyCollection* col = level->getCollection();
    JoinyLevelID id = level->getLevelId()+1;
    if(id <= col->getLevelsNumber())
    {
        return col->getLevel(id);
    }
    return nullptr;
}

const JoinyLevel* LevelManager::getPreviousLevel(const JoinyLevel* level) const
{
    const JoinyCollection* col = level->getCollection();
    JoinyLevelID id = level->getLevelId()-1;
    if(level->getLevelId()>1)
    {
        return col->getLevel(id);
    }
    return nullptr;
}

void LevelManager::onLevelFinished(const JoinyLevel* level, const Score score)
{
    if(level->getHighScore() < score)
    {
        JoinyCollectionID col_id = level->getCollection()->getCollectionID();
        CollectionPtr col = _collections[col_id];
        JoinyLevel* editable_level = col->getLevel(level->getLevelId());
        editable_level->_highscore = score;
        saveGame();
    }
}

void LevelManager::loadLevelsInfo()
{
    GameInfo* info = GameInfo::getInstance();

    std::vector<std::string> coll_files = info->getCollectionFiles();

    for(unsigned int j=0; j<coll_files.size(); ++j)
    {
        std::string file = coll_files[j];
        std::stringstream ss;
        if(FileUtils::openPackageFile(file, ss))
        {
            InputBinaryStream is(ss);

            uint32_t width=0;
            is >> width;

            uint32_t height=0;
            is >> height;

            uint32_t level_num = 0;
            is >> level_num;

            uint32_t r = 0;
            uint32_t g = 0;
            uint32_t b = 0;
            is >> r;
            is >> g;
            is >> b;


            std::vector<JoinyPuzzle> inp;
            is >> inp;

            CollectionPtr col(new JoinyCollection);
            col->_id = j+1;
            col->_levels = std::shared_ptr<JoinyCollection::LevelsVector>(
                        new JoinyCollection::LevelsVector(level_num,
                                                          JoinyLevel()));
            std::string coll_name = AD_to_string(width)
                    + "x"
                    + AD_to_string(height);
            col->setCollectionName(coll_name);
            col->setCollectionColor(ccc3(r,g,b));

            for(unsigned int i=0; i<inp.size(); ++i)
            {
                JoinyLevel& l = col->_levels->at(i);
                l._level_id = i+1;
                l._parent = col.get();
                l._puzzle = inp[i];
                l._size = FlowPoint(width,height);
            }

            _collections[col->_id] = col;
        }
    }
}
void LevelManager::saveSettings()
{
    std::stringstream ss(std::ios::out | std::ios::binary);
    OutputBinaryStream os(ss,BinaryStream::MaxProtocolVersion);

    bool sound = MusicSettings::isSoundEffectOn();
    if(sound)
        os << uint16_t(1);
    else
        os << uint16_t(0);

    bool music = MusicSettings::isMusicOn();
    if(music)
        os << uint16_t(1);
    else
        os << uint16_t(0);

    std::ofstream oss(_settings_path, std::ios::out | std::ios::binary);
    oss.write(ss.str().c_str(), ss.str().length());


    //save hint number in file
    std::stringstream ss2(std::ios::out | std::ios::binary);
    OutputBinaryStream os2(ss2,BinaryStream::MaxProtocolVersion);

    os2 << uint32_t(_hints_number);

    std::ofstream oss2(_hints_path, std::ios::out | std::ios::binary);
    oss2.write(ss2.str().c_str(), ss2.str().length());

    //save is full version buyed
    std::stringstream ss3(std::ios::out | std::ios::binary);
    OutputBinaryStream os3(ss3,BinaryStream::MaxProtocolVersion);

    bool full_version = false;
    if(full_version)
        os3 << uint16_t(1);
    else
        os3 << uint16_t(0);

    std::ofstream oss3(_full_version_path, std::ios::out | std::ios::binary);
    oss3.write(ss3.str().c_str(), ss3.str().length());

}
void LevelManager::loadSettings()
{
    if(FileUtils::isFileExists(_settings_path))
    {
        std::ifstream iss(_settings_path, std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        uint16_t sound_effect = 0;
        uint16_t music_effect = 0;
        is >> sound_effect;
        is >> music_effect;

        if(sound_effect == 0)
        {
            MusicSettings::turnOffSoundEffect();
        }
        else
        {
            MusicSettings::turnOnSoundEffect();
        }

        if(music_effect == 0)
        {
            MusicSettings::turnOffMusic();
        }
        else
        {
            MusicSettings::turnOnMusic();
        }
    }

    //save hint number to the LevelManager
    if(FileUtils::isFileExists(_hints_path))
    {
        std::ifstream iss(_hints_path, std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        uint32_t hints_number = 10;
        is >> hints_number;

        _hints_number = hints_number;
    }

    //save full version info
    if(FileUtils::isFileExists(_full_version_path))
    {
        std::ifstream iss(_full_version_path, std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        uint16_t full = 0;
        is >> full;

        _full_version = full;
    }

}

void LevelManager::saveGame()
{
    std::stringstream ss(std::ios::out | std::ios::binary);
    OutputBinaryStream os(ss,BinaryStream::MaxProtocolVersion);

    os << uint32_t(_collections.size());

    for(auto& i : _collections)
    {
        CollectionPtr col = i.second;
        os << uint32_t(col->getCollectionID()) << uint32_t(col->getLevelsNumber());
        for(const JoinyLevel& level : *col->_levels)
        {
            os << uint32_t(level.getLevelId()) << uint32_t(level.getHighScore());
        }

    }

    std::ofstream oss(_save_path, std::ios::out | std::ios::binary);
    oss.write(ss.str().c_str(), ss.str().length());
}

bool LevelManager::resetProgress()
{
    if(FileUtils::isFileExists(_save_path))
    {
        std::ifstream iss(_save_path, std::ios::in | std::ios::binary);

        iss.clear();
        iss.close();
    }

//    for(CollectionsArr::iterator it = _rw->_collections.begin();
//        it != _rw->_collections.end(); ++it)
//    {
//        Collection* a = it->second;
//        if(a->_state == Collection::Unlocked)
//            a->_state = Collection::Locked;
//    }

//    for(auto& i : _collections)
//    {
//        CollectionPtr col = i.second;
//        os << uint32_t(col->getCollectionID()) << uint32_t(col->getLevelsNumber());
//        for(const JoinyLevel& level : *col->_levels)
//        {
//            os << uint32_t(level.getLevelId()) << uint32_t(level.getHighScore());
//        }

//    }

    saveGame();
    return true;
}

void LevelManager::loadGame()
{
    if(FileUtils::isFileExists(_save_path))
    {
        std::ifstream iss(_save_path, std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

        uint32_t collections_number = 0;
        is >> collections_number;

        for(uint32_t i = 0; i<collections_number; ++i)
        {
            uint32_t col_id=0;
            uint32_t levels_num=0;
            is >> col_id >> levels_num;

            CollectionMap::iterator it = _collections.find(col_id);
            if(it == _collections.end())
                return;
            CollectionPtr col = _collections[col_id];

            if(col->getLevelsNumber() != levels_num)
                return;

            for(unsigned int i=0; i<levels_num; ++i)
            {
                uint32_t level_id=0;
                uint32_t highscore=0;

                is >> level_id >> highscore;
                if(level_id > levels_num)
                    return;

                JoinyLevel* level = col->getLevel(level_id);
                level->_highscore = highscore;

            }
        }

    }

}
unsigned int LevelManager::getCollectionMaxStars(const JoinyCollection* coll)
{
    return coll->getLevelsNumber()*3;
}
unsigned int LevelManager::getCollectionCurrStars(const JoinyCollection *coll)
{
    return coll->getStarSum();
}
unsigned int LevelManager::getCollectionMinStars(const JoinyCollection *coll)
{
    return coll->getMinStarsNumber();
}
void LevelManager::makePreviews(unsigned int joiny_size,
                                unsigned int levels_num)
{


    for(unsigned int curr_l_num = 0; curr_l_num < levels_num; ++curr_l_num)
    {
        std::string curr_file_name = AD_to_string(joiny_size)
                + "x"
                + AD_to_string(joiny_size)
                +"_"
                + AD_to_string(curr_l_num);

        std::stringstream fname;
        fname << "../build/JoinyGeneratorDebug/" << curr_file_name << ".ad";

        JoinyPuzzle inp;
        JoinyTask task;
        std::stringstream ss;
        if(FileUtils::openPackageFile(fname.str().c_str(), ss))
        {
            InputBinaryStream is(ss);
            is >> inp;
        }
        task = inp.getJoinyTask();

        //const CCPoint ORIGIN = Screen::getOrigin();
        const CCSize VISIBLE_SIZE = Screen::getVisibleSize();

        FlowTable table(joiny_size,joiny_size);
        for(unsigned int i=0; i<task.size(); ++i)
        {
            JoinyPair pair = task[i];
            FlowStartEnd start_end = pair.getPoints();
            unsigned int color = pair.getColor();

            table.addColor(start_end.first,start_end.second, color);
        }

        FlowRenderer renderer(table);

        CCRenderTexture * texture = CCRenderTexture::create(joiny_size*100,
                                                        joiny_size*100);
        texture->setPosition(ccp(VISIBLE_SIZE.width/2,
                                 VISIBLE_SIZE.height/2));
        texture->beginWithClear(1,1,1,1);
        renderer.setScale(0.5);
        renderer.visit();
        texture->end();

        std::string save_png_name = "../../JoinyGeneratorDebug/"
                + curr_file_name
                + ".png";
        texture->saveToFile(save_png_name.c_str(),
                            kCCImageFormatPNG);
    }

}

void LevelManager::SaveScreenshot()
{
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();

    CCRenderTexture * texture = CCRenderTexture::create(int(VISIBLE_SIZE.width),
                                                    int(VISIBLE_SIZE.height));
    texture->setPosition(ccp(VISIBLE_SIZE.width/2, VISIBLE_SIZE.height/2));
    texture->beginWithClear(1,1,1,1);
    CCDirector::sharedDirector()->getRunningScene()->visit();
    texture->end();
    texture->saveToFile("screenshot.png", kCCImageFormatPNG);
}

const unsigned int LevelManager::getHintNumber() const
{
    return _hints_number;
}
void LevelManager::decreaseHintNumber(unsigned int num)
{
    _hints_number-=num;
}
void LevelManager::increaseHintNumber(unsigned int num)
{
    _hints_number+=num;
}

bool LevelManager::isFullGameVersion() const
{
    return _full_version;
}
void LevelManager::makeFullGameVersion()
{
    _full_version = true;
}
