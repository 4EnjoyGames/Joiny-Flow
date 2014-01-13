#include "Scenes/SelectCollection.h"
#include "Logic/LevelManager.h"
#include "Logic/RW.h"
#include "Scenes/SelectLevel.h"
#include "Scenes/MainScene.h"
#include "BackButton.h"
#include "Localization/CCLocalizedString.h"


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

    std::stringstream collection_name;
    collection_name << collection->getCollectionName();

    //static ccColor3B working(ccc3(255,242,28));
    Color label_color = collection->getCollectionColor();

    CCSprite* background = spl->loadSprite("collection_button.png");
    background->setColor(ccc3(label_color.red(),label_color.green(),label_color.blue()));
    AnimatedMenuItem* item = AnimatedMenuItem::create(
                    background,
                    this,
                    menu_selector(SelectCollection::onCollectionSelect));

    CCLabelTTF* label = CCLabelTTF::create(collection_name.str().c_str(), "Arial", 60/scaled);
    item->addChild(label);

    float scale = MIN(1, background->getContentSize().width * 0.7 / label->getContentSize().width);
    label->setPosition(ccp(background->getContentSize().width/2-3*scale, background->getContentSize().height/2));
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setScale(scale);


    label->setColor(ccc3(label_color.red(),label_color.green(),label_color.blue()));
    //label->setColor(working);

    return item;
}
bool SelectCollection::init()
{
    if (!CCLayer::init() )
    {
        return false;

    }
//    if (!CCLayerColor::initWithColor(ccc4(231, 232, 232, 255)))
//    {
//        return false;
//    }
    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();

    CCMenu* main_menu = CCMenu::create();
    main_menu->setPosition(ccp(0,0));


//    //Load one piece
//    CCSprite* sp_noise = CCSprite::create("main-menu/back.png");

//    //Take the texture from sprite
//    CCTexture2D *texture = sp_noise->getTexture();

//    //Set parameters GL_MIRRORED_REPEAT mean that texture should repeat one time mirrored other time not
//    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT};
//    texture->setTexParameters(&params);

//    //Final sprite
//    CCSprite *noise = CCSprite::createWithTexture(texture, CCRectMake(0, 0, VISIBLE_SIZE.width, VISIBLE_SIZE.height));
//    noise->setPosition(ccp(ORIGIN.x +  VISIBLE_SIZE.width/2,
//                           ORIGIN.y + VISIBLE_SIZE.height/2));
//    this->addChild(noise);

    CCLabelTTF * collections = CCLabelTTF::create( CCLocalizedString("Collection").c_str(),"fonts/Fredoka One.ttf",72);
    collections->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + VISIBLE_SIZE.height - 100/SCALE));
    collections->setColor(ccc3(11,216,224));
    this->addChild(collections);


    //Back Button
//    CCSprite* back_logo = CCSprite::create("back_button.png");
//    AnimatedMenuItem* back_button= AnimatedMenuItem::create(
//                back_logo, this, menu_selector(SelectCollection::onButtonBackClicked));
//    back_logo = CCSprite::create("back_button.png");

//    CCPoint position_bb(ORIGIN.x + 120/SCALE,
//                        ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE);
//    back_button->setPosition(position_bb);
//    back_button->setAnchorPoint(ccp(1, 0.5));

//    back_button->addChild(back_logo);
//    back_logo->setPosition(ccp(back_logo->getContentSize().width/2,
//                              back_logo->getContentSize().height/2));


//    main_menu->addChild(back_button);

    CCMenu* menu = back.start(this, [this](){this->onButtonBackClicked(0);});
    this->addChild(menu);


    //coll button
//    CCSize back_size = back_logo->getContentSize();
//    CCSprite* coll_title = CCSprite::create("collection-menu/collections.png");
//    coll_title->setPosition(ccp(ORIGIN.x + back_size.width + (VISIBLE_SIZE.width - back_size.width) * 0.5,
//                                ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE));
//    setAnchorPoint(ccp(0.5f, 1));
//    this->addChild(coll_title);


    //To trigger back button
    this->setKeypadEnabled(true);


    //Create menu with collections
    _col_spl = GraphicsManager::getLoaderFor(
                0,
                "collection_button.plist",
                "collection_button.png");


    _buttons_menu = MenuSpriteBatch::create(_col_spl);


    //unsigned int i = 0;
    unsigned int in_row = 1;

    CCSprite* image = _col_spl->loadSprite("collection_button.png");

    CCSize s = image->getContentSize();
    image->removeFromParent();

    float margin = s.width * 0.15f;
    float width = s.width*in_row + margin*(in_row - 1);

    unsigned int rows = 2;
    float height = rows * s.height + margin*(rows - 1);;

    _buttons_menu->setContentSize(CCSize(width+margin*2, height+margin*2));


    unsigned int collection_id_first = 1;
    float working_y = height-s.height/2+margin;
    for(unsigned int y=0; y<rows; ++y)
    {
        float working_x = s.width / 2 + margin;

        const JoinyCollectionID coll_id = collection_id_first;
        const JoinyCollection* l = RW::getLevelManager().getCollection(coll_id);

        AnimatedMenuItem* item = createCollectionItem(l, _col_spl);
        _buttons_menu->menu()->addChild(item, 100);

        item->setPosition(ccp(working_x, working_y));

        _buttons_map[item] = l;

        working_x += s.width + margin;
        collection_id_first++;
        working_y -= s.height + margin;
    }

    //_buttons_menu->setAnchorPoint(ccp(0, 0));
    //_buttons_menu->setPosition(origin.x, origin.y);
    //this->addChild(_buttons_menu);
    newScrolling(_buttons_menu);

    //this->addChild(main_menu);
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
    //CCDirector::sharedDirector()->end();
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());

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
    float scroll_view_height = visibleSize.height-250/scaled;
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
    //CCRect eat_zone(origin.x,origin.y, visibleSize.width, _collections_scroll_view->getPositionY() + scroll_view_height);
        CCRect eat_zone(origin.x,origin.y, 0, 0);
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
