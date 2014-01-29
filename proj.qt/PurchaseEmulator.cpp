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
#define AD_LANGUAGE_CURRENT "en"

#include <ADLib/PlatformImpl/ADBrowser_None.hpp>
#include <ADLib/PlatformImpl/ADStatistics_None.hpp>
#include <ADLib/PlatformImpl/ADLanguage_Default.hpp>
#include <ADLib/PlatformImpl/ADNotification_None.hpp>
#include <ADLib/PlatformImpl/ADInfo_Default.hpp>
