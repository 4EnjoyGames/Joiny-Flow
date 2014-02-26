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
#include <ADLib/Device/ADInApp.h>
#include <ADLib/Device/ADStatistics.h>
#include "Core/Fonts.h"
#include "Logic/Tutorial.h"
#include "GameInfo.h"
#include <ADLib/Device/ADBrowser.h>

class LevelScene::RateMePopUp : public PopUpWindow::Content
{
public:
    RateMePopUp(LevelScene* parent=0)
        : _parent(parent)
    {}
private:
    typedef RateMePopUp Me;
    LevelScene* _parent;

    void onRateLevel(CCObject*)
    {
        CCLog("on Rate Level clicked");
        ADBrowser::openApplicationPage(GameInfo::getPackageName("joiny"));
        this->closeWindow();
    }

    void onCreate(CCNode *parent)
    {
        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;

        //create welcome text
        CCLabelTTF* label = CCLabelTTF::create(_("RateMePopUp.RateLabel"),
                                               Fonts::getFontName(),
                                               55);
        label->setPosition(ccp(x_middle, size.height*0.8f));
        label->setColor(ccc3(255,255,255));
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
            parent_rgb->setColor(GameInfo::getInstance()->getTitleColor());

        //TODO: add 5 stars
        SpritesLoader spl = GraphicsManager::getLoaderFor(parent,
                                                           "level-end/big_stars.plist",
                                                           "level-end/big_stars.png");
        spl->inject();
        CCSprite* stars_spr = spl->loadSprite("rate_stars.png");
        stars_spr->setPosition(ccp(x_middle, size.height*0.55f));


        //create rate button
        /////////////////////////////////////////

        int font_size = 35;
        float label_scale_factor = 0.1f;

        CCSprite* rate_sprite = menu_spl->loadSprite("level_end_button.png");
        rate_sprite->setScale(rate_sprite->getContentSize().width/
                              rate_sprite->getContentSize().width*label_scale_factor);
        rate_sprite->setColor(ccc3(255,255,255));

        AnimatedMenuItem *rate_button = AnimatedMenuItem::create(
                    rate_sprite,
                    this, menu_selector(Me::onRateLevel));
        rate_button->setPosition(ccp(x_middle,size.height*0.25));

        CCLabelTTF * rate_text = CCLabelTTF::create(_("RateMePopUp.RateText"),
                                                          Fonts::getFontName(),
                                                          font_size);

        rate_text->setColor(ccc3(255,255,255));
        rate_text->setPosition(ccp(rate_button->getContentSize().width/2,
                                         rate_button->getContentSize().height/2));
        rate_button->addChild(rate_text);
        menu->menu()->addChild(rate_button);

        ////////////////////////////////////////////////////
    }
};

class LevelScene::TesterEndPopUp : public PopUpWindow::Content
{
public:
    TesterEndPopUp(LevelScene* parent=0, const JoinyLevel* level=0)
        : _parent(parent),
          _level(level)
    {}
private:
    typedef TesterEndPopUp Me;
    LevelScene* _parent;

    void onGoodLevel(CCObject*)
    {
        std::string collection_id = AD_to_string(_level->getCollection()->getCollectionID());
        std::string level_id =  collection_id
                +'-' + AD_to_string(_level->getLevelId());
        std::string rate = "good";

        ADStatistics::Params params;
        params.add("level_id", level_id);
        params.add("collection_id", collection_id);
        params.add("rate", rate);
        ADStatistics::logEvent("Tester Level Rate", params);

        _parent->onNextLevel();
    }

    void onBadLevel(CCObject*)
    {
        std::string collection_id = AD_to_string(_level->getCollection()->getCollectionID());
        std::string level_id =  collection_id
                +'-' + AD_to_string(_level->getLevelId());
        std::string rate = "bad";

        ADStatistics::Params params;
        params.add("level_id", level_id);
        params.add("collection_id", collection_id);
        params.add("rate", rate);
        ADStatistics::logEvent("Tester Level Rate", params);

        _parent->onNextLevel();
    }

