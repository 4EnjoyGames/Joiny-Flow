#ifndef STORE_H
#define STORE_H
#include <string>
class Store
{
public:
    static std::string ITEM_UNLOCK_ALL;
    static std::string ITEM_BUY_ALL;
    static std::string PREFIX_BUY_COLLECTION;
    static std::string PREFIX_BUY_HINTS;
    static bool buyItem(const std::string& item);
    static void restorePurchases();
private:
};

#endif // STORE_H
