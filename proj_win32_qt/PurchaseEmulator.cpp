#include "StoreBridge/cocos2dx_EventHandlers.h"
#include "StoreBridge/cocos2dx_StoreController.h"
#include "AppDelegate.h"
#include "Core/Browser.h"
#include "Core/Statistics.h"
#include "Logic/Language.h"
#include "Core/Notification.h"
cocos2dx_EventHandlers* cocos2dx_EventHandlers::instance=0;

cocos2dx_EventHandlers* cocos2dx_EventHandlers::getInstance()
{
    if(instance == 0)
        instance = new cocos2dx_EventHandlers();
    return instance;
}

void cocos2dx_EventHandlers::addHandler(IEventHandler* )
{}
void cocos2dx_EventHandlers::removeHandler(IEventHandler*)
{}
void cocos2dx_StoreController::initialize(string)
{}
void cocos2dx_StoreController::restoreTransactions()
{
#ifndef JUNIOR
    unsigned int i = rand() % 2;
    bool res = i == 1;
    AppDelegate::getPurchaseHandler()->restoreTransactions(res);
#endif
}

void cocos2dx_StoreController::buyMarketItem(string item)
{
#ifndef JUNIOR
    AppDelegate::getPurchaseHandler()->marketPurchase(item);
#endif
}
void cocos2dx_StoreController::storeOpening()
{}
void cocos2dx_StoreController::storeClosing()
{}
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
