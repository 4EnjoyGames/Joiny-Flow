#include "Scenes/SelectCollection.h"
#include "Logic/LevelManager.h"
#include "Logic/RW.h"
#include "Scenes/SelectLevel.h"
#include "Scenes/MainScene.h"
#include "BackButton.h"
#include "Localization/CCLocalizedString.h"
#include <ADLib/Device/ADAds.h>
#include <ADLib/ADString.h>
#include "Core/Fonts.h"
#include "Core/Screen.h"
#include <ADLib/Device/ADInApp.h>
#include <ADLib/Rendering/ADBMFont.h>
class SelectCollection::BuyFullVerdionPopUp : public PopUpWindow::Content
{
public:

    BuyFullVerdionPopUp(
            SelectCollection* parent=0)
        : _parent(parent)
    {}
private:
    typedef BuyFullVerdionPopUp Me;
    SelectCollection* _parent;
    void onBuy(CCObject*)
    {
        //TODO: add functions
        ADInApp::buyProduct("unlock_full");
    }

    void onCancle(CCObject*)
    {
        this->closeWindow();
    }


    void onCreate(CCNode *parent)
    {

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;
        float vertical = size.height * 0.25f;

        CCLabelTTF* label = CCLabelTTF::create(_("SelectColection.BuyFullVerdionPopUp.BuyTitle"),
                                               Fonts::getFontName(),
                                               62);
        label->setFontSize(48);
        label->setPosition(ccp(x_middle, size.height*0.85f));
        parent->addChild(label);

        //add 3 stars
        SpritesLoader spl = GraphicsManager::getLoaderFor(parent,
                                                          "level-end/big_stars.plist",
                                                          "level-end/big_stars.png");
        spl->inject();
        CCSprite* stars_spr = spl->loadSprite("big_stars_3.png");
        stars_spr->setScale(0.75f);
        stars_spr->setPosition(ccp(x_middle, size.height*0.65f));


        //add "or" text
        CCLabelTTF* or_text = CCLabelTTF::create(_("SelectColection.BuyFullVerdionPopUp.OrText"),
                                               Fonts::getFontName(),
                                               40);
        or_text->setPosition(ccp(x_middle, size.height*0.5f));
        parent->addChild(or_text);

        /**
         * @brief menu_spl
         */
        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                               "collection-menu/collection_button.plist",
                                                               "collection-menu/collection_button.png");
        MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
        menu->setPosition(ccp(0,0));
        menu->setAnchorPoint(ccp(0,0));
        menu->setContentSize(size);
        parent->addChild(menu);

        CCSprite* parent_rgb = (CCSprite*)parent->getChildByTag(123);
        if(parent_rgb)
            parent_rgb->setColor(GameInfo::getInstance()->getNegativeColor());

        CCSprite* coll_button = menu_spl->loadSprite("collection_button.png");

        //coll_button->setScale(coll_button->getContentSize().width/
        //           parent->getContentSize().width*0.6);
        AnimatedMenuItem *buy_item = AnimatedMenuItem::create(
                    coll_button,
                    this, menu_selector(Me::onBuy));
        buy_item->setPosition(ccp(size.width*0.5,
                                  vertical));
        buy_item->setBaseScale(coll_button->getContentSize().width/
                               parent->getContentSize().width*0.8);

        std::string text = _("SelectColection.BuyFullVerdionPopUp.Yes");
        std::string price = ADInApp::getProduct("unlock_full")->getPrice();
        std::string button_buy_text = text +'\n' + price;
        CCLabelTTF * buy_text = CCLabelTTF::create(button_buy_text.c_str(),
                                                          Fonts::getFontName(),
                                                          45);
        buy_text->setColor(ccc3(255,255,255));
        buy_text->setPosition(ccp(buy_item->getContentSize().width/2,
                                  buy_item->getContentSize().height/2));
        buy_item->addChild(buy_text);
        menu->menu()->addChild(buy_item);
    }
};


SelectCollection::SelectCollection():
    _pop_up_manager(this)
{
}
cocos2d::CCScene* SelectCollection::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SelectCollection *layer = SelectCollection::create();

    // add layer as a child to scene
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(SelectCollection::onKeyBackClicked));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}
SelectCollection* SelectCollection::create()
{
    SelectCollection *pRet = new SelectCollection();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = 0;
        return 0;
    }
}

