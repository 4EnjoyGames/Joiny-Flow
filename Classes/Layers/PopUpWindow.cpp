#include "PopUpWindow.h"
#include "Core/Screen.h"
#include "cocos2d-A.h"
using namespace cocos2d;

void PopUpWindow::closeWindow(CCObject* obj_callback,
                 cocos2d::SEL_CallFunc callfunc_callback)
{

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();


    _content->perform_close();
    this->runAction(
                CCSequence::create(
                    CCCallFunc::create(_close_callback_obj,
                                       _close_callback_fun),
                    CCMoveTo::create(
                        0.3f,
                        ccp(_sheet_target_position.x,
                            _sheet_target_position.y + visibleSize.height)),

                    CCCallFunc::create(obj_callback,
                                       callfunc_callback),
                    CCCallFunc::create(this,
                                       callfunc_selector(PopUpWindow::cleanUp)),
                    NULL));

}
void PopUpWindow::cleanUp()
{
    this->removeFromParent();
}

PopUpWindow* PopUpWindow::create(
        Content* content,
        CCObject* obj_close_callback,
        cocos2d::SEL_CallFunc callfunc_close_callback)
{
    PopUpWindow* obj = new PopUpWindow(content, obj_close_callback, callfunc_close_callback);
    if(obj)
    {
        obj->autorelease();
    }
    return obj;
}
PopUpWindow::~PopUpWindow()
{
    if(_content)
        _content->release();
}


PopUpWindow::PopUpWindow(Content* content,
            CCObject* obj_close_callback,
            cocos2d::SEL_CallFunc callfunc_close_callback)
    : _content(content), _close_callback_obj(obj_close_callback), _close_callback_fun(callfunc_close_callback)
{
    _content->autorelease();
    _content->retain();


}

void PopUpWindow::showWindow(bool vertical)
{
    initWindow(vertical);
}

bool PopUpWindow::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent * /* pEvent */)
{
    CCPoint touchLocation = pTouch->getLocation();
    CCPoint local_cords = this->convertToNodeSpace(touchLocation);

    CCRect r = this->boundingBox();
    r.origin = CCPointZero;


    if (!r.containsPoint(local_cords))
    {
        return true;
    }
    return false;
}

void PopUpWindow::ccTouchMoved(cocos2d::CCTouch *, cocos2d::CCEvent *)
{}
void PopUpWindow::ccTouchEnded(cocos2d::CCTouch *, cocos2d::CCEvent *)
{
    closeWindow();
}

void PopUpWindow::ccTouchCancelled(cocos2d::CCTouch *, cocos2d::CCEvent *)
{}

void PopUpWindow::initWindow(bool vertical)
{
    this->setTouchEnabled(true);
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, false);
    const CCPoint origin = Screen::getOrigin();
    const CCSize visibleSize = Screen::getVisibleSize();

    _sheet_menu = this;

    CCSprite* sheet = CCSprite::create("pop-up/background.png");
    this->addChild(sheet, 0, 123);


    //Create the zone for menu
    CCSize menu_zone_size;
    if(vertical)
    {
        menu_zone_size = CCSize(visibleSize.width*0.8,
                          visibleSize.width*1.0);
    }
    else
    {
        menu_zone_size = CCSize(visibleSize.width*0.9,
                          visibleSize.width*0.6);
    }

    CCSize sheet_size = sheet->getContentSize();

    _sheet_target_position = ccp(visibleSize.width/2+origin.x - menu_zone_size.width/2,
                                 visibleSize.height/2+origin.y - menu_zone_size.height/2);
    _sheet_menu->setPosition(ccp(_sheet_target_position.x,
                                 _sheet_target_position.y-visibleSize.height));
    _sheet_menu->setContentSize(menu_zone_size);
    _sheet_menu->setAnchorPoint(ccp(0,0));
    _sheet_menu->stopAllActions();
    _sheet_menu->runAction(CCMoveTo::create(0.2f, _sheet_target_position));

