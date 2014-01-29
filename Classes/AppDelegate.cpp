#include "AppDelegate.h"
//#include "Scenes/Loading.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Logic/Language.h"
#include "Scenes/Loading.h"
#include "Core/Screen.h"
#include <ADLib/Device/ADAds.h>
#include <ADLib/Device/ADLanguage.h>
#include <ADLib/Device/ADStatistics.h>
USING_NS_CC;

#ifdef CC_WIN8_METRO
#include "CCEGLView.h"
#endif

#include "Logic/RW.h"


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    RW::onDestroy();

    //delete _purchase_handler;
}

bool AppDelegate::applicationDidFinishLaunching() {
    //Statistics init
    ADStatistics::startSession("flurry_id");

    //Language init
    ADLanguage::addSupportedLanguage("en");
    ADLanguage::addSupportedLanguage("uk");
    //ADLanguage::addSupportedLanguage("ru");
    //ADLanguage::addSupportedLanguage("hu");
    //ADLanguage::addSupportedLanguage("fr");
    //ADLanguage::addSupportedLanguage("de");

    ADLanguage::setDefaultLanguage("en");
    ADLanguage::getLanguage();


    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();


    pDirector->setOpenGLView(pEGLView);

    // Set the design resolution
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    CCSize vsize = pEGLView->getVisibleSize();

    CCSize frameSize = pEGLView->getFrameSize();
    //std::cout << "TESST" <<std::endl;
    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.
    float width_change = vsize.width / designResolutionSize.width;
    // if the frame's height is larger than the height of medium resource size, select large resource.
    //CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);

    std::string resource_directory = "images-normal";
    float resource_scale = 1;

    if(frameSize.height < 500)
    {
        resource_directory = "images-small";
        resource_scale = 0.5;
    }

    std::vector<std::string> searchPath;
    searchPath.push_back(resource_directory);

    Screen::setDesignScale(resource_scale);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    pDirector->setContentScaleFactor(
                MIN(
                    mediumResource.size.height*resource_scale/
                    designResolutionSize.height/
                    width_change,

                    mediumResource.size.width*resource_scale/
                    designResolutionSize.width/
                    width_change));

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    
    //std::ifstream is(CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile());
    // create a scene. it's an autorelease object
    CCScene *pScene = Loading::scene();

    // run
    pDirector->runWithScene(pScene);

    ADAds& manager = ADAds::getInstance();

    std::string pid = "ca-app-pub-1097233306002326/3272087699";
    manager.registerBannerType("BANNER", pid);
    manager.registerBannerType("IAB_BANNER", pid);

    std::string pid_inter = "ca-app-pub-1097233306002326/5031020099";
    manager.registerInterstitialType(pid_inter);

    manager.addTestDevice("419CBB113860522A7AB95487DBB0CC2B"); //Andriy Tab
    manager.addTestDevice("9AC43D4250441F63E2E677C2C06F5D41"); //Diana Tab
    manager.addTestDevice("C31238A94F2B52E9F4B77E58270A3943"); //Tonya

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();


    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    RW::onPause();
}
#include "Core/MusicSettings.h"
#include "Core/DrawLayer.h"
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    DrawLayer::registerUpdateDrawingNodes();
}
