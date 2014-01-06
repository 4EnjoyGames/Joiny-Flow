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

USING_NS_CC;

#ifdef CC_WIN8_METRO
#include "CCEGLView.h"
#endif

#include "Logic/RW.h"
#ifndef JUNIOR
PurchaseHandler AppDelegate::_purchase_handler;
PurchaseHandler* AppDelegate::getPurchaseHandler()
{
    return &_purchase_handler;
}
#endif

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
    RW::onDestroy();
#ifndef JUNIOR
    cocos2dx_EventHandlers::getInstance()->removeHandler(&_purchase_handler);
#endif
    //delete _purchase_handler;
}
#ifdef CC_WIN8_METRO
bool AppDelegate::initInstance()
{
    bool bRet = false;
    do
    {



        // fix bug: 16bit aligned
        void* buff=_aligned_malloc(sizeof(CCEGLView),16);
        CCEGLView* mainView = new (buff) CCEGLView();

        CCDirector *pDirector = CCDirector::sharedDirector();
        mainView->Create();
        pDirector->setOpenGLView(mainView);



        bRet = true;
    } while (0);
    return bRet;
}
#endif
bool AppDelegate::applicationDidFinishLaunching() {



    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

#ifndef CC_WIN8_METRO
    pDirector->setOpenGLView(pEGLView);
#endif

    Language::setLanguage(Language::getDeviceLanguage());

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

    std::vector<std::string> searchPath;
    searchPath.push_back(mediumResource.directory);

    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height/width_change, mediumResource.size.width/designResolutionSize.width/width_change));
    /*if (frameSize.height > mediumResource.size.height)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(largeResource.directory);
        pDirector->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small resource size, select medium resource.
    else if (frameSize.height > smallResource.size.height)
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(mediumResource.directory);
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium resource size, select small resource.
    else
    {
        CCFileUtils::sharedFileUtils()->setResourceDirectory(smallResource.directory);
        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }*/

    // turn on display FPS
    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    
    //std::ifstream is(CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile());
    // create a scene. it's an autorelease object
    CCScene *pScene = Loading::scene();

    // run
    pDirector->runWithScene(pScene);

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
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(MusicSettings::isBackgrHolderMusic() && MusicSettings::isMusicOn())
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    }
#else
    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
#endif
    
}
