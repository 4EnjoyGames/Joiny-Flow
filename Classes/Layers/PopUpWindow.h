#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H
#include "cocos2d.h"
#include "Core/CCScrollView.h"

#include <functional>
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


    /**
     * @brief Type for planned actions
     */
    typedef std::function<void ()> Action;

    /**
     * @brief The given action will be performed just before first window is showed
     * @param action functor with action to perform
     */
    void addOnShowWindowAction(const Action& action);
    /**
     * @brief The action will be performed just after the last window was hidden
     * @param action functor with action to perform
     */
    void addOnHideWindowAction(const Action& action);
private:
    /**
     * @brief Runs all actions added by addOnShowWindowAction in the same
     * order as they were added
     */
    void runOnShowWindowActions();
    /**
     * @brief Runs all actions added by addOnHideWindowAction in the same
     * order as they were added
     */
    void runOnHideWindowActions();

    void onLastClosed();
    void setMenusAvaliablitity(bool enabled);
    void do_openWindow(PopUpWindow* window,
                       bool vertical_mode=false);
    PopUpWindow* _opened_window;
    PopUpWindow* _pending_window;

    std::vector<cocos2d::CCMenu*> _menus;
    std::vector<cocos2d::CCScrollView*> _scroll_views;
    cocos2d::CCNode* _parent;

    std::vector<Action> _show_window_action;
    std::vector<Action> _hide_window_action;
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