    //when clock "I do not knaw is it a bad or a good level"
    void onSoLevel(CCObject*)
    {
        std::string collection_id = AD_to_string(_level->getCollection()->getCollectionID());
        std::string level_id =  collection_id
                +'-' + AD_to_string(_level->getLevelId());
        std::string rate = "so-so";

        ADStatistics::Params params;
        params.add("level_id", level_id);
        params.add("collection_id", collection_id);
        params.add("rate", rate);
        ADStatistics::logEvent("Tester Level Rate", params);

        _parent->onNextLevel();
    }


    void onCreate(CCNode *parent)
    {

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;

        CCLabelTTF* label = CCLabelTTF::create(_("Test_level_end"),
                                               Fonts::getFontName(),
                                               42);
        label->setPosition(ccp(x_middle, size.height*0.8f));
        label->setColor(ccc3(255,255,255));
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
            parent_rgb->setColor(GameInfo::getInstance()->getTitleColor());

        /////////////////////////////////////////

        int font_size = 35;
        float label_scale_factor = 0.1f;

        CCSprite* good_sprite = menu_spl->loadSprite("level_end_button.png");
        good_sprite->setScale(good_sprite->getContentSize().width/
                              good_sprite->getContentSize().width*label_scale_factor);
        good_sprite->setColor(GameInfo::getInstance()->getPositiveColor());

        AnimatedMenuItem *good_level_button = AnimatedMenuItem::create(
                    good_sprite,
                    this, menu_selector(Me::onGoodLevel));
        good_level_button->setPosition(ccp(x_middle,size.height*0.6));

        CCLabelTTF * good_level_text = CCLabelTTF::create(_("Good_level"),
                                                          Fonts::getFontName(),
                                                          font_size);

        good_level_text->setColor(ccc3(255,255,255));
        good_level_text->setPosition(ccp(good_level_button->getContentSize().width/2,
                                         good_level_button->getContentSize().height/2));
        good_level_button->addChild(good_level_text);
        menu->menu()->addChild(good_level_button);

        ////////////////////////////////////////////////////

        CCSprite* so_sprite = menu_spl->loadSprite("level_end_button.png");
        so_sprite->setScale(so_sprite->getContentSize().width/
                            so_sprite->getContentSize().width*label_scale_factor);
        so_sprite->setColor(ccc3(255,255,255));

        AnimatedMenuItem *so_level_button = AnimatedMenuItem::create(
                    so_sprite,
                    this, menu_selector(Me::onSoLevel));
        so_level_button->setPosition(ccp(x_middle,size.height*0.4));

        CCLabelTTF * so_level_text = CCLabelTTF::create(_("So_level"),
                                                        Fonts::getFontName(),
                                                        font_size);
        so_level_text->setColor(ccc3(255,255,255));
        so_level_text->setPosition(ccp(so_level_button->getContentSize().width/2,
                                       so_level_button->getContentSize().height/2));
        so_level_button->addChild(so_level_text);
        menu->menu()->addChild(so_level_button);

        ////////////////////////////////////////////////////

        CCSprite* bad_sprite = menu_spl->loadSprite("level_end_button.png");
        bad_sprite->setScale(bad_sprite->getContentSize().width/
                             bad_sprite->getContentSize().width*label_scale_factor);
        bad_sprite->setColor(GameInfo::getInstance()->getNegativeColor());

        AnimatedMenuItem *badlevel_button = AnimatedMenuItem::create(
                    bad_sprite,
                    this, menu_selector(Me::onSoLevel));
        badlevel_button->setPosition(ccp(x_middle,size.height*0.2));

        CCLabelTTF * bad_level_text = CCLabelTTF::create(_("Bad_level"),
                                                         Fonts::getFontName(),
                                                         font_size);
        bad_level_text->setColor(ccc3(255,255,255));
        bad_level_text->setPosition(ccp(badlevel_button->getContentSize().width/2,
                                        badlevel_button->getContentSize().height/2));
        badlevel_button->addChild(bad_level_text);
        menu->menu()->addChild(badlevel_button);

    }
    const JoinyLevel* _level;
};


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
        parent_rgb->setColor(GameInfo::getInstance()->getTitleColor());


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
                                                   Fonts::getFontName(),
                                                   50);
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
                                                            Fonts::getFontName(),
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
                                                             Fonts::getFontName(),
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
                                                            Fonts::getFontName(),
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
        //this->closeWindow();

    }
    void onSecondButton(CCObject*)
    {
        ADInApp::buyProduct("hints_100");
        //this->closeWindow();
    }
    void onThirdButton(CCObject*)
    {
        ADInApp::buyProduct("hints_1000");
        //this->closeWindow();
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
            text = _("End_bad");
        }
        else
        {
            text =  _("End_good");
        }

        CCLabelTTF* label = CCLabelTTF::create(text.c_str(),
                                               Fonts::getFontName(),
                                               48);

        if(_mode == NotEnough)
        {
            label->setPosition(ccp(x_middle, size.height*0.8f));
        }
        else
        {
            label->setPosition(ccp(x_middle, size.height*0.9f));

            CCLabelTTF* score = CCLabelTTF::create(AD_to_string(_score).c_str(),
                                                   Fonts::getFontName(),
                                                   78);
            score->setPosition(ccp(x_middle, size.height*0.78f));
            parent->addChild(score);
        }
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
        SpritesLoader _spl = GraphicsManager::getLoaderFor(parent,
                                                           "level-end/big_stars.plist",
                                                           "level-end/big_stars.png");
        _spl->inject();

        CCSprite* stars_spr = 0;
        if(_mode==LevelEnd)
        {
            if(_stars==1)
                stars_spr = _spl->loadSprite("big_stars_1.png");
            else if(_stars==2)
                stars_spr = _spl->loadSprite("big_stars_2.png");
            else
                stars_spr = _spl->loadSprite("big_stars_3.png");

            stars_spr->setPosition(ccp(x_middle, size.height*0.5f));

        }
        else
        {
            stars_spr = _spl->loadSprite("big_stars_0.png");
            stars_spr->setPosition(ccp(x_middle, size.height*0.5f));
        }

        float vertical = size.height * 0.18f;


        AnimatedMenuItem *play_more_item = AnimatedMenuItem::create(
                    menu_spl->loadSprite("level_end_button.png"),
                    this, menu_selector(Me::onPlayMore));

        CCLabelTTF * retry_text = CCLabelTTF::create(_("Retry"),
                                                     Fonts::getFontName(),
                                                     35);
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
                                                              Fonts::getFontName(),
                                                              35);
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

