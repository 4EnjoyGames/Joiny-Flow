#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H
#include "cocos2d.h"
#include "Core/CCScrollView.h"
#include "Flow/FlowGame.h"

class PopUpWindow : public cocos2d::CCLayer
{
public:
    class Content;

    void closeWindow(CCObject* obj_callback = 0,
                     cocos2d::SEL_CallFunc callfunc_callback = 0);

    static PopUpWindow* create(
            Content* content,
            CCObject* obj_close_callback = 0,
            cocos2d::SEL_CallFunc callfunc_close_callback = 0);

    void showWindow(bool vertical=false);
    ~PopUpWindow();
private:
    void initWindow(bool vertical=false);

    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

    CCObject* _close_callback_obj;
    cocos2d::SEL_CallFunc _close_callback_fun;
    Content* _content;

    CCNode* _sheet_menu;
    cocos2d::CCPoint _sheet_target_position;

    void cleanUp();

    PopUpWindow(Content* content,
                CCObject* obj_close_callback = 0,
                cocos2d::SEL_CallFunc callfunc_close_callback = 0);
};

class PopUpWindowManager : public cocos2d::CCObject
{
public:
    PopUpWindowManager(cocos2d::CCNode* parent);
    void closeWindow(cocos2d::CCObject* obj_callback = 0,
                     cocos2d::SEL_CallFunc callfunc_callback = 0);
    PopUpWindow* openWindow(PopUpWindow::Content* content,
                            bool vertical=false);

    bool backAction();
    void addMenuToAutoDisable(cocos2d::CCMenu* menu);
    void addScrollViewToAutoDisable(cocos2d::CCScrollView* scroll_view);
    void setGameDisable(FlowGame* game);
private:
    void onLastClosed();
    void setMenusAvaliablitity(bool enabled);
    void do_openWindow(PopUpWindow* window,
                       bool vertical_mode=false);
    PopUpWindow* _opened_window;
    PopUpWindow* _pending_window;
    FlowGame* _game;

    std::vector<cocos2d::CCMenu*> _menus;
    std::vector<cocos2d::CCScrollView*> _scroll_views;
    cocos2d::CCNode* _parent;
};

class PopUpWindow::Content : public CCObject
{
public:
    Content();

    void closeWindow(CCObject* obj_callback = 0, cocos2d::SEL_CallFunc callfunc_callback = 0);

    virtual ~Content()
    {}
private:
    friend class PopUpWindow;
    PopUpWindow* _parent_window;

    void perform_init(cocos2d::CCNode* parent,
                      PopUpWindow* window);
    void perform_close();

    virtual void onCreate(cocos2d::CCNode* parent)=0;
    virtual void onClose()
    {}
};

#endif // POPUPWINDOW_H
