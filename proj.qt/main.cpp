#include "main.h"
#include "../Classes/AppDelegate.h"
#include <Testing/ADDeviceEmulator.h>
#include <ADLib/Device/ADAds.h>

void mainMenuTest(TestInfo);
void selectCollectionTest(TestInfo info);
void developersTest(TestInfo info);
void openSelectLevelScene(TestInfo info, int collection_number);
void openPlayLevel(TestInfo info,
                    int collection_number,
                    int level_number,
                    int use_hints);

int CALLBACK WinMain(
        _In_  HINSTANCE,
        _In_  HINSTANCE,
        _In_  LPSTR,
        _In_  int
        )
{
    //Create AppDelegate
    AppDelegate app;

    //Configure emulator
    ADDeviceEmulator* emulator = ADDeviceEmulator::getInstance();

    emulator->setFitTheScreen(true);
    emulator->setDevice(Device::IPad2);
    emulator->setLanguage("en");
    emulator->setOrientation(Orientation::Portrait);

    //Test cases
    //emulator->addTestCase(&mainMenuTest);
    emulator->addTestCase(&selectCollectionTest);

    emulator->addTestCase([](TestInfo info){
        openSelectLevelScene(info,1);
    });
    emulator->addTestCase([](TestInfo info){
        openSelectLevelScene(info,6);
    });


    emulator->addTestCase([](TestInfo info){
        openPlayLevel(info, 1, 5, 2);
    });
    emulator->addTestCase([](TestInfo info){
        openPlayLevel(info, 1, 23, 2);
    });
    emulator->addTestCase([](TestInfo info){
        openPlayLevel(info, 10, 3, 5);
    });
    emulator->addTestCase([](TestInfo info){
        openPlayLevel(info, 4, 5, 6);
    });
    emulator->addTestCase([](TestInfo info){
        openPlayLevel(info, 10, 22, 8);
    });
    emulator->addTestCase([](TestInfo info){
        openPlayLevel(info, 5, 11, 6);
    });
    emulator->addTestCase([](TestInfo info){
        openPlayLevel(info, 6, 35, 8);
    });
    return emulator->run();
}


#include "Scenes/MainScene.h"
#include "Scenes/SelectCollection.h"
#include "Scenes/SelectLevel.h"
#include "Scenes/LevelScene.h"
#include "Scenes/DeveloperScene.h"

void mainMenuTest(TestInfo info)
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
    ADDeviceEmulator::runLater(1.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });

}

void selectCollectionTest(TestInfo info)
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
    ADDeviceEmulator::runLater(1.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}

void developersTest(TestInfo info)
{
    CCDirector::sharedDirector()->replaceScene(DeveloperScene::scene());
    ADDeviceEmulator::runLater(1.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}
void selectLevelTest1(TestInfo info)
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene(RW::getLevelManager().getCollection(1)));
    ADDeviceEmulator::runLater(1.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}
void selectLevelTest2(TestInfo info)
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene(RW::getLevelManager().getCollection(6)));
    ADDeviceEmulator::runLater(1.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}
void openSelectLevelScene(TestInfo info,
                          int collection_number)
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene(RW::getLevelManager().getCollection(collection_number)));
    ADDeviceEmulator::runLater(1.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}

void openPlayLevel(TestInfo info,
                    int collection_number,
                    int level_number,
                    int use_hints)
{
    ADAds::disableAds();
    LevelScene* level = LevelScene::create(
                RW::getLevelManager().getLevel(collection_number,level_number),
                                           false,
                                           false);
    CCScene *scene = CCScene::create();
    scene->addChild(level);

    CCDirector::sharedDirector()->replaceScene(scene);

    for(int i=0; i<use_hints; ++i)
        level->onHintClicked(0);

    ADDeviceEmulator::runLater(1.5f, [info](){

        ADDeviceEmulator::createScreenShoot(info);
        info.finish();
    });
}
