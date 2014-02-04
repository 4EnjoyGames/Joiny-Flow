#include "LevelScene.h"
#include "AppMacros.h"
USING_NS_CC;


#include "Flow/FlowGame.h"
#include "Flow/Generator/FGenerator.h"
#include "Flow/JoinyPair.h"
#include "Scenes/SelectLevel.h"
#include <algorithm>
#include <ADLib/Device/ADAds.h>
#include "Localization/CCLocalizedString.h"
#include <ADLib/ADString.h>
#include "Logic/Hints.h"
#include <ADLib/Device/ADInApp.h>

class LevelScene::BuyHintPopUp : public PopUpWindow::Content
{
public:
    BuyHintPopUp(LevelScene* parent=0):_parent(parent){}
private:

    typedef BuyHintPopUp Me;
    LevelScene* _parent;

    void onCreate(CCNode *parent)
    {
        CCSize size = parent->getContentSize();

        //set collor to background
        CCSprite* parent_rgb = (CCSprite*)parent->getChildByTag(123);
        parent_rgb->setColor(GameInfo::getInstance()->getPositiveColor());


        //create menu for AnimatedSprites
        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                               "buy-hints/buy_hints.plist",
                                                               "buy-hints/buy_hints.png");
        MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
        menu->setPosition(ccp(0,0));
        menu->setAnchorPoint(ccp(0,0));
        menu->setContentSize(size);
        parent->addChild(menu);


        //greeting text
        CCLabelTTF * greeting = CCLabelTTF::create(_("BuyHint"),
                                                   "fonts/Fredoka One.ttf",
                                                   72);
        greeting->setPosition(ccp(size.width*0.5,
                                  size.height*0.8));
        greeting->setColor(ccc3(255,255,255));
        parent->addChild(greeting);


        //create first button == buy_5_hints

        AnimatedMenuItem *x5_hint = AnimatedMenuItem::create(
                    menu_spl->loadSprite("buy_button.png"),
                    this, menu_selector(Me::onFirstButton));
        x5_hint->setPosition(ccp(size.width*0.5,
                                    size.height*0.6));


        std::string x10_price = ADInApp::getProduct("hints_10")->getPrice();
        std::string x10_for1 = "x10  " + x10_price;
        CCLabelTTF * first_button_text = CCLabelTTF::create(x10_for1.c_str(),
                                                "fonts/Fredoka One.ttf",
                                                48);
        first_button_text->setColor(ccc3(255,255,255));
        first_button_text->setPosition(ccp(x5_hint->getContentSize().width/2,
                               x5_hint->getContentSize().height/2));
        x5_hint->addChild(first_button_text);
        menu->menu()->addChild(x5_hint);


        //create second button == buy_100_hints
        AnimatedMenuItem *x100_hint = AnimatedMenuItem::create(
                    menu_spl->loadSprite("buy_button.png"),
                    this, menu_selector(Me::onSecondButton));
        x100_hint->setPosition(ccp(size.width*0.5,
                                   size.height*0.4));

        std::string x100_price = ADInApp::getProduct("hints_100")->getPrice();
        std::string x100_for4 = "x100  " + x100_price;
        CCLabelTTF * second_button_text = CCLabelTTF::create(x100_for4.c_str(),
                                                "fonts/Fredoka One.ttf",
                                                48);
        second_button_text->setColor(ccc3(255,255,255));
        second_button_text->setPosition(ccp(x100_hint->getContentSize().width/2,
                               x100_hint->getContentSize().height/2));
        x100_hint->addChild(second_button_text);

        menu->menu()->addChild(x100_hint);