LevelScene* LevelScene::create(const JoinyLevel *level,
                               bool show_animation,
                               bool show_rate_me)
{
    LevelScene *pRet = new LevelScene(level,show_animation,show_rate_me);
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


LevelScene::LevelScene(const JoinyLevel * current_level,
                       bool show_animation,
                       bool show_rate_me)
    :_show_open_animation(show_animation),
      _pop_up_manager(this),
      _last_score(0),
      _current_level(current_level),
      _score_label(0),
      _showed_ads(false),
      _show_rate_me(show_rate_me)
{
    //_last_scene = this;
    this->setTag(123456);
}

CCScene* LevelScene::scene(const JoinyLevel *current_level, bool show_animation, bool show_rate_me)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    LevelScene *layer = LevelScene::create(current_level,show_animation,show_rate_me);

    CCCallFunc* back = CCCallFunc::create(layer,
                                          callfunc_selector(LevelScene::keyBackClicked));

    // add layer as a child to scene
    BackgroundHolder::backgroundSwitchTo(scene, back, true,"level");
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
void LevelScene::doShowRateMe()
{
    _pop_up_manager.openWindow(new RateMePopUp(this));
}

bool LevelScene::showRateMe()
{
    bool res = false;
    const JoinyCollection* coll = _current_level->getCollection();
    if((coll->getCollectionID() == 2 &&
        _current_level->getLevelId() == 45) ||
            (coll->getCollectionID() == 3 &&
             _current_level->getLevelId() == 30) ||
            (coll->getCollectionID() == 4 &&
             _current_level->getLevelId() == 35))
    {
        res = true;
    }
    return res;
}

void LevelScene::onNextLevel(const bool show_ads)
{
    _showed_ads = false;
    if(show_ads)
    {
        if(ADAds::getInterstialTimesShowed() < 5)
        {
            if(rand() % 25 == 0)
            {
                ADAds::showInterstitial();
                _showed_ads = true;
            }
            ADAds::prepareInterstitial();
        }
        if(_last_scene)
            _last_scene->_pop_up_manager.backAction();

    }

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
    if(!_showed_ads && showRateMe())
        CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_next_level,false,true));
    else
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
        this->hideEverythingAndBack(CCCallFunc::create(
                                        this, callfunc_selector(LevelScene::doGoToCollection)));

    }
}
void LevelScene::onNextLevelClicked(CCObject*)
{
    onNextLevel(false);
}

