#ifndef YESNODIALOG_H
#define YESNODIALOG_H
#include "PopUpWindow.h"
#include "cocos2d-A.h"
class YesNoDialog : public PopUpWindow::Content
{
public:
    YesNoDialog(CCNode* content,
                CCObject* yes_callback,
                SEL_CallFunc yes_fun,
                CCObject* no_callback=0,
                SEL_CallFunc no_fun=0);
private:
    void onCreate(CCNode *parent);
    void yes(CCObject*);
    void no(CCObject*);
    CCNode* _content;

    CCObject* _yes_obj;
    SEL_CallFunc _yes_fun;
    CCObject* _no_obj;
    SEL_CallFunc _no_fun;
};

#endif // YESNODIALOG_H
