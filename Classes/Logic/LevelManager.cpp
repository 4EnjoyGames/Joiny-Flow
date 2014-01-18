#include "LevelManager.h"
#include "Core/FileUtils.h"
#include <fstream>
LevelManager::LevelManager()
    : _save_path(FileUtils::getStorageFilePath("save.ad"))
{
}

void LevelManager::onInit()
{
    loadLevelsInfo();
}

void LevelManager::onDestroy()
{
    saveGame();
}

void LevelManager::onPause()
{
    saveGame();
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
    std::vector<std::string> coll_names = info->getCollectionNames();
    std::vector<ccColor3B> coll_colors  = info->getCollectionColors();

    std::vector<FlowPoint> flow_points = info->getFlowPoints();

    for(unsigned int j=0; j<coll_files.size(); ++j)
    {
        std::string file = coll_files[j];
        std::stringstream ss;
        if(FileUtils::openPackageFile(file, ss))
        {
            InputBinaryStream is(ss);
            std::vector<JoinyPuzzle> inp;
            is >> inp;

            CollectionPtr col(new JoinyCollection);
            col->_id = j+1;
            col->_levels = std::shared_ptr<JoinyCollection::LevelsVector>(
                        new JoinyCollection::LevelsVector(inp.size(),
                                                          JoinyLevel()));
            col->setCollectionName(coll_names[j]);
            col->setCollectionColor(coll_colors[j]);

            for(unsigned int i=0; i<inp.size(); ++i)
            {
                JoinyLevel& l = col->_levels->at(i);
                l._level_id = i+1;
                l._parent = col.get();
                l._puzzle = inp[i];
                l._size = flow_points[j];
            }

            _collections[col->_id] = col;
        }
    }
}

void LevelManager::saveGame()
{
    std::stringstream ss(std::ios::out | std::ios::binary);
    OutputBinaryStream os(ss,BinaryStream::MaxProtocolVersion);

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

void LevelManager::loadGame()
{
    if(FileUtils::isFileExists(_save_path))
    {
        std::ifstream iss(_save_path, std::ios::in | std::ios::binary);
        InputBinaryStream is(iss);

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