LevelScene* LevelScene::_last_scene = 0;
void LevelScene::purchaseUpdateHints()
{
    if(_last_scene)
    {
        _last_scene->renewOneHint();
        _last_scene->_pop_up_manager.backAction();
    }
}
void LevelScene::onHintClicked(CCObject*)
{
    this->stopAction(_hint_action);
    this->runAction(createHintAction());

    bool succesfull_hint = false;
    CCLog("Hint clicked in LevelScene");
    if(_hints.hasHint())
        succesfull_hint = _hints.showHint();
    else
    {
        //show hint purchase window
        _pop_up_manager.openWindow(new BuyHintPopUp(this),true);
    }

    renewOneHint();
}
void LevelScene::renewOneHint()
{
    //renew hint num
    unsigned int hint = _hints.getHintNumber();
    std::string hint_num = AD_to_string(hint);
    _hint_number_text->setString(hint_num.c_str());
}

void LevelScene::onReloadLevelClicked(CCObject*)
{
    this->hideEverything(CCCallFunc::create(
                             this, callfunc_selector(LevelScene::doReloadLevel)));

}
void LevelScene::onEnter()
{
    _last_scene = this;
    DrawLayer::onEnter();
}

void LevelScene::onExit()
{
    _last_scene = 0;
    _flow_game->stopTrackingTouch();
    DrawLayer::onExit();
}

