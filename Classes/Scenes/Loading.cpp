#include "Loading.h"
//#include "SelectLevel.h"
#include "MainScene.h"
#include "Core/CCTime.h"
#include "SimpleAudioEngine.h"
#include "Logic/Language.h"
#include "../Core/MusicSettings.h"

#include "StoreBridge/cocos2dx_StoreController.h"
#include "StoreBridge/cocos2dx_EventHandlers.h"
#include "Logic/LevelManager.h"

Loading::Loading() : _sprites_to_load(0), _loaded_sprites(0)
{
}
CCScene* Loading::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    Loading *layer = Loading::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void Loading::drawEverything(float)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCSprite* logo = CCSprite::create("universal/x4Enjoy.png");
    this->addChild(logo);

    logo->setPosition(ccp(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height/2));

#ifdef CC_WIN8_METRO
	this->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Loading::loadAllAsync), this, 0.1, 0, 0, false);
#endif

}

void Loading::loadSprite(const char* name)
{
    _sprites_to_load++;

    CCTextureCache::sharedTextureCache()->
            addImageAsync(name, this, callfuncO_selector(Loading::loadingCallBack));
	
}
void Loading::hideLogo()
{
    this->runAction(CCCallFunc::create(this, callfunc_selector(Loading::openMenu)));
}

void Loading::openMenu()
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());   
}

void Loading::loadingCallBack(CCObject *)
{
    _loaded_sprites++;

    if(_loaded_sprites == _sprites_to_load)
    {
        const float splash_screen_time = 1;
        float loading_time = GetMilliCount() - _started_in;
        float still_wait = splash_screen_time - float(loading_time)/1000;
        if(still_wait > 0)
        {
            this->runAction(CCSequence::create(
                                CCDelayTime::create(still_wait),
                                CCCallFunc::create(this, callfunc_selector(Loading::hideLogo)),
                                NULL));
        }
        else
        {
            hideLogo();
        }

    }
}

