#ifndef RWGENERIC_H
#define RWGENERIC_H
class Loading;
class AppDelegate;
#include <memory>
template <class TLogicManager, class TLevelManager, class TMusicManager>
class RWGeneric
{
public:
    typedef TLogicManager LogicManager;
    typedef TLevelManager LevelManager;
    typedef TMusicManager MusicManager;

    static LogicManager& getLogicManager()
    {
        return *_logic_manager;
    }

    static LevelManager& getLevelManager()
    {
        return *_level_manager;
    }

    static MusicManager& getMusicManager()
    {
        return *_music_manager;
    }

private:
    static void onInit()
    {
        _logic_manager = std::shared_ptr<TLogicManager>(new TLogicManager);
        _level_manager = std::shared_ptr<TLevelManager>(new TLevelManager);
        _music_manager = std::shared_ptr<TMusicManager>(new TMusicManager);

        _level_manager->onInit();
        _logic_manager->onInit();
        _music_manager->onInit();

        _level_manager->onRun();
        _logic_manager->onRun();
        _music_manager->onRun();
    }

    static void onDestroy()
    {
        if(_level_manager.get() && _logic_manager.get() && _music_manager.get())
        {
            _level_manager->onDestroy();
            _logic_manager->onDestroy();
            _music_manager->onDestroy();
        }
    }

    static void onPause()
    {
        if(_level_manager.get() && _logic_manager.get() && _music_manager.get())
        {
            _level_manager->onPause();
            _logic_manager->onPause();
            _music_manager->onPause();
        }
    }

    static std::shared_ptr<LogicManager> _logic_manager;
    static std::shared_ptr<LevelManager> _level_manager;
    static std::shared_ptr<MusicManager> _music_manager;

    friend class AppDelegate;
    friend class Loading;
};

template <class A, class B, class C>
std::shared_ptr<A> RWGeneric<A,B,C>::_logic_manager;

template <class A, class B, class C>
std::shared_ptr<B> RWGeneric<A,B,C>::_level_manager;

template <class A, class B, class C>
std::shared_ptr<C> RWGeneric<A,B,C>::_music_manager;

#define RWManager template<class, class, class> friend class RWGeneric;
#endif // RWGENERIC_H