bool LevelScene::init()
{
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

    //Back Button
    _back_menu = _back.start(this, [this](){this->onButtonBackClicked(0);});
    this->addChild(_back_menu);

    CCSize back_size = _back.getBackSize();

    _silver = _current_info.getSilver();
    _bronze = _current_info.getBronze();
    _gold = _current_info.getGold();
    _max_score = _gold + 100;

    _score_label = CCLabelTTF::create("0", Fonts::getFontName(), 70);

    _score_label->setAnchorPoint(ccp(0, 1));
    _score_label->setPosition(ccp(ORIGIN.x + back_size.width + (VISIBLE_SIZE.width - back_size.width)*0.25 ,
                                  ORIGIN.y + VISIBLE_SIZE.height - 70/SCALE));
    _score_label->setColor(GameInfo::getInstance()->getTitleColor());
    _score_label->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(_score_label);

    _flow_game = FlowGame::create(table, FlowGame::DelegatePtr(new FlowDelegate(this)));

    this->addChild(_flow_game);


    /////////////////////////////////////////////////////////////////

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
    _buttons.push_back(prev_level_button);

    //reload
    CCSprite* reload_level_logo = buttons_spl->loadSprite("reload_level.png");
    AnimatedMenuItem* reload_level_button = AnimatedMenuItem::create(
                reload_level_logo,
                this,
                menu_selector(LevelScene::onReloadLevelClicked));

    reload_level_button->setPosition(ccp(VISIBLE_SIZE.width/2 ,
                                         menu_height/2));
    _buttons_menu->menu()->addChild(reload_level_button);
    _buttons.push_back(reload_level_button);


    //next level
    CCSprite* next_level_logo = buttons_spl->loadSprite("next_level.png");
    AnimatedMenuItem* next_level_button = AnimatedMenuItem::create(
                next_level_logo,
                this,
                menu_selector(LevelScene::onNextLevelClicked));

    next_level_button->setPosition(ccp(VISIBLE_SIZE.width/2 + 100/SCALE,
                                       menu_height/2));
    _buttons_menu->menu()->addChild(next_level_button);
    _buttons.push_back(next_level_button);


    //hint
    CCSprite* hint_logo = buttons_spl->loadSprite("hint.png");
    _hint_button = AnimatedMenuItem::create(
                hint_logo,
                this,
                menu_selector(LevelScene::onHintClicked));

    _hint_button->setPosition(ccp(VISIBLE_SIZE.width/2 + 300/SCALE,
                                  menu_height/2));
    _buttons_menu->menu()->addChild(_hint_button);
    _buttons.push_back(_hint_button);


    //add hints number text with background
    CCPoint hint_text_position(ccp(_hint_button->getContentSize().width*0.5,
                                   _hint_button->getContentSize().height*0.1));

    //number of hints
    unsigned int hints = _hints.getHintNumber();
    std::string hints_str = AD_to_string(hints);
    _hint_number_text = CCLabelTTF::create(hints_str.c_str(),
                                           Fonts::getFontName(),
                                           45);
    _hint_number_text->setColor(GameInfo::getInstance()->getNegativeColor());
    _hint_number_text->setPosition(hint_text_position);
    _hint_number_text->setAnchorPoint(ccp(0.5,0.5));
    _hint_button->addChild(_hint_number_text);


    this->addChild(_buttons_menu);

    //////////////////////////////////////////////////////////////////////////

    //progress bar title
    _progress = CCSprite::create("level-scene/progress_bar_title.png");
    _progress->setPosition(ccp(ORIGIN.x+VISIBLE_SIZE.width -200/SCALE,
                               ORIGIN.y+VISIBLE_SIZE.height-70/SCALE));
    this->addChild(_progress);


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


    CCPoint bar_begin = ccp(_progress->getPositionX() - _progress->getContentSize().width*0.5,
                            _progress->getPositionY());
    CCSize bar_size = _progress->getContentSize();
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
    _flow_game->setScaleY(0);
    _flow_game->setAnchorPoint(ccp(0.5, 0.5));
    _flow_game->setPositionX(ORIGIN.x + VISIBLE_SIZE.width/2);
    _flow_game->setPositionY(_buttons_menu->getPositionY() + menu_height + main_node_margin
                             + _flow_game->getContentSize().height*main_node_scale*0.5f);

    _flow_game->runAction(CCEaseElasticOut::create(
                              CCScaleTo::create(0.4f, main_node_scale),
                              0.8f));


    //create hint object
    _hints = Hints(_current_level,_flow_game);


    //on what menu do not respond PopUpWindow
    _pop_up_manager.addMenuToAutoDisable(_buttons_menu->menu());
    _pop_up_manager.addOnShowWindowAction([this](){
        if(this == _last_scene)
            this->_flow_game->stopTrackingTouch();
    });
    _pop_up_manager.addOnHideWindowAction([this](){
        if(this == _last_scene)
            this->_flow_game->startTrackingTouch();
    });


    //if this is the first level of the first collection - show tutorial
    if( (_current_level->getLevelId()==1 ||
         _current_level->getLevelId()==2)
            &&
            _current_level->getCollection()->getCollectionID()==1)
    {
        Tutorial::getInstance()->setInfo(_flow_game);
        Tutorial::getInstance()->setLevel(_current_level->getCollection()->getCollectionID(),
                                          _current_level->getLevelId());

        // call hint in 4 seconds
        CCCallFunc *callAction = CCCallFunc::create(Tutorial::getInstance(),
                                                    callfunc_selector(
                                                    Tutorial::showTutorial));

        CCSequence* action = CCSequence::create(CCDelayTime::create(0.4f),
                                          callAction,
                                          NULL);
        this->runAction(action);
    }


    // set up the time delay
    createHintAction();
    // run the action
    this->runAction(_hint_action);

    if(_show_open_animation)
        showAnimation();

    if(_show_rate_me)
        doShowRateMe();

    return true;
}
CCSequence* LevelScene::createHintAction()
{
    CCDelayTime *delayAction = CCDelayTime::create(20.0f);
    // perform the selector call
    CCCallFunc *callAction = CCCallFunc::create(this,
                                                callfunc_selector(LevelScene::hintBlink));
    _hint_action = CCSequence::create(delayAction,
                                      callAction,
                                      NULL);
    return _hint_action;
}