        //create third button == buy 1000 hints
        AnimatedMenuItem *x1000_hint = AnimatedMenuItem::create(
                    menu_spl->loadSprite("buy_button.png"),
                    this, menu_selector(Me::onThirdButton));
        x1000_hint->setPosition(ccp(size.width*0.5,
                                    size.height*0.2));
        std::string x1000_price = ADInApp::getProduct("hints_1000")->getPrice();
        std::string x1000_for9 = "x1000  " + x1000_price;
        CCLabelTTF * third_button_text = CCLabelTTF::create(x1000_for9.c_str(),
                                                "fonts/Fredoka One.ttf",
                                                48);
        third_button_text->setColor(ccc3(255,255,255));
        third_button_text->setPosition(ccp(x1000_hint->getContentSize().width/2,
                               x100_hint->getContentSize().height/2));
        x1000_hint->addChild(third_button_text);


        menu->menu()->addChild(x1000_hint);
    }
    void onFirstButton(CCObject*)
    {
        //TODO: may be it will be "get free hints button" action
        ADInApp::buyProduct("hints_10");

    }
    void onSecondButton(CCObject*)
    {
        ADInApp::buyProduct("hints_100");
    }
    void onThirdButton(CCObject*)
    {
        ADInApp::buyProduct("hints_1000");
    }

};

class LevelScene::LevelEndPopUp : public PopUpWindow::Content
{
public:
    enum Mode
    {
        NotEnough,
        LevelEnd
    };


    LevelEndPopUp(const Mode mode,
                  const FlowScore score=0,
                  const unsigned int stars=0,
                  const JoinyLevel* level=0,
                  LevelScene* parent=0)
        : _mode(mode), _score(score),
          _stars(stars), _level(level), _parent(parent)
    {}
private:
    typedef LevelEndPopUp Me;
    LevelScene* _parent;
    void onNextLevel(CCObject*)
    {
        _parent->onNextLevel();
    }

    void onPlayMore(CCObject*)
    {
        this->closeWindow();
    }


    void onCreate(CCNode *parent)
    {

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;

       std::string text ="";
        if(_mode==NotEnough)
        {
            text = _("End_bad");//+'\n';
                    //+ std::to_string(_score);
        }
        else
        {
            text =  _("End_good");//+'\n';
                    //std::to_string(_score)
                    //+'\n';
        }

        CCLabelTTF* label = CCLabelTTF::create(text.c_str(),
                                               "fonts/Fredoka One.ttf",
                                               62);

        if(_mode == NotEnough)
        {
            label->setFontSize(48);
            label->setPosition(ccp(x_middle, size.height*0.7f));

            std::string score_to_bronze = AD_to_string(_level->getPuzzle()._info.getBronze());

            CCLabelTTF* score = CCLabelTTF::create(score_to_bronze.c_str(),
                                                   "fonts/Fredoka One.ttf",
                                                   92);
            score->setPosition(ccp(x_middle, size.height*0.55f));
            parent->addChild(score);
        }
        else
        {
            label->setFontSize(48);
            label->setPosition(ccp(x_middle, size.height*0.9f));

            CCLabelTTF* score = CCLabelTTF::create(AD_to_string(_score).c_str(),
                                                   "fonts/Fredoka One.ttf",
                                                   78);
            score->setPosition(ccp(x_middle, size.height*0.78f));
            parent->addChild(score);
        }

        //label->setPosition(ccp(x_middle, size.height*0.7f));
        parent->addChild(label);

        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                               "level-end/level_end.plist",
                                                               "level-end/level_end.png");
        MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
        menu->setPosition(ccp(0,0));
        menu->setAnchorPoint(ccp(0,0));
        menu->setContentSize(size);
        parent->addChild(menu);

        CCSprite* parent_rgb = (CCSprite*)parent->getChildByTag(123);
        if(parent_rgb)
        {
            if(_mode==NotEnough)
                parent_rgb->setColor(GameInfo::getInstance()->getNegativeColor());
            else
                parent_rgb->setColor(GameInfo::getInstance()->getTitleColor());
        }

