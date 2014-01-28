#include "Logic/RW.h"
#include "SelectLevel.h"
#include "Logic/Language.h"
#include "Store.h"
#include "Core/Statistics.h"
#include "Scenes/LevelScene.h"
#include "Scenes/SelectCollection.h"
#include "Localization/CCLocalizedString.h"
#include "ADLib/Device/ADAds.h"
#include "ADLib/ADString.h"
SelectLevel::SelectLevel(const JoinyCollection *collection)
    :  _last_selected_level(nullptr), _current_collection(collection)
{
}

CCScene* SelectLevel::scene(const JoinyCollection *collection)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SelectLevel *layer = SelectLevel::create(collection);

    // add layer as a child to scene
    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(SelectLevel::onKeyBackClicked));
    BackgroundHolder::backgroundSwitchTo(scene,back);
    scene->addChild(layer);

    // return the scene
    return scene;
}

SelectLevel* SelectLevel::create(const JoinyCollection *collection)
{
    SelectLevel *pRet = new SelectLevel(collection);
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


void SelectLevel::keyBackClicked()
{

    hideEverything(
                CCCallFunc::create(
                    this,
                    callfunc_selector(SelectLevel::doGoBack)));

}
void SelectLevel::doGoBack()
{
    CCDirector::sharedDirector()->replaceScene(SelectCollection::scene());
}


void SelectLevel::onLevelSelect(CCObject* sender)
{
    AnimatedMenuItem* tile = dynamic_cast<AnimatedMenuItem*>(sender);

    ButtonsMap::iterator it = _buttons_map.find(tile);
    if(it != _buttons_map.end())
    {
        const JoinyLevel* selected = it->second;


        _last_selected_level = selected;
        hideEverything(
                    CCCallFunc::create(
                        this,
                        callfunc_selector(SelectLevel::doOpenLevel)));
    }

}



AnimatedMenuItem* SelectLevel::createLevelItem(const JoinyLevel* level,
                                               const SpritesLoader& spl)
{
    const float SCALE = Screen::getScaleFactor();
    JoinyLevelID joiny_level_id = level->getLevelId();


    ccColor3B openLevel = _current_collection->getCollectionColor();
    static ccColor3B closeLevel = GameInfo::getInstance()->getCloseColor();

    static ccColor3B labelColor(ccc3(255,255,255));

    ccColor3B working = openLevel;
    unsigned int stars = level->getStarsNumber(level->getHighScore());
    if(stars == 0)
        working = closeLevel;


    CCSprite* background = spl->loadSprite("level_button.png");
    background->setColor(working);
    AnimatedMenuItem* item = AnimatedMenuItem::create(
                    background,
                    this,
                    menu_selector(SelectLevel::onLevelSelect));

    CCLabelTTF* label = CCLabelTTF::create(AD_to_string(joiny_level_id).c_str(),
                                           "fonts/Fredoka One.ttf",
                                           60/SCALE);
    item->addChild(label);
    float scale = MIN(1, background->getContentSize().width * 0.7 / label->getContentSize().width);
    if(_scale==0)
        _scale  = scale;
    label->setPosition(ccp(background->getContentSize().width/2-3*SCALE,
                           background->getContentSize().height/2 + 10/SCALE));
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setScale(scale);
    label->setColor(labelColor);

    return item;
}
AnimatedMenuItem* SelectLevel::createStars(AnimatedMenuItem* item,
                              const JoinyLevel* level)
{
    const float SCALE = Screen::getScaleFactor();
    unsigned int stars = level->getStarsNumber(level->getHighScore());
    CCSprite* stars_spr = 0;
    if(stars>=1)
    {
        if(stars==1)
            stars_spr = _spl->loadSprite("stars_1.png");
        else if(stars==2)
            stars_spr = _spl->loadSprite("stars_2.png");
        else
            stars_spr = _spl->loadSprite("stars_3.png");
    }
    else
        stars_spr = _spl->loadSprite("stars_none.png");


    CCPoint position = item->getPosition();
    stars_spr->setPosition(ccp(position.x,
                               position.y - 40/SCALE));
    stars_spr->setScale(_scale);
    item->addNephew(stars_spr);

    return item;
}

bool SelectLevel::init()
{
//    if ( !CCLayer::init() )
//    {
//        return false;
//    }
    if (!DrawLayer::init())
    {
        return false;
    }

    const CCPoint ORIGIN = Screen::getOrigin();
    const CCSize VISIBLE_SIZE = Screen::getVisibleSize();
    const float SCALE = Screen::getScaleFactor();


    _scale = 0;
    //create collection name label
    ccColor3B openLevel = _current_collection->getCollectionColor();

    const std::string coll_name = _current_collection->getCollectionName();
    CCLabelTTF * collections = CCLabelTTF::create( coll_name.c_str(),"fonts/Fredoka One.ttf",72);
    collections->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width*0.5,
                          ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE));
    collections->setColor(openLevel);
    this->addChild(collections);


    //Back Button
    _back_menu = _back.start(this, [this](){this->onButtonBackClicked(0);});
    this->addChild(_back_menu);


    //To trigger back button
    this->setKeypadEnabled(true);


    //Create menu with collections
    _col_spl = GraphicsManager::getLoaderFor(
                0,
                "level-menu/level_buttons.plist",
                "level-menu/level_buttons.png");
    _buttons_menu = MenuSpriteBatch::create(_col_spl);

    _spl = GraphicsManager::getLoaderFor(_buttons_menu,
                                                      "level-menu/save_stars.plist",
                                                      "level-menu/save_stars.png");
    _spl->inject();


    unsigned int in_row = 5;
    CCSprite* image = _col_spl->loadSprite("level_button.png");
    CCSize s = image->getContentSize();
    image->removeFromParent();

    float margin = s.width * 0.15f;
    float width = s.width*in_row + margin*(in_row - 1);

    unsigned int rows = _current_collection->getLevelsNumber() / in_row;
    float height = rows * s.height + margin*(rows - 1);

    _buttons_menu->setContentSize(CCSize(width+margin*2, height+margin*2));

    float working_y = height-s.height/2+margin;
    for(unsigned int y=0; y<rows; ++y)
    {
        float working_x = s.width / 2 + margin;
        for(unsigned int x=0; x<in_row; ++x)
        {
            JoinyLevelID id = y*in_row + x + 1;
            if(id <= _current_collection->getLevelsNumber())
            {
                const JoinyLevel* l = _current_collection->getLevel(id);

                AnimatedMenuItem* item = createLevelItem(l, _col_spl);

                _buttons_menu->menu()->addChild(item, 100);

                item->setPosition(ccp(working_x, working_y));

                item = createStars(item,l);

                _buttons_map[item] = l;

                working_x += s.width + margin;
            }

        }
        working_y -= s.height + margin;
    }
    newScrolling(_buttons_menu);

    return true;
}

void SelectLevel::doOpenLevel()
{
    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_last_selected_level));
}

void SelectLevel::newScrolling(MenuSpriteBatch* menu)
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
    CCRect eat_zone(origin.x,origin.y,
                    visibleSize.width,
                    visibleSize.height);
    _collections_scroll_view->setTouchEatZone(eat_zone);
    _collections_scroll_view->updateInset();
    _collections_scroll_view->setDirection(kCCScrollViewDirectionVertical);
    _collections_scroll_view->setContentOffset(_collections_scroll_view->minContainerOffset());
    _collections_scroll_view->addHighPriorityTouchListener(_back_menu);
    this->addChild(_collections_scroll_view);

    //Add our tiles to scroll area
    _collections_scroll_view->addChild(menu);
    _collections_scroll_view->setMenu(menu->menu());

   // this->addChild(menu);
    menu->setAnchorPoint(ccp(0,0));
    menu->setPosition(ccp(0,
                          0));



}

void SelectLevel::hideEverything(CCCallFunc *callback)
{

    //_collections_scroll_view->runAction(CCFadeTo::create(fade_out_duration, 0));
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(0),
                    callback,
                    NULL));

}

