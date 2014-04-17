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
#include "Logic/Hints.h"
#include "Logic/RW.h"
#include "Scenes/LevelScene.h"
#include "Scenes/SelectCollection.h"
#include "ADLib/Device/ADBrowser.h"
#include <ADLib/Device/ADVirtualCurrency.h>
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
        Hints hints;
        if(id == "hints_10")
            hints.increaseHintNumber(10);
        else if(id == "hints_100")
            hints.increaseHintNumber(100);
        else if(id == "hints_1000")
            hints.increaseHintNumber(1000);
        else if(id == "unlock_full")
            RW::getLevelManager().makeFullGameVersion();

        CCLog("Purchase %s finished successful", id.c_str());

        if(id != "unlock_full")
        {
            RW::getLevelManager().saveSettings();
            LevelScene::purchaseUpdateHints();
        }
        else
            SelectCollection::purchaseUpdateFullGame();

        //
    }
    void purchaseFailed(const ADInApp::ProductID & id,
                        const ADInApp::ErrorType error)
    {
        typedef ADInApp::ErrorType Error;
        if(error == ADInApp::ErrorType::BillingUnavaliable)
        {
            ADNotification::showNotification(_("iap.error.billing_unavaliable"));
        }
        else if(error == ADInApp::ErrorType::DeveloperError ||
                error == ADInApp::ErrorType::Error)
        {
            ADNotification::showNotification(_("iap.error.purchase_error"));
        }

        CCLog("Purchase %s failed", id.c_str());
    }

    void restorePurchasesSuccessfully()
    {
        ADNotification::showNotification(_("iap.restore_purchases.success"));
    }

    void restorePurchasesFailed()
    {
        ADNotification::showNotification(_("iap.restore_purchases.failed"));
    }
};

class VirtualCurrencyDelegate : public ADVirtualCurrency::Delegate
{
    void onCurrencyAdded(const unsigned int number)
    {
        Hints hints;
        cocos2d::CCLog("Hints recieved: %d", number);
        hints.increaseHintNumber(number);
        LevelScene::purchaseUpdateHints();

        RW::getLevelManager().saveSettings();
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
        if(GameInfo::getVersion() != Version::Premium)
            ADInApp::setStoreKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzjm8X23+3Aws5jkK8wBdau9jWYmIOSSdW9VfEg3E5f2lwolhWxxMpn7+o1SqZ2HljKDwvLyeVRLKuBjiwW/0OzMzFwW/JSFQdz/zFKL8UsypmKTdLc3dTo6mgPgUshgchse6ArF3xRWMFYVzmf8ePLDozCfGp71jhSbO5Rx5mQrq9se0eThRpuGUcNUC9PNvvQOnSm6ddkpnuVfxvLGwBRM7RlDCT60ClrfTY+/A6x0+YSiz7C+LJDaxkUg0X1U3nzQbVG8V9L0nk0dW6Fdtc5vY6wAcwtAV5KsX7tbQwrTM+OD2+R7h/3MqM/BH2/MWFXcoT9hakx6ZrYlc8hDZIwIDAQAB");
        else
            ADInApp::setStoreKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAoIhqi4akTyYD9cYWD+GN5yWqhu2obg+bq2i/Bu5aipras7LinAXGZapVDMd+lrQ+EmrauyBL/lrdhzXdZryQ77ExxJUMXxBm0wVkpNeSYe4DBs6eh/g7/r7D4YqZ73uHldHN+UcTTnZj9O/O46S9n6i5F0Sgu2hvMdGKfYgH+Dg+21UCsxc3drXkytNvAoq1dFz+Zi2kWsnTMscSrQ5rqVw8nLfEQR3vXg2JXeZJkxATj8Ew4CXlEzMCFE02GBDyosSz0dL7xxoSs10yrkcW+Xs0p7k68yyuxgiCt+I5FUI2pIYuF5Qp2mr5k21s3FaSleLRdY9oa2XQkzB3y7TWkwIDAQAB");
        unlock_full.setParameter("type", "non-consumable");
        hints_10.setParameter("type", "consumable");
        hints_100.setParameter("type", "consumable");
        hints_1000.setParameter("type", "consumable");
    }
    else if(store == ADStore::SamsungStore)
    {
        if(GameInfo::getVersion() != Version::Premium)
        {
            ADInApp::setStoreKey("100000102739");
            unlock_full.setParameter("samsung-id", "000001013715");
            hints_10.setParameter("samsung-id", "000001013712");
            hints_100.setParameter("samsung-id", "000001013713");
            hints_1000.setParameter("samsung-id", "000001013714");
        }
        else
        {
            ADInApp::setStoreKey("100000103136");
            hints_10.setParameter("samsung-id", "000001016100");
            hints_100.setParameter("samsung-id", "000001016101");
            hints_1000.setParameter("samsung-id", "000001016103");
        }
    }
    else if(store == ADStore::AmazonStore)
    {
        if(GameInfo::getVersion() == Version::Premium)
        {
            hints_10.setParameter("asku", "joiny.premium.hints_10");
            hints_100.setParameter("asku", "joiny.premium.hints_100");
            hints_1000.setParameter("asku", "joiny.premium.hints_1000");
        }
    }
    else if(store == ADStore::iTunes)
    {
        if(GameInfo::getVersion() == Version::Premium)
        {
            hints_10.setParameter("asku", "joiny.premium.hints_10");
            hints_100.setParameter("asku", "joiny.premium.hints_100");
            hints_1000.setParameter("asku", "joiny.premium.hints_1000");
        }
    }

