#include "AppDelegate.h"
//#include "Scenes/Loading.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Scenes/Loading.h"
#include "Core/Screen.h"
#include <ADLib/Device/ADAds.h>
#include <ADLib/Device/ADLanguage.h>
#include <ADLib/Device/ADStatistics.h>
#include <ADLib/Device/ADInfo.h>
#include <ADLib/Device/ADInApp.h>
#include <ADLib/Device/ADNotification.h>
#include "Localization/CCLocalizedString.h"
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

class InAppDelegate : public ADInApp::Delegate
{
public:
    void purchaseSuccessful(const ADInApp::ProductID & id)
    {
        CCLog("Purchase %s finished successful", id.c_str());
    }
    void purchaseFailed(const ADInApp::ProductID & id,
                        const ADInApp::ErrorType error)
    {
        typedef ADInApp::ErrorType Error;
        if(error == ADInApp::ErrorType::BillingUnavaliable)
        {
            ADNotification::showNotification(CCLocalizedString("iap.error.billing_unavaliable"));
        }
        else if(error == ADInApp::ErrorType::DeveloperError ||
                error == ADInApp::ErrorType::Error)
        {
            ADNotification::showNotification(CCLocalizedString("iap.error.purchase_error"));
        }

        CCLog("Purchase %s failed", id.c_str());
    }

    void restorePurchasesSuccessfully()
    {
        ADNotification::showNotification(CCLocalizedString("iap.restore_purchases.success"));
    }

    void restorePurchasesFailed()
    {
        ADNotification::showNotification(CCLocalizedString("iap.restore_purchases.failed"));
    }
};


void initInAppPurchases()
{
    typedef ADInApp::Product Product;
    Product unlock_full("unlock_full", "$1.99");
    Product hints_10("hints_10", "$0.99");
    Product hints_100("hints_100", "$3.99");
    Product hints_1000("hints_1000", "$9.99");

    ADStore store = ADInfo::getStore();
    if(store == ADStore::GooglePlay)
    {
        ADInApp::setStoreKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzjm8X23+3Aws5jkK8wBdau9jWYmIOSSdW9VfEg3E5f2lwolhWxxMpn7+o1SqZ2HljKDwvLyeVRLKuBjiwW/0OzMzFwW/JSFQdz/zFKL8UsypmKTdLc3dTo6mgPgUshgchse6ArF3xRWMFYVzmf8ePLDozCfGp71jhSbO5Rx5mQrq9se0eThRpuGUcNUC9PNvvQOnSm6ddkpnuVfxvLGwBRM7RlDCT60ClrfTY+/A6x0+YSiz7C+LJDaxkUg0X1U3nzQbVG8V9L0nk0dW6Fdtc5vY6wAcwtAV5KsX7tbQwrTM+OD2+R7h/3MqM/BH2/MWFXcoT9hakx6ZrYlc8hDZIwIDAQAB");
        unlock_full.setParameter("type", "non-consumable");
        hints_10.setParameter("type", "consumable");
        hints_100.setParameter("type", "consumable");
        hints_1000.setParameter("type", "consumable");
    }
    else if(store == ADStore::SamsungStore)
    {
        ADInApp::setStoreKey("100000102739");
        unlock_full.setParameter("samsung-id", "000001013715");
        hints_10.setParameter("samsung-id", "000001013712");
        hints_100.setParameter("samsung-id", "000001013713");
        hints_1000.setParameter("samsung-id", "000001013714");
    }

    ADInApp::addProduct(unlock_full);
    ADInApp::addProduct(hints_10);
    ADInApp::addProduct(hints_100);
    ADInApp::addProduct(hints_1000);

    ADInApp::setDelegate(std::make_shared<InAppDelegate>());

    ADInApp::loadStore(ADInApp::Mode::TestSuccess);


}

bool AppDelegate::applicationDidFinishLaunching() {
    //Statistics init
    if(ADInfo::getPlatform() == ADPlatform::Android)
        ADStatistics::setApplicationKey("2YHVNKMPHQW5FF459KJJ");
    ADStatistics::startSession();
    ADInfo::getStore();

    //Language init
    ADLanguage::addSupportedLanguage("en");
    //ADLanguage::addSupportedLanguage("uk");
    //ADLanguage::addSupportedLanguage("ru");
    //ADLanguage::addSupportedLanguage("hu");
    //ADLanguage::addSupportedLanguage("fr");
    //ADLanguage::addSupportedLanguage("de");

    ADLanguage::setDefaultLanguage("en");
    ADLanguage::getLanguage();

    initInAppPurchases();

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



    std::string pid = "ca-app-pub-1097233306002326/3272087699";
    ADAds::registerBannerType("BANNER", pid);
    ADAds::registerBannerType("IAB_BANNER", pid);

    std::string pid_inter = "ca-app-pub-1097233306002326/5031020099";
    ADAds::registerInterstitialType(pid_inter);

    ADAds::addTestDevice("419CBB113860522A7AB95487DBB0CC2B"); //Andriy Tab
    ADAds::addTestDevice("9AC43D4250441F63E2E677C2C06F5D41"); //Diana Tab
    ADAds::addTestDevice("C31238A94F2B52E9F4B77E58270A3943"); //Tonya

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    ADStatistics::stopSession();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    RW::onPause();
}
#include "Core/MusicSettings.h"
#include "Core/DrawLayer.h"
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    ADStatistics::startSession();

    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    DrawLayer::registerUpdateDrawingNodes();
}