        //add stars
        if(_mode==LevelEnd)
        {
            SpritesLoader _spl = GraphicsManager::getLoaderFor(parent,
                                                              "level-end/big_stars.plist",
                                                              "level-end/big_stars.png");
            _spl->inject();

            CCSprite* stars_spr = 0;

            if(_stars==1)
                stars_spr = _spl->loadSprite("big_stars_1.png");
            else if(_stars==2)
                stars_spr = _spl->loadSprite("big_stars_2.png");
            else
                stars_spr = _spl->loadSprite("big_stars_3.png");

            stars_spr->setPosition(ccp(x_middle, size.height*0.5f));
            //parent->addChild(stars_spr);

        }

        float vertical = size.height * 0.18f;


        AnimatedMenuItem *play_more_item = AnimatedMenuItem::create(
                    menu_spl->loadSprite("level_end_button.png"),
                    this, menu_selector(Me::onPlayMore));

        CCLabelTTF * retry_text = CCLabelTTF::create(_("Retry"),
                                                "fonts/Fredoka One.ttf",
                                                48);
        retry_text->setColor(ccc3(255,255,255));



        if(_mode==LevelEnd)
        {
            play_more_item->setPosition(ccp(size.width*0.25,
                                        vertical));

            retry_text->setPosition(ccp(play_more_item->getContentSize().width/2,
                                   play_more_item->getContentSize().height/2));
        }
        else
        {
            play_more_item->setPosition(ccp(size.width*0.5,
                                        vertical));
            retry_text->setPosition(ccp(play_more_item->getContentSize().width/2,
                                   play_more_item->getContentSize().height/2));

        }

        play_more_item->addChild(retry_text);


        if(_mode == LevelEnd)
        {

            CCLabelTTF * next_level_text = CCLabelTTF::create(_("Next"),
                                                    "fonts/Fredoka One.ttf",
                                                    48);
            next_level_text->setColor(ccc3(255,255,255));

            AnimatedMenuItem *next_level = AnimatedMenuItem::create(
                        menu_spl->loadSprite("level_end_button.png"),
                        this, menu_selector(Me::onNextLevel));
            next_level->setPosition(ccp(size.width*0.75,
                                        vertical));

            next_level_text->setPosition(ccp(next_level->getContentSize().width/2,
                                   next_level->getContentSize().height/2));
            next_level->addChild(next_level_text);
            menu->menu()->addChild(next_level);
        }

        menu->menu()->addChild(play_more_item);

    }
    FlowScore _score;
    unsigned int _stars;
    Mode _mode;
    const JoinyLevel* _level;
};

LevelScene* LevelScene::create(const JoinyLevel *level)
{
    LevelScene *pRet = new LevelScene(level);
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


LevelScene::LevelScene(const JoinyLevel * current_level)
    :
      _pop_up_manager(this),
      _last_score(0),
      _current_level(current_level),
      _score_label(0)
{

}

CCScene* LevelScene::scene(const JoinyLevel *current_level)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    LevelScene *layer = LevelScene::create(current_level);

    // add layer as a child to scene
    BackgroundHolder::backgroundSwitchTo(scene, 0);
    //scene->addChild(backgorund);
    scene->addChild(layer);

    // return the scene
    return scene;
}

class LevelScene::FlowDelegate : public FlowGame::Delegate
{
public:
    FlowDelegate(LevelScene* parent)
        : _parent(parent)
    {}

    void onScoreChanged(const FlowScore s)
    {
        _parent->onScoreChanged(s);
    }

    void onWin()
    {
        _parent->onWin();
    }

private:
    LevelScene* _parent;
};

void LevelScene::onNextLevel()
{
//    if(ADAds::getInstance().getInterstialTimesShowed() < 5)
//    {
//        ADAds::getInstance().showInterstitial();
//        ADAds::getInstance().prepareInterstitial();
//    }

    const JoinyLevel* next_level = RW::getLevelManager().getNextLevel(_current_level);
    if(next_level != nullptr)
    {
        _next_level = next_level;
        this->hideEverything(CCCallFunc::create(
                                 this, callfunc_selector(LevelScene::doOpenNextLevel)));
    }
    else
    {
        this->hideEverything(CCCallFunc::create(
                                 this, callfunc_selector(LevelScene::doGoToCollection)));

    }
}
void LevelScene::doReloadLevel()
{
    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_current_level));

}

