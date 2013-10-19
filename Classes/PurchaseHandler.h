#ifndef JUNIOR

#ifndef __EventHandler__
#define __EventHandler__

#include <string>
#include "StoreBridge/cocos2dx_EventHandlers.h"

using namespace std;

class PurchaseHandler : public IEventHandler {
public:
    void billingSupported();
    void billingNotSupported();
	void closingStore();
	void currencyBalanceChanged(string &itemId, int balance, int amountAdded);
	void goodBalanceChanged(string &itemId, int balance, int amountAdded);
    void goodEquipped(string& itemId);
    void goodUnequipped(string& itemId);
    void goodUpgrade(string& itemId, string& upgradeItemId);
	void itemPurchased(string& itemId);
	void itemPurchaseStarted();
    void openingStore();
	void marketPurchaseCancelled(string& itemId);
    void marketPurchase(string& itemId);
    void marketPurchaseStarted(string& itemId);
    void marketRefund(string& itemId);
    void restoreTransactions(bool success);
	void restoreTransactionsStarted();
    void unexpectedErrorInStore();
private:

};

#endif

#endif
