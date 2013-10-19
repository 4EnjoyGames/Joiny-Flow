#include "Store.h"

#include "StoreBridge/cocos2dx_StoreController.h"
#include "Layers/BackgroundHolder.h"
std::string Store::ITEM_UNLOCK_ALL="unlock_all";
std::string Store::ITEM_BUY_ALL="buy_all";
std::string Store::PREFIX_BUY_COLLECTION="buy_";
std::string Store::PREFIX_BUY_HINTS="hint_";

bool Store::buyItem(const std::string& item)
{
#ifndef JUNIOR
    BackgroundHolder::storeOpen();
    try
    {
        cocos2dx_StoreController::buyMarketItem(item);
    }
    catch(cocos2dx_VirtualItemNotFoundException&)
    {
        return false;
    }
#endif
    return true;
}
void Store::restorePurchases()
{
#ifndef JUNIOR
    BackgroundHolder::storeOpen();
    cocos2dx_StoreController::restoreTransactions();
#endif
}
