#ifndef NOTIFICATION_H
#define NOTIFICATION_H
class Notification
{
public:
    enum Type
    {
        RestorePurchasesOk = 100,
        RestorePurchasesFail = 200
    };

    static void notify(const Type);
};

#endif // NOTIFICATION_H
