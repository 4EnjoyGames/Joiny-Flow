#include "LevelScene.h"
#include "AppMacros.h"
USING_NS_CC;


#include "Logic/Language.h"
#include "Core/Browser.h"
#include "Flow/FlowGame.h"
#include "Flow/Generator/FGenerator.h"
#include "Flow/JoinyPair.h"
#include "Scenes/SelectLevel.h"
#include <algorithm>

class LevelScene::LevelEndPopUp : public PopUpWindow::Content
{

public:
    enum Mode
    {
        NotEnough,
        LevelEnd
    };


    LevelEndPopUp(const Mode mode, const FlowScore score=0, const unsigned int stars=0, const JoinyLevel* level=0, LevelScene* parent=0)
        : _mode(mode), _score(score), _stars(stars), _level(level), _parent(parent)
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
        float scaled = CCDirector::sharedDirector()->getContentScaleFactor();

        CCSize size = parent->getContentSize();
        float x_middle = size.width / 2;

        std::stringstream ss;
        if(_mode==NotEnough)
        {
            ss << "You need more score to finish" << std::endl;
            ss << _score << " needed" << std::endl;
        }
        else
        {
            ss << "Your score: " << _score << std::endl;
            ss << "Your medal: ";
            if(_stars == 1)
                ss << "BRONZE";
            else if(_stars == 2)
                ss << "SILVER";
            else if(_stars == 3)
                ss << "GOLD";
            ss << std::endl;
        }


        CCLabelTTF* label = CCLabelTTF::create(ss.str().c_str(),
                                               "Arial",
                                               50,
                                               CCSize(size.width*0.8f, 0),
                                               kCCTextAlignmentCenter);

        label->setPosition(ccp(x_middle, size.height*0.6f));
        parent->addChild(label);




        SpritesLoader menu_spl = GraphicsManager::getLoaderFor(0,
                                                               Language::localizeFileName("level_end_buttons.plist").c_str(),
                                                               Language::localizeFileName("level_end_buttons.png").c_str());
        MenuSpriteBatch* menu = MenuSpriteBatch::create(menu_spl);
        menu->setPosition(ccp(0,0));
        menu->setAnchorPoint(ccp(0,0));
        menu->setContentSize(size);
        parent->addChild(menu);



        float vertical = size.height * 0.2f;


        AnimatedMenuItem *play_more_item = AnimatedMenuItem::create(
                    menu_spl->loadSprite("play_more_button.png"),
                    this, menu_selector(Me::onPlayMore));



        play_more_item->setPosition(ccp(size.width*0.25,
                                        vertical));



        if(_mode == LevelEnd)
        {

            AnimatedMenuItem *next_level = AnimatedMenuItem::create(
                        menu_spl->loadSprite("next_level_button.png"),
                        this, menu_selector(Me::onNextLevel));
            next_level->setPosition(ccp(size.width*0.75,
                                        vertical));
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

void LevelScene::doGoToCollection()
{
    CCDirector::sharedDirector()->replaceScene(SelectLevel::scene(_current_level->getCollection()));
}

void LevelScene::doOpenNextLevel()
{
    CCDirector::sharedDirector()->replaceScene(LevelScene::scene(_next_level));

}

// on "init" you need to initialize your instance
bool LevelScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    //Get the size of the screen we can see
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    //Get the screen start of cordinates
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    float scaled = CCDirector::sharedDirector()->getContentScaleFactor();


    this->setKeypadEnabled(true);



    unsigned int N = _current_level->getBoardSize().x();
    FlowTable table(N,N);
    JoinyPuzzle task = _current_level->getPuzzle();

    _current_info = task._info;
    for(JoinyTask::iterator it = task._task.begin(); it!=task._task.end(); ++it)
    {
        JoinyPair& s = *it;
        if(s.getPoints().size() == 2)
        {
            table.addColor(s.getPoints()[0], s.getPoints()[1], s.getColor());
        }
    }

    std::stringstream ss;
    ss << "Bronze: " << task._info.getBronze() << std::endl;
    ss << "Silver: " << task._info.getSilver() << std::endl;
    ss << "Gold: " << task._info.getGold() << std::endl;
    ss << "Highscore: " << _current_level->getHighScore() << std::endl;



    CCLabelTTF* scores = CCLabelTTF::create(ss.str().c_str(), "Arial", 28);
    scores->setAnchorPoint(ccp(1,1));
    scores->setPosition(ccp(origin.x+visibleSize.width, origin.y+visibleSize.height));
    this->addChild(scores);

    _score_label = CCLabelTTF::create("0", "Arial", 80);

    _score_label->setAnchorPoint(ccp(0, 1));
    _score_label->setPosition(ccp(origin.x, origin.y + visibleSize.height));
    this->addChild(_score_label);

    _flow_game = FlowGame::create(table, FlowGame::DelegatePtr(new FlowDelegate(this)));
    CCSize render_size = _flow_game->getContentSize();

    _flow_game->setScale(MIN(visibleSize.width / render_size.width,
                         visibleSize.width / render_size.height));

    _flow_game->setAnchorPoint(ccp(0, 0.5f));
    _flow_game->setPosition(ccp(origin.x,origin.y+visibleSize.height / 2));
    this->addChild(_flow_game);

    return true;
}

void LevelScene::onScoreChanged(const FlowScore s)
{
    std::stringstream ss;
    ss << s;
    _score_label->setString(ss.str().c_str());

    _last_score = s;
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





