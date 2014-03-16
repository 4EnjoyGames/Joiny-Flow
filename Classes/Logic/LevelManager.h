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
    const JoinyLevel* getLevel( const JoinyLevelID id, const JoinyLevelID l_id) const;
    const JoinyCollection *getCollection(const JoinyCollectionID id) const;

    const JoinyLevel* getNextLevel(const JoinyLevel* level) const;
    const JoinyLevel* getPreviousLevel(const JoinyLevel* level) const;

    void onLevelFinished(const JoinyLevel* level, const Score score);


    void loadSettings();
    void saveSettings();

    unsigned int getCollectionMaxStars(const JoinyCollection *coll);
    unsigned int getCollectionCurrStars(const JoinyCollection *coll);
    unsigned int getCollectionMinStars(const JoinyCollection *coll);

    void makePreviews(unsigned int joiny_size, unsigned int levels_num);
    void SaveScreenshot();

    const unsigned int getHintNumber() const;
    void decreaseHintNumber(unsigned int num=1);
    void increaseHintNumber(unsigned int num=1);

    bool isFullGameVersion() const;
    void makeFullGameVersion();

    //return is resetting succesfull or no
    bool resetProgress();

private:
    LevelManager();
    void onInit();
    void onDestroy();
    void onPause();
    void onRun();

    void loadLevelsInfo();

    void saveGame();
    void loadGame();


    std::string _full_version_path;
    std::string _save_path;
    std::string _settings_path;
    std::string _hints_path;

    unsigned int _hints_number;

    bool _full_version;

    typedef std::map<JoinyLevelID, CollectionPtr> CollectionMap;

    CollectionMap _collections;

    RWManager
};

#endif // LEVELMANAGER_H
