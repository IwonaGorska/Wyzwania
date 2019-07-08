LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
COCOS_ROOT := /Users/iwonagorska/Documents/wazne/cocos2d-x-3.17_git


$(call import-add-path,$(COCOS_ROOT))

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/hellocpp/main.cpp \
                     $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                                       $(LOCAL_PATH)/../../../Classes/AppItem.cpp \
                                       $(LOCAL_PATH)/../../..//Classes/Definitions.h \
                                      $(LOCAL_PATH)/../../../Classes/HitDetectHelper.cpp \
                                       $(LOCAL_PATH)/../../../Classes/LoadingScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/MenuScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/NativeHelperAndroid.cpp \
                                        $(LOCAL_PATH)/../../../Classes/Utils.cpp \
                                       $(LOCAL_PATH)/../../../Classes/PlayScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/NativeHelper.h \
                                       $(LOCAL_PATH)/../../../Classes/HensScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/WaterScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/BlocksScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/BasketScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/InstructionsScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/ScoreScene.cpp \
                                       $(LOCAL_PATH)/../../../Classes/AvatarsScene.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_WHOLE_STATIC_LIBRARIES := cc_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module, prebuilt/android)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END