#include "Logic/RW.h"
#include "AppDelegate.h"
void Loading::initRW()
{
	//TODO: write cleanup
    RW::onInit();

    /**
         * The initialization of StoreController. This is the first and ONLY time it's initialized.
         **/

    cocos2dx_StoreController::initialize("DIb8Vy95lnJH4PtgUQro");
    cocos2dx_EventHandlers::getInstance()->addHandler(AppDelegate::getPurchaseHandler());

}
void Loading::addAll()
{

//	addSpriteToLoad("common.png");
//	addPlistToLoad("common.plist");

//    addSpriteToLoad("loading_screen.png");
//	addPlistToLoad("loading_screen.plist");




//	addSpriteToLoad("font/mathisfun.png");
//	addSpriteToLoad("font/mathisfun_digits.png");
//	addPlistToLoad("font/mathisfun_digits.plist");

//	addSpriteToLoad("level/input_space.png");
//	addPlistToLoad("level/input_space.plist");

//	addSpriteToLoad("level/keyboard.png");
//	addPlistToLoad("level/keyboard.plist");

//    addSpriteToLoad(Language::localizeFileName("level/level_end.png").c_str());
//    addPlistToLoad(Language::localizeFileName("level/level_end.plist").c_str());

//	addSpriteToLoad("level/level_end_menu.png");
//	addPlistToLoad("level/level_end_menu.plist");

//    addSpriteToLoad(Language::localizeFileName("level/found_solutions.png").c_str());
//    addPlistToLoad(Language::localizeFileName("level/found_solutions.plist").c_str());

//	addSpriteToLoad("level/stamps.png");
//	addPlistToLoad("level/stamps.plist");

//	addSpriteToLoad("level_scene/ellements.png");
//	addPlistToLoad("level_scene/ellements.plist");

//	addSpriteToLoad("main_menu/images.png");
//	addPlistToLoad("main_menu/images.plist");

//    addSpriteToLoad(Language::localizeFileName("main_menu/menu.png").c_str());
//    addPlistToLoad(Language::localizeFileName("main_menu/menu.plist").c_str());

//    addSpriteToLoad("sheet_menu.png");
//    addPlistToLoad("sheet_menu.plist");


//    addSpriteToLoad("select_collection/new_collection.png");
//    addPlistToLoad("select_collection/new_collection.plist");

//    addSpriteToLoad(Language::localizeFileName("select_collection/images.png").c_str());
//    addPlistToLoad(Language::localizeFileName("select_collection/images.plist").c_str());

//	addSpriteToLoad("select_level/marks.png");
//	addPlistToLoad("select_level/marks.plist");

//    addSpriteToLoad(Language::localizeFileName("select_level/menu_name.png").c_str());
//    addPlistToLoad(Language::localizeFileName("select_level/menu_name.plist").c_str());

//    addSpriteToLoad(Language::localizeFileName("settings/developers_menu_name.png").c_str());
//    addPlistToLoad(Language::localizeFileName("settings/developers_menu_name.plist").c_str());

//	addSpriteToLoad("settings/settings_developers_ad.png");
//	addPlistToLoad("settings/settings_developers_ad.plist");

//    addSpriteToLoad(Language::localizeFileName("settings/settings_menu.png").c_str());
//    addPlistToLoad(Language::localizeFileName("settings/settings_menu.plist").c_str());

//    addSpriteToLoad(Language::localizeFileName("settings/settings_name.png").c_str());
//    addPlistToLoad(Language::localizeFileName("settings/settings_name.plist").c_str());

////	addSpriteToLoad("stop_level/sheet.png");
////	addPlistToLoad("stop_level/sheet.plist");

//    addSpriteToLoad(Language::localizeFileName("stop_level/stop_level_menu.png").c_str());
//    addPlistToLoad(Language::localizeFileName("stop_level/stop_level_menu.plist").c_str());
	
    RW::getLevelManager().loadSettings();
    addSoundToLoad("music/silly_fun.mp3");
    addMusicToLoad(MusicSettings::BACKGROUND_MUSIC);

    RW::getLevelManager().makePreviews(5,100);

//    const CollectionsArr& arr = RW::getCollections();
//    for(CollectionsArr::const_iterator it = arr.begin(); it != arr.end(); ++it)
//    {
//        Collection* a = it->second;
//        std::stringstream ss;
//        std::stringstream ss_f;
//        ss << "medium/select_collection/coll_" << a->getCollectionID() << ".png";
//        ss_f << "medium/select_collection/clabel_" << a->getCollectionID() << ".png";
//        addSpriteToLoad(ss.str().c_str());
//        addSpriteToLoad(Language::localizeFileName(ss_f.str()).c_str());
//    }
//    addSpriteToLoad("medium/select_collection/coll_lock.png");
//    addSpriteToLoad(Language::localizeFileName("medium/select_collection/clabel_locked.png").c_str());
}
void Loading::addSpriteToLoad(const char* sp)
{
	_q_sprites_to_load.push(sp);
}
void Loading::addMusicToLoad(const char* s)
{
	_q_music_to_load.push(s);
}
void Loading::addSoundToLoad(const char* s)
{
	_q_sound_to_load.push(s);
}
void Loading::addPlistToLoad(const char* s)
{
	_q_plists_to_load.push(s);
}
void Loading::loadAll()
{
	CCTextureCache* cache = CCTextureCache::sharedTextureCache();
	while(!_q_sprites_to_load.empty())
	{
		std::string str = _q_sprites_to_load.front();
		cache->addImage(str.c_str());
		_q_sprites_to_load.pop();
	}

	CocosDenshion::SimpleAudioEngine* engine = CocosDenshion::SimpleAudioEngine::sharedEngine();
	while(!_q_music_to_load.empty())
	{
		std::string str = _q_music_to_load.front();
		engine->preloadBackgroundMusic(str.c_str());
		_q_music_to_load.pop();
	}

	while(!_q_sound_to_load.empty())
	{
		std::string str = _q_sound_to_load.front();
		engine->preloadEffect(str.c_str());
		_q_sound_to_load.pop();
	}

	CCSpriteFrameCache* plist_cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	while(!_q_plists_to_load.empty())
	{
		std::string str = _q_plists_to_load.front();
		plist_cache->addSpriteFramesWithFile(str.c_str());
		_q_plists_to_load.pop(); 
	}
}
void Loading::onLoadFinished()
{
	const float splash_screen_time = float(SPLASH_SCREEN_TIME);
    float loading_time = float(GetMilliCount()) - _started_in;
    float still_wait = splash_screen_time - float(loading_time)/1000;
    if(still_wait > 0)
    {
        this->runAction(CCSequence::create(
                            CCDelayTime::create(still_wait),
                            CCCallFunc::create(this, callfunc_selector(Loading::hideLogo)),
                            NULL));
    }
    else
    {
        hideLogo();
    }
}

#ifdef CC_WIN8_METRO
#include <ppltasks.h>
void Loading::loadAllAsync(float)
{
	using namespace concurrency;
	create_task(create_async([this](){
		this->addAll();
		this->loadAll();
	})).then([this](){
		this->onLoadFinished();
	});
}
#else
void Loading::loadAllAsync(float)
{
    addAll();
    loadAll();
    onLoadFinished();
}

#endif

bool Loading::init()
{
    if (!DrawLayer::init())
    {
        return false;
    }
    _started_in = GetMilliCount();

    
//#ifndef CC_WIN8_PHONE
#ifdef CC_WIN8_METRO
	this->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Loading::drawEverything), this, 0.1, 0, 0, false);
	initRW();
#else
	drawEverything(0);
	initRW();
    this->getScheduler()->scheduleSelector(SEL_SCHEDULE(&Loading::loadAllAsync), this, 0.1, 0, 0, false);
#endif
    return true;
}
