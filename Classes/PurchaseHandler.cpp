#ifndef JUNIOR
#include "cocos2d.h"
#include "PurchaseHandler.h"
#include "StoreBridge/Includes.h"
#include "Layers/BackgroundHolder.h"
#include "Store.h"
//#include "Logic/RW.h"
#include "Core/Notification.h"

using namespace cocos2d;

void PurchaseHandler::marketPurchaseCancelled(string& itemId)
{
    CCLog(("PurchaseHandler::marketPurchaseCancelled:"+itemId).c_str());
    //returnToTheGame(itemId);
}
bool isPrefixItem(const std::string& item_id, const std::string& prefix)
{
    if(item_id.length() < prefix.length())
    {
        return false;
    }

    std::string text_prefix(item_id.begin(), item_id.begin()+prefix.length());
    return text_prefix == prefix;
}
unsigned int strToInt(const std::string& str)
{
    std::stringstream ss(str);
    unsigned int num = 0;
    ss >> num;
    return num;
}

unsigned int itemIdToId(const std::string& item_id, const std::string& prefix)
{
    unsigned int res = 0;
    if(item_id.length() > prefix.length())
    {
        std::string number(item_id.begin()+prefix.length(), item_id.end());
        res = strToInt(number);
    }
    return res;
}

void PurchaseHandler::marketPurchase(string& itemId)
{
//    if(itemId == Store::ITEM_UNLOCK_ALL)
//    {
//        RW::unlockAllCollectionsPurchased();
//        SelectCollection::purchaseReload();
//    }
//    else if(itemId == Store::ITEM_BUY_ALL)
//    {
//        RW::buyAllCollectionsPurchased();
//        SelectCollection::purchaseOpenCollections();
//    }
//    else if(isPrefixItem(itemId, Store::PREFIX_BUY_COLLECTION))
//    {
//        unsigned int coll_id = itemIdToId(itemId, Store::PREFIX_BUY_COLLECTION);
//        RW::buyCollection(coll_id);
//        SelectCollection::purchaseOpenCollections();
//    }
//    else if(isPrefixItem(itemId, Store::PREFIX_BUY_HINTS))
//    {
//        unsigned int count = itemIdToId(itemId, Store::PREFIX_BUY_HINTS);
//        RW::addHints(count);
//        LevelScene::purchaseUpdateHints();
//    }


    CCLog(("PurchaseHandler::marketPurchase:"+itemId).c_str());
}

void PurchaseHandler::restoreTransactions(bool success) {
    if(success)
        Notification::notify(Notification::RestorePurchasesOk);
    else
        Notification::notify(Notification::RestorePurchasesFail);
}

















void PurchaseHandler::billingSupported() {
    CCLog("PurchaseHandler::billingSupported()");
}
void PurchaseHandler::billingNotSupported() {
    CCLog("PurchaseHandler::billingNotSupported()");
}
void PurchaseHandler::closingStore() {
    CCLog("PurchaseHandler::closingStore()");
}
void PurchaseHandler::currencyBalanceChanged(string &/*itemId*/, int /*balance*/, int /*amountAdded*/) {
    CCLog("PurchaseHandler::currencyBalanceChanged");
}
void PurchaseHandler::goodBalanceChanged(string &itemId, int balance, int amountAdded) {
    CCLog(("PurchaseHandler::currencyBalanceChanged: %d %d "+itemId).c_str(), balance, amountAdded);
}
void PurchaseHandler::goodEquipped(string& itemId) {
    CCLog(("PurchaseHandler::goodEquipped:"+itemId).c_str());
}
void PurchaseHandler::goodUnequipped(string& itemId) {
    CCLog(("PurchaseHandler::goodUnequipped:"+itemId).c_str());
}
void PurchaseHandler::goodUpgrade(string& /*itemId*/, string& /*upgradeItemId*/) {
	
}
void PurchaseHandler::itemPurchased(string& itemId) {
    CCLog(("PurchaseHandler::itemPurchased:"+itemId).c_str());
}
void PurchaseHandler::itemPurchaseStarted() {
    CCLog("PurchaseHandler::itemPurchaseStarted()");
}
void PurchaseHandler::openingStore() {
    CCLog("PurchaseHandler::openingStore()");
}

void PurchaseHandler::marketPurchaseStarted(string& itemId) {
    CCLog(("PurchaseHandler::marketPurchaseStarted:"+itemId).c_str());
}
void PurchaseHandler::marketRefund(string& itemId) {
    CCLog(("PurchaseHandler::marketRefund:"+itemId).c_str());
}

void PurchaseHandler::restoreTransactionsStarted() {
    CCLog("PurchaseHandler::restoreTransactionsStarted");
}
void PurchaseHandler::unexpectedErrorInStore() {
    CCLog("PurchaseHandler::unexpectedErrorInStore");
    //BackgroundHolder::storeClose();
}

#endif