void SelectCollection::onCollectionSelect(CCObject* sender)
{
    AnimatedMenuItem* tile = dynamic_cast<AnimatedMenuItem*>(sender);

    ButtonsMap::iterator it = _buttons_map.find(tile);
    if(it != _buttons_map.end())
    {
        const JoinyCollection* selected = it->second;


        _last_selected_collection = selected;

        //if the collection is opened - open it
        //else show buy window
        if(_last_selected_collection->isOpenCollection())
        {
            hideEverything(
                        CCCallFunc::create(
                            this,
                            callfunc_selector(SelectCollection::doOpenCollection)));
        }
        else
        {
            _pop_up_manager.openWindow(new BuyFullVerdionPopUp(this));
        }

    }
}
AnimatedMenuItem* SelectCollection::createStars(AnimatedMenuItem* item,
                                                const JoinyCollection* collection)
{
    float SCALE = Screen::getScaleFactor();

    unsigned int min_stars_num = RW::getLevelManager().getCollectionMinStars(collection);



    CCSprite* stars_spr = 0;

    if(min_stars_num == 0)
        stars_spr = _stars_spl->loadSprite("big_stars_0.png");
    else if(min_stars_num == 1)
        stars_spr = _stars_spl->loadSprite("big_stars_1.png");
    else if(min_stars_num == 2)
        stars_spr = _stars_spl->loadSprite("big_stars_2.png");
    else
        stars_spr = _stars_spl->loadSprite("big_stars_3.png");

    //CCPoint position = item->getPosition();
    stars_spr->setPosition(item->getPosition());
    ADBMFont::setPositionByChangingAnchorPoint(
                stars_spr,
                ccp(item->getPositionX(),
                    item->getPositionY() - _item_size.height*0.5));

    stars_spr->setScale(stars_spr->getContentSize().width/
                        stars_spr->getContentSize().width*0.85);
    item->addNephew(stars_spr);

    return item;
}

AnimatedMenuItem* SelectCollection::createCollectionItem(
        const JoinyCollection* collection, const SpritesLoader& spl)
{
    std::string collection_name = collection->getCollectionName();

    ccColor3B label_color = collection->getCollectionColor();

    static ccColor3B closeLevel = GameInfo::getInstance()->getCloseColor();
    ccColor3B working = closeLevel;
    if(collection->isOpenCollection())
    {
        working = label_color;
    }
    else if (isFreeOpenFullGame() || GameInfo::getVersion() == Version::Premium)
    {
        RW::getLevelManager().makeFullGameVersion();
        working = label_color;
    }

    CCSprite* background = spl->loadSprite("collection_button.png");
    background->setColor(working);
    AnimatedMenuItem* item = AnimatedMenuItem::create(
                background,
                this,
                menu_selector(SelectCollection::onCollectionSelect));

    unsigned int l_num = collection->getLevelsNumber();
    std::string l_num_str = AD_to_string(l_num);

    unsigned int l_curr = collection->getCompletedLevelsNumber();
    std::string l_curr_str = AD_to_string(l_curr);


    std::string label_text = collection_name + "  "+ l_curr_str + '/'  + l_num_str ;
    CCLabelTTF* label = CCLabelTTF::create(label_text.c_str(),
                                           Fonts::getFontName(),
                                           75);
    label->setTag(123);
    item->addChild(label);

    _item_size = background->getContentSize();

    float scale = MIN(1, background->getContentSize().width * 0.7 / label->getContentSize().width);
    _scale = scale;

    label->setPosition(ccp(background->getContentSize().width*0.5,
                           background->getContentSize().height*0.5));
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setScale(scale);
    label->setColor(working);

    return item;
}
void SelectCollection::onEnter()
{
    _last_scene = this;
    DrawLayer::onEnter();
}

void SelectCollection::onExit()
{
    _last_scene = 0;
    DrawLayer::onExit();
}

