#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include "Core/Browser.h"
#include "Core/Statistics.h"
#include "Logic/Language.h"
#include "Core/Notification.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	LOGD("Here");
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
	LOGD("Here");
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

        AppDelegate *pAppDelegate = new AppDelegate();
        CCApplication::sharedApplication()->run();
    }
    else
    {
        ccDrawInit();
        ccGLInvalidateStateCache();
        
        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVNET_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues(); 
    }
}
void Browser::openURL(const URLId id)
{
	 JniMethodInfo methodInfo;
	 if (! JniHelper::getStaticMethodInfo(methodInfo, "com/x4enjoy/Browser", "openURL", "(I)V"))
	 {
	  return;
	 }

	 methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, id);
	 methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
void Statistics::statisticEvent(const StatisticsEvent id)
{
	JniMethodInfo methodInfo;
	 if (! JniHelper::getStaticMethodInfo(methodInfo, "com/x4enjoy/Statistics", "statisticEvent", "(I)V"))
	 {
	  return;
	 }

	 methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, id);
	 methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
void Statistics::detailStatisticEvent(const StatisticsEvent id,
								  const unsigned int level_id,
                                  const unsigned int collection_id,
                                  const unsigned int stamps_num)
{
	JniMethodInfo methodInfo;
	 if (! JniHelper::getStaticMethodInfo(methodInfo, "com/x4enjoy/Statistics", "detailStatisticEvent", "(IIII)V"))
	 {
	  return;
	 }

	 methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, id, level_id+1, collection_id, stamps_num);
	 methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

Language::Languages Language::getDeviceLanguage()
{
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo, "com/x4enjoy/Language", "getLanguage", "()I"))
	{
		LOGD("Lang error");
		return Language::English;
	}

	jint x = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);


    return static_cast<Language::Languages>(x);
}

void Notification::notify(const Type t)
{
	JniMethodInfo methodInfo;
	 if (! JniHelper::getStaticMethodInfo(methodInfo, "com/x4enjoy/Notification", "notify", "(I)V"))
	 {
	  return;
	 }

	 methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, t);
	 methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
}