//        sheet->setScaleX(menu_zone_size.width / sheet_size.width);
//        sheet->setScaleY(visibleSize.width*0.9 / sheet_size.height);

    sheet->setScaleX(menu_zone_size.width / sheet_size.width);
    sheet->setScaleY(menu_zone_size.height / sheet_size.height);
    sheet->setAnchorPoint(ccp(0,0));

    _content->perform_init(_sheet_menu, this);
}

PopUpWindow::Content::Content()
    : _parent_window(0)
{

}

void PopUpWindow::Content::closeWindow(
        CCObject* obj_callback, cocos2d::SEL_CallFunc callfunc_callback)
{
    if(_parent_window)
        _parent_window->closeWindow(obj_callback, callfunc_callback);
}


void PopUpWindow::Content::perform_init(cocos2d::CCNode* parent,
                  PopUpWindow* window)
{
    _parent_window = window;
    onCreate(parent);
}

void PopUpWindow::Content::perform_close()
{
    onClose();
}

PopUpWindowManager::PopUpWindowManager(cocos2d::CCNode* parent)
    : _parent(parent), _opened_window(0), _pending_window(0)
{

}

void PopUpWindowManager::closeWindow(cocos2d::CCObject* obj_callback,
                 cocos2d::SEL_CallFunc callfunc_callback)
{
    if(_opened_window)
    {
        _opened_window->closeWindow(obj_callback, callfunc_callback);

        _opened_window = 0;
    }


}

PopUpWindow* PopUpWindowManager::openWindow(PopUpWindow::Content* content,
                                            bool vertical)
{
    PopUpWindow* window = PopUpWindow::create(
                content,
                this,
                callfunc_selector(PopUpWindowManager::onLastClosed));
    window->retain();

    if(_opened_window)
    {
        closeWindow();
        _pending_window = _opened_window;
        _opened_window = 0;

    }


    do_openWindow(window,vertical);
    return window;
}

 void PopUpWindowManager::do_openWindow(PopUpWindow* window,
                                        bool vertical_mode)
 {
     if(_opened_window == nullptr)
         runOnShowWindowActions();

     setMenusAvaliablitity(false);
     _opened_window = window;
     window->showWindow(vertical_mode);
     _parent->addChild(window);
     window->release();
 }

void PopUpWindowManager::onLastClosed()
{
    setMenusAvaliablitity(true);
    if(_pending_window == 0)
    {
        _opened_window = 0;
        runOnHideWindowActions();
    }
    _pending_window = 0;
}
void PopUpWindowManager::addMenuToAutoDisable(cocos2d::CCMenu* menu)
{
    _menus.push_back(menu);
}
void PopUpWindowManager::addScrollViewToAutoDisable(cocos2d::CCScrollView* scroll_view)
{
    _scroll_views.push_back(scroll_view);
}

void PopUpWindowManager::setMenusAvaliablitity(bool enabled)
{
    for(unsigned int i=0; i<_menus.size(); ++i)
    {
        CCMenu* m = _menus[i];
        m->setEnabled(enabled);
    }
    for(unsigned int i=0; i<_scroll_views.size(); ++i)
    {
        CCScrollView* m = _scroll_views[i];
        m->setTouchEnabled(enabled);
    }

}

bool PopUpWindowManager::backAction()
{
    if(_opened_window)
    {
        closeWindow();
        return true;
    }
    return false;
}

void PopUpWindowManager::addOnShowWindowAction(const Action& action)
{
    _show_window_action.push_back(action);
}

void PopUpWindowManager::addOnHideWindowAction(const Action& action)
{
    _hide_window_action.push_back(action);
}

void PopUpWindowManager::runOnShowWindowActions()
{
    for(auto& it : _show_window_action)
        it();
}

void PopUpWindowManager::runOnHideWindowActions()
{
    for(auto& it : _hide_window_action)
        it();
}
