#include "AppDelegate.h"
#include "Core/Browser.h"
#include "Core/Statistics.h"
#include "Logic/Language.h"
#include "Core/Notification.h"

void Browser::openURL(const URLId id)
{
    cocos2d::CCLog("Open URL: %d", id);
}

void Statistics::statisticEvent(const StatisticsEvent id)
{
    cocos2d::CCLog("Event %d", id);
}

void Statistics::detailStatisticEvent(const StatisticsEvent id, const unsigned int level_id, const unsigned int collection_id, const unsigned int stamps_num)
{
    cocos2d::CCLog("Event %d, Level %d, Collection %d, Stamps %d", id, level_id+1, collection_id, stamps_num);
}

Language::Languages Language::getDeviceLanguage()
{
    return Language::English;
}

void Notification::notify(const Type t)
{
    cocos2d::CCLog("Notification ID %d", t);

}
#define AD_ADS_NOSIZE
#include <ADLib/PlatformImpl/ADAds_NoAds.hpp>
cocos2d::CCSize ADAds::Platform::getBannerSize(const BannerType& type)
{
    cocos2d::CCSize base (468, 60);
    if(type == "BANNER")
        base = cocos2d::CCSize(320, 50);

    float density = 1;
    return cocos2d::CCSize(base.width*density, base.height*density);
}
#define AD_LANGUAGE_CURRENT "ru"

#include <ADLib/PlatformImpl/ADBrowser_None.hpp>
#include <ADLib/PlatformImpl/ADStatistics_None.hpp>
#include <ADLib/PlatformImpl/ADLanguage_Default.hpp>
#include <ADLib/PlatformImpl/ADNotification_None.hpp>
#include <ADLib/PlatformImpl/ADInfo_Default.hpp>
