#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H
#include "RWGeneric.h"
#include "JoinyCollection.h"
#include <map>
#include <memory>
#include "GameInfo.h"

class LevelManager
{
public:
    typedef std::shared_ptr<JoinyCollection> CollectionPtr;
    const JoinyCollection *getCollection(const JoinyCollectionID id) const;

    const JoinyLevel* getNextLevel(const JoinyLevel* level) const;

    void onLevelFinished(const JoinyLevel* level, const Score score);
private:
    LevelManager();
    void onInit();
    void onDestroy();
    void onPause();
    void onRun();

    void loadLevelsInfo();

    void saveGame();
    void loadGame();



    std::string _save_path;

    typedef std::map<JoinyLevelID, CollectionPtr> CollectionMap;

    CollectionMap _collections;

    RWManager
};

#endif // LEVELMANAGER_H
