#include "main.h"
#include "../Classes/AppDelegate.h"
#include <Testing/ADDeviceEmulator.h>

void mainMenuTest(TestInfo);
void selectCollectionTest(TestInfo info);
void developersTest(TestInfo info);

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
    emulator->setLanguage("uk");
    emulator->setOrientation(Orientation::Portrait);

    //Test cases
    emulator->addTestCase(&mainMenuTest);
    emulator->addTestCase(&selectCollectionTest);
    emulator->addTestCase(&developersTest);

    return emulator->run();
}


#include "Scenes/MainScene.h"
#include "Scenes/SelectCollection.h"
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
