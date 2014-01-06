#include "Scenes/SelectCollection.h"
#include "Logic/LevelManager.h"
#include "Logic/RW.h"
#include "Scenes/SelectLevel.h"

SelectCollection::SelectCollection()
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
        hideEverything(
                    CCCallFunc::create(
                        this,
                        callfunc_selector(SelectCollection::doOpenCollection)));


    }
}
AnimatedMenuItem* SelectCollection::createCollectionItem(
        const JoinyCollection* collection, const SpritesLoader& spl)
{
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();

    std::stringstream collection_number;
    collection_number << collection->getCollectionID();

    static ccColor3B working(ccc3(255,242,28));

    CCSprite* background = spl->loadSprite("level_button.png");
    background->setColor(working);
    AnimatedMenuItem* item = AnimatedMenuItem::create(
                    background,
                    this,
                    menu_selector(SelectCollection::onCollectionSelect));

    CCLabelTTF* label = CCLabelTTF::create(collection_number.str().c_str(), "Arial", 60/scaled);
    item->addChild(label);

    float scale = MIN(1, background->getContentSize().width * 0.7 / label->getContentSize().width);
    label->setPosition(ccp(background->getContentSize().width/2-3*scale, background->getContentSize().height/2));
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setScale(scale);
    label->setColor(working);

    return item;
}
bool SelectCollection::init()
{
    if (!CCLayerColor::initWithColor(ccc4(231, 232, 232, 255)))
    {
        return false;
    }
    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();

    CCMenu* main_menu = CCMenu::create();
    main_menu->setPosition(ORIGIN);

    CCLabelTTF * collections = CCLabelTTF::create("Collections","Arial",24);
    collections->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + VISIBLE_SIZE.height*0.75));
    this->addChild(collections);
    this->addChild(main_menu);


    //To trigger back button
    this->setKeypadEnabled(true);


    //Get the screen start of cordinates
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();



    //Create menu with collections


    _col_spl = GraphicsManager::getLoaderFor(
                0,
                "level_buttons.plist",
                "level_buttons.png");
    _buttons_menu = MenuSpriteBatch::create(_col_spl);


    //unsigned int i = 0;
    unsigned int in_row = 1;

    CCSprite* image = _col_spl->loadSprite("level_button.png");

    CCSize s = image->getContentSize();
    image->removeFromParent();

    float margin = s.width * 0.15f;
    float width = s.width*in_row + margin*(in_row - 1);

    unsigned int rows = 4;
    float height = rows * s.height + margin*(rows - 1);;

    _buttons_menu->setContentSize(CCSize(width+margin*2, height+margin*2));


    float working_y = height-s.height/2+margin;
    for(unsigned int y=0; y<rows; ++y)
    {
        float working_x = s.width / 2 + margin;
        for(unsigned int x=0; x<in_row; ++x)
        {
            //JoinyLevelID id = y*in_row + x + 1;
            const JoinyCollectionID coll_id = 1;y*in_row + x + 1;
            //if(id <= _current_collection->getLevelsNumber())
            //{
                const JoinyCollection* l = RW::getLevelManager().getCollection(coll_id);

                AnimatedMenuItem* item = createCollectionItem(l, _col_spl);
                _buttons_menu->menu()->addChild(item, 100);

                item->setPosition(ccp(working_x, working_y));

                _buttons_map[item] = l;

                working_x += s.width + margin;
            //}

        }
        working_y -= s.height + margin;
    }

    //_buttons_menu->setAnchorPoint(ccp(0, 0));
    //_buttons_menu->setPosition(origin.x, origin.y);
    //this->addChild(_buttons_menu);
    newScrolling(_buttons_menu);

    return true;
}

void SelectCollection::keyBackClicked()
{
    hideEverything(
                CCCallFunc::create(
                    this,
                    callfunc_selector(SelectCollection::doGoBack)));
}

void SelectCollection::doGoBack()
{
    CCDirector::sharedDirector()->end();
}

void SelectCollection::doOpenCollection()
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene(_last_selected_collection));
}

void SelectCollection::hideEverything(cocos2d::CCCallFunc *callback)
{
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(0),
                    callback,
                    NULL));
}

void SelectCollection::buildCollectionTiles()
{

}

void SelectCollection::newScrolling(MenuSpriteBatch* menu)
{
    //Get the size of the screen we can see
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();

    //Get the screen start of cordinates
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

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
    float scroll_view_width = visibleSize.width - 50/scaled;
    float scroll_view_height = visibleSize.height-150/scaled;
    CCSize scroll_view_size(scroll_view_width, scroll_view_height);

    //Create layer to fit all tiles
    CCLayer *layer = CCLayer::create();
    layer->setContentSize(CCSize(menu_size.width, menu_size.height));

    //Create scroll view for this layer
    CCScrollView* _collections_scroll_view = CCScrollView::create(
                scroll_view_size,layer);



    CCPoint collections_target_position(
                origin.x + visibleSize.width/2 -
                scroll_view_size.width/2,
                origin.y + visibleSize.height/2 -
                scroll_view_size.height/2);

    float scale = scroll_view_width/menu_size.width;
    layer->setScale(scale);

    _collections_scroll_view->setPosition(collections_target_position);
    CCRect eat_zone(origin.x,origin.y, visibleSize.width, _collections_scroll_view->getPositionY() + scroll_view_height);
    _collections_scroll_view->setTouchEatZone(eat_zone);
    _collections_scroll_view->updateInset();
    _collections_scroll_view->setDirection(kCCScrollViewDirectionVertical);
    _collections_scroll_view->setContentOffset(_collections_scroll_view->minContainerOffset());
    this->addChild(_collections_scroll_view);

    //Add our tiles to scroll area
    _collections_scroll_view->addChild(menu);
    _collections_scroll_view->setMenu(menu->menu());

   // this->addChild(menu);
    menu->setAnchorPoint(ccp(0,0));
    menu->setPosition(ccp(0,
                          0));

}