void LevelScene::doGoToCollection()
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene(_current_level->getCollection()));
}

void LevelScene::doOpenNextLevel()
{
    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_next_level));

}
void LevelScene::doOpenPreviousLevel()
{
    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_previous_level));

}
void LevelScene::onPreviousLevelClicked(CCObject*)
{
    const JoinyLevel* previous_level = RW::getLevelManager().getPreviousLevel(_current_level);
    if(previous_level != nullptr)
    {
        _previous_level = previous_level;
        this->hideEverything(CCCallFunc::create(
                                 this, callfunc_selector(LevelScene::doOpenPreviousLevel)));
    }
    else
    {
        this->hideEverything(CCCallFunc::create(
                                 this, callfunc_selector(LevelScene::doGoToCollection)));

    }
}
void LevelScene::onNextLevelClicked(CCObject*)
{
    onNextLevel();
}
void LevelScene::onHintClicked(CCObject*)
{
    bool succesfull_hint = false;
    CCLog("Hint clicked in LevelScene");
    if(Hints::getInstance()->hasHint())
        succesfull_hint = Hints::getInstance()->showHint(_current_level,_flow_game);
    else
    {
        //show hint purchase window
        _pop_up_manager.openWindow(new BuyHintPopUp(this),true);
    }

    if(succesfull_hint)
    {
        unsigned int hint = Hints::getInstance()->getHintNumber();
        std::string hint_num = AD_to_string(hint);
        _hint_number_text->setString(hint_num.c_str());
    }
    else
    {
        //TODO: if not succesfull hint - show toast: "Showed all hints at the level"
    }
}
void LevelScene::onReloadLevelClicked(CCObject*)
{
    this->hideEverything(CCCallFunc::create(
                             this, callfunc_selector(LevelScene::doReloadLevel)));

}

