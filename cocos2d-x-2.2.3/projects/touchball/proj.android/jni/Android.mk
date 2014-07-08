LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/datas/ClientData.cpp \
                   ../../Classes/present/PresentManager.cpp \
                   ../../Classes/present/BasicPresent.cpp \
                   ../../Classes/present/FloatMumberPresent.cpp \
                   ../../Classes/script/GameScript.cpp \
                   ../../Classes/script/LuaReg.cpp \
                   ../../Classes/script/LuaRegClass.cpp \
                   ../../Classes/script/LuaRegDef.cpp \
                   ../../Classes/script/ScriptObject.cpp \
                   ../../Classes/utils/UserLocalData.cpp \
                   ../../Classes/ui/DialogEvent.cpp \
                   ../../Classes/ui/DialogManager.cpp \
                   ../../Classes/ui/ScoreDialog.cpp \
                   ../../Classes/ui/BasicDialog.cpp \
                   ../../Classes/ui/PropsDialog.cpp \
                   ../../Classes/ui/LevelDialog.cpp \
                   ../../Classes/ui/BattleResultDialog.cpp\
                   ../../Classes/scene/SceneData.cpp \
                   ../../Classes/scene/SceneManager.cpp \
                   ../../Classes/scene/gameScene.cpp \
                   ../../Classes/scene/props/BasicProps.cpp \
                   ../../Classes/scene/props/PropsManager.cpp \
                   ../../Classes/scene/ball/ball.cpp \
                   ../../Classes/scene/ball/ballManager.cpp \
                   ../../Classes/scene/map/ballMap.cpp \
                   ../../Classes/scene/map/touchMap.cpp \
                   ../../Classes/scene/map/mapCell.cpp \
                   ../../Classes/scene/path/pathFinder.cpp \
                   ../../Classes/hawksdk/HawkZlib.cpp \
                   ../../Classes/hawksdk/HawkDiskFile.cpp \
                   ../../Classes/hawksdk/HawkMalloc.cpp \
                   ../../Classes/hawksdk/HawkFile.cpp \
                   ../../Classes/hawksdk/HawkMemoryFile.cpp \
                   ../../Classes/hawksdk/HawkBinCfgFile.cpp \
                   ../../Classes/script/lua/tolua_fix.c \
                   ../../Classes/script/lua/lapi.c \
                   ../../Classes/script/lua/lauxlib.c \
                   ../../Classes/script/lua/lbaselib.c \
                   ../../Classes/script/lua/lcode.c \
                   ../../Classes/script/lua/ldblib.c \
                   ../../Classes/script/lua/ldebug.c \
                   ../../Classes/script/lua/ldo.c \
                   ../../Classes/script/lua/ldump.c \
                   ../../Classes/script/lua/lfunc.c\
                   ../../Classes/script/lua/lgc.c \
                   ../../Classes/script/lua/linit.c \
                   ../../Classes/script/lua/liolib.c \
                   ../../Classes/script/lua/llex.c \
                   ../../Classes/script/lua/lmathlib.c \
                   ../../Classes/script/lua/lmem.c \
                   ../../Classes/script/lua/loadlib.c \
                   ../../Classes/script/lua/lobject.c \
                   ../../Classes/script/lua/lopcodes.c \
                   ../../Classes/script/lua/loslib.c \
                   ../../Classes/script/lua/lparser.c \
                   ../../Classes/script/lua/lstate.c \
                   ../../Classes/script/lua/lstring.c \
                   ../../Classes/script/lua/lstrlib.c \
                   ../../Classes/script/lua/ltable.c \
                   ../../Classes/script/lua/ltablib.c \
                   ../../Classes/script/lua/ltm.c \
                   ../../Classes/script/lua/lua.c \
                   ../../Classes/script/lua/lundump.c \
                   ../../Classes/script/lua/lvm.c \
                   ../../Classes/script/lua/lzio.c \
                   ../../Classes/script/lua/print.c \
                   ../../Classes/script/tolua/tolua_event.c \
                   ../../Classes/script/tolua/tolua_is.c \
                   ../../Classes/script/tolua/tolua_map.c \
                   ../../Classes/script/tolua/tolua_push.c \
                   ../../Classes/script/tolua/tolua_to.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
