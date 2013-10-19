
#include "Logic/RW.h"
#include "SelectLevel.h"
#include "Logic/Language.h"
#include "Store.h"
#include "Core/Statistics.h"
#include "Scenes/LevelScene.h"

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
    CCDirector::sharedDirector()->end();
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



AnimatedMenuItem* SelectLevel::createLevelItem(const JoinyLevel* level, const SpritesLoader& spl)
{
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();

    std::stringstream level_number;
    level_number << level->getLevelId();

    unsigned int stars = level->getStarsNumber(level->getHighScore());
    static ccColor3B none(ccc3(255, 255, 255));
    static ccColor3B highscore(ccc3(65,255,28));
    static ccColor3B finished(ccc3(255,242,28));

    ccColor3B working=finished;
    if(stars == 3)
        working = highscore;
    else if(stars == 0)
        working = none;


    CCSprite* background = spl->loadSprite("level_button.png");
    background->setColor(working);
    AnimatedMenuItem* item = AnimatedMenuItem::create(
                    background,
                    this,
                    menu_selector(SelectLevel::onLevelSelect));

    CCLabelTTF* label = CCLabelTTF::create(level_number.str().c_str(), "Arial", 60/scaled);
    item->addChild(label);
    float scale = MIN(1, background->getContentSize().width * 0.7 / label->getContentSize().width);
    label->setPosition(ccp(background->getContentSize().width/2-3*scale, background->getContentSize().height/2));
    label->setAnchorPoint(ccp(0.5, 0.5));
    label->setScale(scale);
    label->setColor(working);

    return item;
}

bool SelectLevel::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }


    //To trigger back button
    this->setKeypadEnabled(true);

    //Get the size of the screen we can see
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    //Get the screen start of cordinates
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();



    //Create menu with collections


    _col_spl = GraphicsManager::getLoaderFor(
                0,
                "level_buttons.plist",
                "level_buttons.png");
    _buttons_menu = MenuSpriteBatch::create(_col_spl);


    float padding_left = 330/scaled;
    unsigned int i = 0;
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

                _buttons_map[item] = l;

                working_x += s.width + margin;
            }

        }
        working_y -= s.height + margin;
    }

    //_buttons_menu->setAnchorPoint(ccp(0, 0));
    //_buttons_menu->setPosition(origin.x, origin.y);
    //this->addChild(_buttons_menu);
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

void SelectLevel::hideEverything(CCCallFunc *callback)
{

    //_collections_scroll_view->runAction(CCFadeTo::create(fade_out_duration, 0));
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(0),
                    callback,
                    NULL));

}