void LevelScene::hintBlink()
{
    //animation
    float scale_button = _hint_button->getScale();
    _hint_button->setAnchorPoint(ccp(0.5, 0.5));
    _hint_button->runAction(CCRepeat::create(CCSequence::createWithTwoActions(
                                CCScaleTo::create(0.2f, scale_button*0.85),
                                CCEaseElasticOut::create(
                                    CCScaleTo::create(0.9f, scale_button),
                                    0.3f)
                                ), 3));

    this->runAction(createHintAction());
}

void LevelScene::onScoreChanged(const FlowScore s)
{
    //if the score changed - we tuched
    //stop the old action
    this->stopAction(_hint_action);
    //create new
    this->runAction(createHintAction());



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
        _bronze_star->removeFromParent();
        _bronze_star = _spl->loadSprite("color_star.png");
        _bronze_star->setPosition(_bronze_position);
    }
    if(s>=_silver)
    {
        _silver_star->removeFromParent();
        _silver_star = _spl->loadSprite("color_star.png");
        _silver_star->setPosition(_silver_position);
    }
    if(s>=_gold)
    {
        _gold_star->removeFromParent();
        _gold_star = _spl->loadSprite("color_star.png");
        _gold_star ->setPosition(_gold_position);
    }

    if(s<_bronze)
    {
        _bronze_star->removeFromParent();
        _bronze_star = _spl->loadSprite("black_star.png");
        _bronze_star->setPosition(_bronze_position);
    }
    if(s<_silver)
    {
        _silver_star->removeFromParent();
        _silver_star = _spl->loadSprite("black_star.png");
        _silver_star->setPosition(_silver_position);
    }
    if(s<_gold)
    {
        _gold_star->removeFromParent();
        _gold_star = _spl->loadSprite("black_star.png");
        _gold_star ->setPosition(_gold_position);
    }
}

