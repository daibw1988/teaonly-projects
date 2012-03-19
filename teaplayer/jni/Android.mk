#####################################################################
# the build script for android libjingle
#

LOCAL_PATH:= $(call my-dir)

###########################################################
# the third prebuild library for linking
#
include $(CLEAR_VARS) 
LOCAL_MODULE := libffmpeg-prebuild	
LOCAL_SRC_FILES := libffmpeg.so
LOCAL_MODULE_TAGS := prebuild
include $(PREBUILT_SHARED_LIBRARY)

###########################################################
# build android libteaony 
# libteaonly: clone from libjingle, for generat NDK appplicaton
#      threads, socket, signal/slot and messages, etc.
#
include $(CLEAR_VARS)
LOCAL_MODULE := libteaonly
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := -O2 -Werror -Wall -DHAMMER_TIME=1 -DLOGGING=1 -DHASHNAMESPACE=__gnu_cxx -DHASH_NAMESPACE=__gnu_cxx -DPOSIX -DDISABLE_DYNAMIC_CAST -D_REENTRANT -DOS_LINUX=OS_LINUX -DLINUX -D_DEBUG  -DANDROID
include $(LOCAL_PATH)/lib_build_android.mk
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)


###########################################################
# teaplayer based on ffmpeg
#

include $(CLEAR_VARS)
LOCAL_MODULE    := teaplayer
LOCAL_CPPFLAGS := -Werror -Wall -DANDROID -DPOSIX -DLINUX -DOLD_FFMPEG 
LOCAL_SHARED_LIBRARIES := libffmpeg-prebuild libteaonly
LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/ffmpeg $(LOCAL_PATH)/app $(LOCAL_PATH)/framework $(LOCAL_PATH)/ ./  
LOCAL_LDLIBS := -llog -ljnigraphics -lz -lm

include $(LOCAL_PATH)/app_build_android.mk
include $(BUILD_SHARED_LIBRARY)