bool SelectCollection::init()
{
    if (!DrawLayer::init())
    {
        return false;
    }
    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    _collections = CCLabelTTF::create( _("Collection"),
                                                   Fonts::getFontName(),
                                                   72);

    auto button_show = [](){return CCFadeTo::create(0.1f, 255);};
    _collections->setOpacity(0);
    _collections->runAction(button_show());


    _collections->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                               ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE));
    _collections->setColor(GameInfo::getInstance()->getTitleColor());
    this->addChild(_collections);


    //Back Button
    _back_menu = _back.start(this, [this](){this->onButtonBackClicked(0);});
    this->addChild(_back_menu);


    //To trigger back button
    this->setKeypadEnabled(true);


    //Create menu with collections
    _col_spl = GraphicsManager::getLoaderFor(
                0,
                "collection-menu/collection_button.plist",
                "collection-menu/collection_button.png");




    _buttons_menu = MenuSpriteBatch::create(_col_spl);


    //unsigned int i = 0;
    unsigned int in_row = 1;

    CCSprite* image = _col_spl->loadSprite("collection_button.png");

    CCSize s = image->getContentSize();
    image->removeFromParent();

    float margin = s.width * 0.15f;
    float margin_vertical = s.width * 0.09f;
    float width = s.width*in_row + margin*(in_row - 1);

    unsigned int rows = GameInfo::getInstance()->getCollectionNumber();
    float height = rows * s.height + margin*(rows - 1);

    _buttons_menu->setContentSize(CCSize(width+margin*2, height+margin*2));

    _stars_spl = GraphicsManager::getLoaderFor(_buttons_menu,
                                               "level-end/big_stars.plist",
                                               "level-end/big_stars.png");
    _stars_spl->inject();

    unsigned int collection_id_first = 0;

    unsigned int* order = nullptr;

    unsigned int arr_premium[10] = {1,2,3,10,4,9,5,8,6,7};
    unsigned int arr_normal[10] = {1,2,3,4,5,6,7,8,9,10};
    if(GameInfo::getVersion() == Version::Premium)
    {
        
        order = arr_premium;
    }
    else
    {
        
        order = arr_normal;
    }

    float working_y = height-s.height/2+margin;
    for(unsigned int y=0; y<rows; ++y)
    {
        float working_x = s.width / 2 + margin;

        const JoinyCollectionID coll_id = order[collection_id_first];
        const JoinyCollection* l = RW::getLevelManager().getCollection(coll_id);

        AnimatedMenuItem* item = createCollectionItem(l, _col_spl);
        _buttons_menu->menu()->addChild(item, 100);

        item->setPosition(ccp(working_x, working_y));

        item = createStars(item,l);

        _buttons_map[item] = l;

        working_x += s.width + margin;
        collection_id_first++;
        working_y -= s.height + margin_vertical;
    }



    newScrolling(_buttons_menu);

    for(auto& i:_buttons_map)
    {
        AnimatedMenuItem* item = i.first;

        //animation
        float scale_button = item->getScale();
        item->setScale(scale_button*0.9);
        item->setAnchorPoint(ccp(0.5, 0.5));
        item->runAction(CCEaseElasticOut::create(
                                  CCScaleTo::create(0.7f, scale_button),
                                  0.4f));
    }

    return true;
}

void SelectCollection::keyBackClicked()
{
    if(!_pop_up_manager.backAction())
    {
        this->hideEverything(CCCallFunc::create(
                                 this, callfunc_selector(SelectCollection::doGoBack)));

    }
}

void SelectCollection::doGoBack()
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}

void SelectCollection::doOpenCollection()
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene(_last_selected_collection));
}

void SelectCollection::hideEverything(cocos2d::CCCallFunc *callback)
{   
    float duration = 0.15f;
    CCFadeTo* logo_move = CCFadeTo::create(duration, 0);
    _collections->runAction(logo_move);
    auto button_hide = [](){return CCFadeTo::create(0.15f, 0);};

    for(auto & i: _buttons_map)
    {
        AnimatedMenuItem* item = i.first;
        item->runAction(button_hide());
    }


    float delay = 0.2;
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    callback,
                    NULL));
}