bool LevelScene::init()
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


    this->setKeypadEnabled(true);

    unsigned int N = _current_level->getBoardSize().x();
    FlowTable table(N,N);
    JoinyPuzzle task = _current_level->getPuzzle();

    _current_info = task._info;
    for(JoinyTask::iterator it = task._task.begin(); it!=task._task.end(); ++it)
    {
        JoinyPair& s = *it;
        table.addColor(s.getPoints().first, s.getPoints().second, s.getColor());
    }

    //    std::stringstream ss;
    //    ss << "Bronze: " << task._info.getBronze() << std::endl;
    //    ss << "Silver: " << task._info.getSilver() << std::endl;
    //    ss << "Gold: " << task._info.getGold() << std::endl;
    //    ss << "Highscore: " << _current_level->getHighScore() << std::endl;


    //Back Button
    CCMenu* menu = _back.start(this, [this](){this->onButtonBackClicked(0);});
    this->addChild(menu);

    CCSize back_size = _back.getBackSize();

    _silver = _current_info.getSilver();
    _bronze = _current_info.getBronze();
    _gold = _current_info.getGold();
    _max_score = _gold + 100;

    //    CCLabelTTF* scores = CCLabelTTF::create(ss.str().c_str(), "fonts/Fredoka One.ttf", 28);
    //    scores->setAnchorPoint(ccp(1,1));
    //    scores->setColor(ccc3(0,0,0));
    //    scores->setPosition(ccp(ORIGIN.x+VISIBLE_SIZE.width, ORIGIN.y+VISIBLE_SIZE.height));
    //    this->addChild(scores);

    _score_label = CCLabelTTF::create("0", "fonts/Fredoka One.ttf", 70);

    _score_label->setAnchorPoint(ccp(0, 1));
    _score_label->setPosition(ccp(ORIGIN.x + back_size.width + (VISIBLE_SIZE.width - back_size.width)*0.25 ,
                                  ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE));
    _score_label->setColor(GameInfo::getInstance()->getTitleColor());
    _score_label->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(_score_label);

    _flow_game = FlowGame::create(table, FlowGame::DelegatePtr(new FlowDelegate(this)));
    //CCSize render_size = _flow_game->getContentSize();

    //    _flow_game->setScale(MIN(VISIBLE_SIZE.width / (render_size.width),
    //                         VISIBLE_SIZE.width / (render_size.height+60/SCALE) ));
    //    //_flow_game->setScale(0.3f);
    //_flow_game->setAnchorPoint(ccp(0, 0.5f));
    //    _flow_game->setPosition(ccp(ORIGIN.x + 20/SCALE,
    //                                ORIGIN.y + VISIBLE_SIZE.height*0.57));
    this->addChild(_flow_game);


    /////////////////////////////////////////////////////////////////

    //    CCSprite* tablo = CCSprite::create("level-scene/tablo.png");
    //    //tablo->setPosition(ORIGIN);

    //    CCSize tablo_size = tablo->getContentSize();
    //    tablo->removeFromParentAndCleanup(true);

    SpritesLoader  buttons_spl = GraphicsManager::getLoaderFor(
                0,
                "level-scene/level_novigation.plist",
                "level-scene/level_novigation.png");

    _buttons_menu = MenuSpriteBatch::create(buttons_spl);
    _buttons_menu->setPosition(ORIGIN);

    //previous level
    CCSprite* prev_level_logo = buttons_spl->loadSprite("prev_level.png");

    float menu_height = prev_level_logo->getContentSize().height * 1.2f;

    _buttons_menu->setContentSize(CCSize(VISIBLE_SIZE.width,
                                         menu_height));

    AnimatedMenuItem* prev_level_button = AnimatedMenuItem::create(
                prev_level_logo,
                this,
                menu_selector(LevelScene::onPreviousLevelClicked));

    prev_level_button->setPosition(ccp(VISIBLE_SIZE.width/2 - 100/SCALE,
                                       menu_height/2));
    _buttons_menu->menu()->addChild(prev_level_button);

    //reload
    CCSprite* reload_level_logo = buttons_spl->loadSprite("reload_level.png");
    AnimatedMenuItem* reload_level_button = AnimatedMenuItem::create(
                reload_level_logo,
                this,
                menu_selector(LevelScene::onReloadLevelClicked));

    reload_level_button->setPosition(ccp(VISIBLE_SIZE.width/2 ,
                                         menu_height/2));
    _buttons_menu->menu()->addChild(reload_level_button);


    //next level
    CCSprite* next_level_logo = buttons_spl->loadSprite("next_level.png");
    AnimatedMenuItem* next_level_button = AnimatedMenuItem::create(
                next_level_logo,
                this,
                menu_selector(LevelScene::onNextLevelClicked));

    next_level_button->setPosition(ccp(VISIBLE_SIZE.width/2 + 100/SCALE,
                                       menu_height/2));
    _buttons_menu->menu()->addChild(next_level_button);


    //hint
    CCSprite* hint_logo = buttons_spl->loadSprite("hint.png");
    AnimatedMenuItem* hint_button = AnimatedMenuItem::create(
                hint_logo,
                this,
                menu_selector(LevelScene::onHintClicked));

    hint_button->setPosition(ccp(VISIBLE_SIZE.width/2 + 300/SCALE,
                                       menu_height/2));
    _buttons_menu->menu()->addChild(hint_button);


    //add hints number text with background
    CCPoint hint_text_position(ccp(hint_button->getContentSize().width*0.9,
                                   hint_button->getContentSize().height*0.3));

    //number of hints
    unsigned int hints = Hints::getInstance()->getHintNumber();
    std::string hints_str = AD_to_string(hints);
    _hint_number_text = CCLabelTTF::create(hints_str.c_str(),
                                           "fonts/Fredoka One.ttf",
                                           55);
    _hint_number_text->setColor(GameInfo::getInstance()->getNegativeColor());
    _hint_number_text->setPosition(hint_text_position);
    _hint_number_text->setAnchorPoint(ccp(0.5,0.5));
    hint_button->addChild(_hint_number_text);


    //white circle background