    if(GameInfo::getVersion() != Version::Premium)
        ADInApp::addProduct(unlock_full);

    ADInApp::addProduct(hints_10);
    ADInApp::addProduct(hints_100);
    ADInApp::addProduct(hints_1000);

    ADInApp::setDelegate(std::make_shared<InAppDelegate>());




}

/**
 * @brief Performs init and configuration of AdMob and home ads
 */
void initAds()
{
    ADStore store = ADInfo::getStore();
    ADPlatform platform = ADInfo::getPlatform();

    std::stringstream pid_banner;
    std::stringstream pid_interstitial;

    pid_banner << "ca-app-pub-" << 16126979 << 60946304 << "/";
    pid_interstitial << "ca-app-pub-" << 16126979 << 60946304 << "/";

    if(platform == ADPlatform::Android)
    {
        if(store == ADStore::SamsungStore)
        {
            //ca-app-pub-1612697960946304/8892739877
            pid_banner  << 8892739877;
            //ca-app-pub-1612697960946304/4322939475
            pid_interstitial << 4322939475;
        }
        else if(store == ADStore::GooglePlay)
        {
            //ca-app-pub-1612697960946304/5939273474
            pid_banner << 5939273474;
            //ca-app-pub-1612697960946304/1369473073
            pid_interstitial << 1369473073;
        }
        else
        {
            //ca-app-pub-1612697960946304/7416006677
            pid_banner << 7416006677;
            //ca-app-pub-1612697960946304/2846206274
            pid_interstitial << 2846206274;
        }
    }
    else if(platform == ADPlatform::iOS)
    {
            //ca-app-pub-1612697960946304/4238554272
            pid_banner << 4238554272;

            //ca-app-pub-1612697960946304/5715287478
            pid_interstitial << 5715287478;
    }
	else if(platform == ADPlatform::WindowsPhone)
	{
		//ca-app-pub-1612697960946304/9611946677
        pid_banner << 9611946677;

        //ca-app-pub-1612697960946304/2088679871
        pid_interstitial << 2088679871;
	}

    ADAds::registerBannerType("BANNER", pid_banner.str());

	//Windows Phone supports only one banner type
	if(platform != ADPlatform::WindowsPhone)
		ADAds::registerBannerType("IAB_BANNER", pid_banner.str());

    ADAds::registerInterstitialType(pid_interstitial.str());

    //ADAds::addTestDevice("419CBB113860522A7AB95487DBB0CC2B"); //Andriy Tab
    ADAds::addTestDevice("9AC43D4250441F63E2E677C2C06F5D41"); //Diana Tab
    ADAds::addTestDevice("C31238A94F2B52E9F4B77E58270A3943"); //Tonya

    ADAds::addCustomBanner(CCSprite::create("banners/mif_kids.png"),
                           [](){
        //TODO: select which game baner is it
        ADBrowser::openApplicationPage(GameInfo::getPackageName("mif"));
    },
    "MiF");
}