void SelectCollection::newScrolling(MenuSpriteBatch* menu)
{
    //Get the size of the screen we can see
    CCSize visibleSize = Screen::getVisibleSize();
    float scaled = Screen::getScaleFactor();

    //Get the screen start of cordinates
    CCPoint origin = Screen::getOrigin();

    const float TOP_MARGIN = 140/scaled;
    const float BOTTOM_MARGIN = 30/scaled;


    float menu_left_padding = 0;
    float min_height = 1000/scaled;
    CCSize menu_size = menu->getContentSize();

    float collection_height = menu_size.height;
    if(collection_height < min_height)
    {
        collection_height = min_height;
        menu_left_padding = (min_height - menu_size.height) / 2;
    }
    //Get the scroll area size
    float scroll_view_width = visibleSize.width - TOP_MARGIN * 0.2;
    float scroll_view_height = visibleSize.height-TOP_MARGIN - BOTTOM_MARGIN;

    CCSize scroll_view_size(scroll_view_width, scroll_view_height);

    //Create layer to fit all tiles
    CCLayer *layer = CCLayer::create();
    layer->setContentSize(CCSize(menu_size.width, menu_size.height));

    //Create scroll view for this layer
    CCScrollView* _collections_scroll_view = CCScrollView::create(
                scroll_view_size,layer);



    CCPoint collections_target_position(
                origin.x + visibleSize.width/2 - scroll_view_size.width/2,
                origin.y + BOTTOM_MARGIN);


    float scale = scroll_view_width/menu_size.width;
    layer->setScale(scale);

    _collections_scroll_view->setPosition(collections_target_position);
    _collections_scroll_view->setAnchorPoint(ccp(0.5, 0));
    //CCRect eat_zone(origin.x,origin.y, visibleSize.width, _collections_scroll_view->getPositionY() + scroll_view_height);
    CCRect eat_zone(origin.x,origin.y,
                    visibleSize.width,
                    visibleSize.height);
    _collections_scroll_view->setTouchEatZone(eat_zone);
    _collections_scroll_view->updateInset();
    _collections_scroll_view->setDirection(kCCScrollViewDirectionVertical);
    _collections_scroll_view->setContentOffset(_collections_scroll_view->minContainerOffset());
    this->addChild(_collections_scroll_view);

    //Add our tiles to scroll area
    _collections_scroll_view->addChild(menu);
    _collections_scroll_view->setMenu(menu->menu());
    _collections_scroll_view->addHighPriorityTouchListener(_back_menu);
    // this->addChild(menu);
    menu->setAnchorPoint(ccp(0,0));
    menu->setPosition(ccp(0,
                          0));

    _pop_up_manager.addOnHideWindowAction([_collections_scroll_view](){
        _collections_scroll_view->setTouchEnabled(true);
    });
    _pop_up_manager.addOnShowWindowAction([_collections_scroll_view](){
        _collections_scroll_view->setTouchEnabled(false);
    });
    _pop_up_manager.addMenuToAutoDisable(menu->menu());

}
SelectCollection* SelectCollection::_last_scene = 0;
void SelectCollection::purchaseUpdateFullGame()
{
    if(_last_scene)
    {
        updateItems();
        _last_scene->_pop_up_manager.backAction();
    }
}
void SelectCollection::updateItems()
{
    for(auto & i: (_last_scene->_buttons_map) )
    {
        AnimatedMenuItem* item = i.first;
        const JoinyCollection* col = i.second;
        ccColor3B coll_color = col->getCollectionColor();

        CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(item->getChildByTag(123));
        if(label)
        {
            label->setColor(coll_color);
        }

        item->setColor(coll_color);
    }
}
/**
 * @brief SelectCollection::isFreeOpenFullGame
 *if the player has min 3 stars in all Collections
 *return true - and open full game
 * @return
 */
bool SelectCollection::isFreeOpenFullGame()
{
    bool result = true;
    unsigned int coll_num = GameInfo::getInstance()->getCollectionNumber();
    unsigned int min_stars = 0;

    for(unsigned int i=1; i<=coll_num; ++i)
    {
        const JoinyCollection* col = RW::getLevelManager().getCollection(i);
        if(col->isOpenCollection())
        {
            min_stars = RW::getLevelManager().getCollectionMinStars(col);
            if(min_stars<3)
            {
                result = false;
                break;
            }
        }
    }

    return result;
}