//    CCSprite* hint_text_background = buttons_spl->loadSprite("hint_background.png");
//    hint_text_background->setPosition(ccp(hint_button->getPositionX()+hint_button->getContentSize().width*0.41,
//                                          hint_button->getContentSize().height*0.4));
//    hint_text_background->setScale(_hint_number_text->getContentSize().width/_hint_number_text->getContentSize().width*0.5);
//    hint_button->addNephew(hint_text_background);

    this->addChild(_buttons_menu);

    //////////////////////////////////////////////////////////////////////////

    //progress bar title
    CCSprite* progress = CCSprite::create("level-scene/progress_bar_title.png");
    progress->setPosition(ccp(ORIGIN.x+VISIBLE_SIZE.width -200/SCALE,
                              ORIGIN.y+VISIBLE_SIZE.height-70/SCALE));
    this->addChild(progress);


    _procc = 0;
    _progress_timer = CCProgressTimer::create(CCSprite::create("level-scene/progress_bar.png") );
    if ( _progress_timer != NULL )
    {
        _progress_timer->setType(kCCProgressTimerTypeBar);
        _progress_timer->setMidpoint(ccp(0, 0));
        _progress_timer->setBarChangeRate(ccp(1, 0));
        _progress_timer->setPercentage(_procc);
        _progress_timer->setPosition(ccp(ORIGIN.x+VISIBLE_SIZE.width -200/SCALE,
                                         ORIGIN.y+VISIBLE_SIZE.height-70/SCALE));

    }
    addChild(_progress_timer);


    ///////////////////////////////////////////////////////////////////
    //Get the sprites loader
    //stars
    _spl = GraphicsManager::getLoaderFor(this,
                                         "level-scene/level_stars.plist",
                                         "level-scene/level_stars.png");
    _spl->inject();


    CCPoint bar_begin = ccp(progress->getPositionX() - progress->getContentSize().width*0.5,
                            progress->getPositionY());
    CCSize bar_size = progress->getContentSize();
    //3-d
    _bronze_star = _spl->loadSprite("black_star.png");
    float bronze_proc = (static_cast<float>(_bronze*100)/_max_score)/100.0f;
    _bronze_position = ccp(bar_begin.x + bar_size.width*bronze_proc,
                           bar_begin.y - 30/SCALE);
    _bronze_star->setPosition(_bronze_position);

    //2-d
    _silver_star = _spl->loadSprite("black_star.png");
    float silver_proc = (static_cast<float>(_silver*100)/_max_score)/100.0f;
    _silver_position = ccp(bar_begin.x + bar_size.width*silver_proc,
                           bar_begin.y - 30/SCALE);
    _silver_star->setPosition(_silver_position);


    //1-st
    _gold_star = _spl->loadSprite("black_star.png");
    float gold_proc = (static_cast<float>(_gold*100)/_max_score)/100.0f;
    _gold_position = ccp(bar_begin.x + bar_size.width*gold_proc,
                         bar_begin.y - 30/SCALE);
    _gold_star->setPosition(_gold_position);

    ////////////////////////////////////////////////////////////////////

    const float TOP_MARGIN = 120/SCALE;

    float main_zone_height = VISIBLE_SIZE.height - TOP_MARGIN*2 - menu_height;

    if(main_zone_height > VISIBLE_SIZE.width)
        main_zone_height = VISIBLE_SIZE.width;

    float banner_zone_height = VISIBLE_SIZE.height - TOP_MARGIN -
            menu_height - main_zone_height;

    const float BANNER_MARGIN = 10/SCALE;
    CCSize banner_zone(VISIBLE_SIZE.width - BANNER_MARGIN*2,
                       banner_zone_height - BANNER_MARGIN);


    ADAds::Banner* banner = ADAds::getBanner(banner_zone);

    banner_zone_height = 0;
    if(banner)
    {
        banner->setAnchorPoint(ccp(0.5f, 0));
        banner->setPosition(ccp(ORIGIN.x + VISIBLE_SIZE.width/2,
                                ORIGIN.y + BANNER_MARGIN));
        banner_zone_height = banner->getContentSize().height + BANNER_MARGIN;
        this->addChild(banner);
    }

    main_zone_height = VISIBLE_SIZE.height - TOP_MARGIN - banner_zone_height - menu_height;
    if(main_zone_height > VISIBLE_SIZE.width)
        main_zone_height = VISIBLE_SIZE.width;

    float main_node_size = main_zone_height - BANNER_MARGIN*2;

    float main_node_margin = (VISIBLE_SIZE.height - TOP_MARGIN -
                              banner_zone_height - menu_height - main_zone_height)/3;

    _buttons_menu->setPositionY(ORIGIN.y + banner_zone_height + main_node_margin);


    float main_node_scale = main_node_size /
            _flow_game->getContentSize().height;
    _flow_game->setScale(main_node_scale);
    _flow_game->setAnchorPoint(ccp(0.5, 0));
    _flow_game->setPositionX(ORIGIN.x + VISIBLE_SIZE.width/2);
    _flow_game->setPositionY(_buttons_menu->getPositionY() + menu_height + main_node_margin);

    return true;
}