void initTapJoy()
{
    if(ADInfo::getPlatform() == ADPlatform::Android)
    {
        std::stringstream ss;
        ss << "f1041413-201e-443d-b03a-82b0bdb941b1" << "|" << "QZdV7bkpyvZiGxiwBDw2";
        ADVirtualCurrency::initProvider(ss.str());
    }

    ADVirtualCurrency::setDelegate(std::make_shared<VirtualCurrencyDelegate>());
}

bool AppDelegate::applicationDidFinishLaunching() {
    
    //if(ADInfo::getPlatform() == ADPlatform::Android)
        RW::onInit();

    //Statistics init
    if(GameInfo::getVersion() != Version::Premium)
    {
        if(ADInfo::getPlatform() == ADPlatform::Android)
            ADStatistics::setApplicationKey("2YHVNKMPHQW5FF459KJJ");
        else if(ADInfo::getPlatform() == ADPlatform::iOS)
            ADStatistics::setApplicationKey("2CKY6VTP8TQ5XR7CPR5Y");
		else if(ADInfo::getPlatform() == ADPlatform::WindowsPhone)
			ADStatistics::setApplicationKey("CP65XWB4YWTCQF5J9B6W");
    }
    else
    {
        if(ADInfo::getPlatform() == ADPlatform::Android)
            ADStatistics::setApplicationKey("B2QN6WVFZ2W259SCYDYJ");
        else if(ADInfo::getPlatform() == ADPlatform::iOS)
            ADStatistics::setApplicationKey("95YVYC295ZX5RB6BKJ9S");
    }
    ADStatistics::startSession();
    ADInfo::getStore();

    //Language init
    ADLanguage::addSupportedLanguage("en");
    ADLanguage::addSupportedLanguage("uk");
    //ADLanguage::addSupportedLanguage("pt");
    ADLanguage::addSupportedLanguage("ru");
    ADLanguage::addSupportedLanguage("hu");
    ADLanguage::addSupportedLanguage("de");
    //ADLanguage::addSupportedLanguage("de");

    ADLanguage::setDefaultLanguage("en");
    ADLanguage::getLanguage();

    initInAppPurchases();
    if(GameInfo::getVersion() != Version::Premium)
    {

        initTapJoy();
    }
    else
    {
        ADAds::disableAds();
    }

    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();


    pDirector->setOpenGLView(pEGLView);

    CCSize frameSize = pEGLView->getFrameSize();

    // Set the design resolution
    if(frameSize.width > frameSize.height)
    {
        //Mode for Samsung who launches in wrong mode
        ADAds::disableAds();
        pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);
    }
    else
    {
        // Set the design resolution
        pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    }
    CCSize vsize = pEGLView->getVisibleSize();


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
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    
    //std::ifstream is(CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile());
    // create a scene. it's an autorelease object
    CCScene *pScene = Loading::scene();

    // run
    pDirector->runWithScene(pScene);


    if(GameInfo::getVersion() != Version::Premium)
    {
        initAds();
    }
    
    
    
    return true;
}
#include <ADLib/Device/ADDeviceEvents.h>
// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    //ADStatistics::stopSession();
    //ADVirtualCurrency::onPause();
    ADDeviceEvents::onPause();
    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    RW::onPause();
}
#include "Core/MusicSettings.h"
#include "Core/DrawLayer.h"
// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    ADDeviceEvents::onResume();
    //ADStatistics::startSession();
    //ADVirtualCurrency::onResume();
    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();

}
