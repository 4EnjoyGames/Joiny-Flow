#ifndef RW_H
#define RW_H

#include "RWGeneric.h"
#include "LevelManager.h"
class DummyManager
{
private:
    DummyManager()
    {}
    void onInit()
    {}
    void onDestroy()
    {}
    void onRun()
    {}
    void onPause()
    {}

    RWManager
};

typedef RWGeneric<DummyManager, LevelManager, DummyManager> RW;


#endif // RW_H