void LevelScene::onScoreChanged(const FlowScore s)
{
    std::stringstream ss;
    ss << s;
    _score_label->setString(ss.str().c_str());

    _last_score = s;

    _procc = static_cast<float>(_last_score*100)/_max_score;
    if ( _progress_timer != NULL )
    {
        _progress_timer->setPercentage(_procc);
    }

    if(s>=_bronze)
    {
        _bronze_star = _spl->loadSprite("color_star.png");
        _bronze_star->setPosition(_bronze_position);
    }
    if(s>=_silver)
    {
        _silver_star = _spl->loadSprite("color_star.png");
        _silver_star->setPosition(_silver_position);
    }
    if(s>=_gold)
    {
        _gold_star = _spl->loadSprite("color_star.png");
        _gold_star ->setPosition(_gold_position);
    }

    if(s<_bronze)
    {
        _bronze_star = _spl->loadSprite("black_star.png");
        _bronze_star->setPosition(_bronze_position);
    }
    if(s<_silver)
    {
        _silver_star = _spl->loadSprite("black_star.png");
        _silver_star->setPosition(_silver_position);
    }
    if(s<_gold)
    {
        _gold_star = _spl->loadSprite("black_star.png");
        _gold_star ->setPosition(_gold_position);
    }
}

void LevelScene::onWin()
{
    if(_last_score < _current_info.getBronze())
        _pop_up_manager.openWindow(new LevelEndPopUp(LevelEndPopUp::NotEnough, _current_info.getBronze()-_last_score, 0, _current_level, this));
    else
    {
        RW::getLevelManager().onLevelFinished(_current_level, _last_score);
        unsigned int stars = _current_level->getStarsNumber(_last_score);

        _pop_up_manager.openWindow(new LevelEndPopUp(LevelEndPopUp::LevelEnd, _last_score, stars, _current_level, this));
    }
}

void LevelScene::hideEverything(cocos2d::CCCallFunc* callback)
{
    _flow_game->endGame();
    this->runAction(callback);
}


void LevelScene::keyBackClicked()
{
    if(!_pop_up_manager.backAction())
    {
        this->hideEverything(CCCallFunc::create(
                                 this, callfunc_selector(LevelScene::doGoToCollection)));

    }
}






