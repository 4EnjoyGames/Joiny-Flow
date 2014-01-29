TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += ../../../cocos2dx \
    ../../../cocos2dx/include \
    ../../../cocos2dx/kazmath/include \
    ../../../cocos2dx/platform/win32 \
    ../../../cocos2dx/platform/third_party/win32/OGLES \
    ../Classes \
    ../../../CocosDenshion/include

include(../libraries/ADLib/ADLib.pri)

#DEFINES +=NDEBUG
CONFIG(debug, debug|release) {
    LIBS += -L../../../../Debug.win32/ -lglew32
    LIBS += -L../../../../Debug.win32/ -llibcocos2d
    LIBS += -L../../../../Debug.win32/ -llibCocosDenshion
    LIBS += -lopengl32
} else {
    LIBS += -L../../../Release.win32/ -llibcocos2d
    LIBS += -L../../../Release.win32/ -lglew32
    LIBS += -L../../../Release.win32/ -llibCocosDenshion
    LIBS += -lopengl32
    DEFINES +=NDEBUG
}
LIBS += -luser32
QMAKE_CXXFLAGS_DEBUG += -wd4267 -wd4251 -wd4244
QMAKE_CXXFLAGS_RELEASE += -wd4267 -wd4251 -wd4244

DEFINES += WIN32 \
            _DEBUG \
            _WINDOWS \
            COCOS2D_DEBUG="1" \
            _CRT_SECURE_NO_WARNINGS \
            _SCL_SECURE_NO_WARNINGS

SOURCES += main.cpp \
    ../Classes/AppDelegate.cpp \
    ../Classes/Core/AnimatedMenuItem.cpp \
    ../Classes/Core/GraphicsManager.cpp \
    ../Classes/Core/MenuSpriteBatch.cpp \
    ../Classes/Layers/BackgroundHolder.cpp \
    ../Classes/Core/CCScrollView.cpp \
    ../Classes/Layers/DebugLayer.cpp \
    ../Classes/Scenes/Loading.cpp \
    ../Classes/Core/MusicSettings.cpp \
    PurchaseEmulator.cpp \
    ../Classes/Layers/PopUpWindow.cpp \
    ../Classes/Layers/YesNoDialog.cpp \
    ../Classes/Flow/FlowTable.cpp \
    ../Classes/Flow/FlowRendererNode.cpp \
    ../Classes/Flow/FlowRenderer.cpp \
    ../Classes/Flow/FlowPointState.cpp \
    ../Classes/Flow/FlowGame.cpp \
    ../Classes/Logic/JoinyLevel.cpp \
    ../Classes/Logic/JoinyCollection.cpp \
    ../Classes/Logic/LevelManager.cpp \
    ../Classes/Core/FileUtils.cpp \
    ../Classes/Scenes/LevelScene.cpp \
    ../Classes/Scenes/SelectLevel.cpp \
    ../Classes/Scenes/MainScene.cpp \
    ../Classes/Scenes/SelectCollection.cpp \
    ../Classes/Scenes/SettingScene.cpp \
    ../Classes/GameInfo.cpp \
    ../Classes/BackButton.cpp \
    ../Classes/Localization/CCLocalizedString.cpp \
    ../Classes/Core/DrawLayer.cpp \
    ../Classes/Scenes/DeveloperScene.cpp

HEADERS += \
    ../Classes/AppMacros.h \
    ../Classes/AppDelegate.h \
    main.h \
    ../Classes/Core/AnimatedMenuItem.h \
    ../Classes/Core/GraphicsManager.h \
    ../Classes/cocos2d-A.h \
    ../Classes/Core/MenuSpriteBatch.h \
    ../Classes/Layers/BackgroundHolder.h \
    ../Classes/Core/CCScrollView.h \
    ../Classes/Layers/DebugLayer.h \
    ../Classes/Logic/RW.h \
    ../Classes/Core/CCTime.h \
    ../Classes/Core/MusicSettings.h \
    ../Classes/Layers/PopUpWindow.h \
    ../Classes/Store.h \
    ../Classes/Layers/YesNoDialog.h \
    ../Classes/Flow/FlowTable.h \
    ../Classes/Flow/FlowRendererNode.h \
    ../Classes/Flow/FlowRenderer.h \
    ../Classes/Flow/FlowPointState.h \
    ../Classes/Flow/FlowPointPair.h \
    ../Classes/Flow/FlowPoint.h \
    ../Classes/Flow/FlowGame.h \
    ../Classes/Flow/FlowColor.h \
    ../Classes/Flow/JoinyPair.h \
    ../Classes/Logic/JoinyLevel.h \
    ../Classes/Logic/RWGeneric.h \
    ../Classes/Logic/JoinyCollection.h \
    ../Classes/Logic/LevelManager.h \
    ../Classes/Core/FileUtils.h \
    ../Classes/Scenes/LevelScene.h \
    ../Classes/Scenes/SelectLevel.h \
    ../Classes/Scenes/MainScene.h \
    ../Classes/Core/Screen.h \
    ../Classes/Scenes/SelectCollection.h \
    ../Classes/Scenes/SettingScene.h \
    ../Classes/GameInfo.h \
    ../Classes/BackButton.h \
    ../Classes/Localization/CCLocalizedString.h \
    ../Classes/Core/DrawLayer.h \
    ../Classes/Scenes/DeveloperScene.h


