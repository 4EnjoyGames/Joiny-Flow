LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := hellocpp_shared
LOCAL_MODULE_FILENAME := libhellocpp
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/Core/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Logic/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Layers/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Scenes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Flow/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/Localization/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/StoreBridge/*.cpp)

LOCAL_SRC_FILES := hellocpp/main.cpp

LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_CPPFLAGS += -std=gnu++0x
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) $(call import-module,cocos2dx)