void LevelScene::onWin()
{
    this->stopAction(_hint_action);

    Tutorial::getInstance()->deleteTutorialPath();

    if(_last_score < _current_info.getBronze())
    {
        _pop_up_manager.openWindow(new LevelEndPopUp(LevelEndPopUp::NotEnough, _current_info.getBronze()-_last_score, 0, _current_level, this));
        this->runAction(createHintAction());
    }
    else
    {

        //save the game
        RW::getLevelManager().onLevelFinished(_current_level, _last_score);

        //if it is a test version - open statistic window
        static bool test_mode = false;

        if(!test_mode)
        {
            unsigned int stars = _current_level->getStarsNumber(_last_score);
             _pop_up_manager.openWindow(new LevelEndPopUp(LevelEndPopUp::LevelEnd, _last_score, stars, _current_level, this));

        }
        else
        {
            _pop_up_manager.openWindow(new TesterEndPopUp(this,_current_level),true);
        }
    }
}
void LevelScene::showAnimation()
{
    CCArray* buttons = _buttons_menu->getChildren();
    unsigned int num = buttons->capacity();
    for(auto& item:_buttons)
    {
        float scale_button = item->getScale();
        item->setScale(scale_button*0.9);
        item->setAnchorPoint(ccp(0.5, 0.5));
        item->runAction(CCEaseElasticOut::create(
                                  CCScaleTo::create(0.7f, scale_button),
                                  0.4f));
    }

    float scale_bronze = _bronze_star->getScale();
    _bronze_star->setScale(scale_bronze*0.9);
    _bronze_star->runAction(CCEaseElasticOut::create(
                                CCScaleTo::create(0.7f, scale_bronze),
                                0.4f));

    float scale_silver = _silver_star->getScale();
    _silver_star->setScale(scale_silver*0.9);
    _silver_star->runAction(CCEaseElasticOut::create(
                                CCScaleTo::create(0.7f, scale_silver),
                                0.4f));

    float scale_gold = _gold_star->getScale();
    _gold_star->setScale(scale_gold*0.9);
    _gold_star->runAction(CCEaseElasticOut::create(
                                CCScaleTo::create(0.7f, scale_gold),
                                0.4f));

    auto button_show = [](){return CCFadeTo::create(0.15f, 255);};


    float scale_score = _score_label->getScale();
    _score_label->setScale(scale_score*0.9);
    _score_label->runAction(CCEaseElasticOut::create(
                                CCScaleTo::create(0.7f, scale_score),
                                0.4f));

    float scale_hint = _hint_number_text->getScale();
    _hint_number_text->setScale(scale_hint*0.9);
    _hint_number_text->runAction(CCEaseElasticOut::create(
                                CCScaleTo::create(0.7f, scale_hint),
                                0.4f));


    _progress->setOpacity(0);
    _progress->runAction(button_show());
    _progress_timer->setOpacity(0);
    _progress_timer->runAction(button_show());


}

void LevelScene::hideEverything(cocos2d::CCCallFunc* callback)
{
    Tutorial::getInstance()->deleteTutorialPath();
    _flow_game->endGame();
    //CCFadeTo* game_hide = CCFadeTo::create(0.4f, 0);
    float animation_time = 0.1f;
    _flow_game->stopAllActions();
    _flow_game->runAction(CCScaleTo::create(animation_time, _flow_game->getScaleX(), 0));

    //_pop_up_manager.backAction();

    float delay = animation_time*1.2f;
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    callback,
                    NULL));
    //this->runAction(callback);
}

void LevelScene::hideEverythingAndBack(cocos2d::CCCallFunc *callback)
{
    //Tutorial::getInstance()->deleteTutorialPath();
    _flow_game->endGame();
    auto button_hide = [](){return CCFadeTo::create(0.15f, 0);};

    float animation_time = 0.1f;
    _flow_game->stopAllActions();
    _flow_game->runAction(CCScaleTo::create(animation_time, _flow_game->getScaleX(), 0));


    _progress_timer->runAction(button_hide());
    _progress->runAction(button_hide());

    _score_label->runAction(button_hide());

    //_back_menu->runAction(button_hide());


    _bronze_star->runAction(button_hide());
    _silver_star->runAction(button_hide());
    _gold_star->runAction(button_hide());

    //hints and anothers
    //_buttons_menu->runAction(button_hide);
    CCFadeTo* logo_fade = CCFadeTo::create(0.15f, 0);
    _buttons_menu->setCascadeOpacityEnabled(true);
    _buttons_menu->runAction(logo_fade);


    float delay = 0.3f;
    this->runAction(
                CCSequence::create(
                    CCDelayTime::create(delay),
                    callback,
                    NULL));
}

void LevelScene::keyBackClicked()
{

    this->stopAction(_hint_action);
    Tutorial::getInstance()->deleteTutorialPath();
    //_last_scene = 0;
    if(!_pop_up_manager.backAction())
    {
        this->hideEverythingAndBack(CCCallFunc::create(
                                        this, callfunc_selector(LevelScene::doGoToCollection)));

    }
}
