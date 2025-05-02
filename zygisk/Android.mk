LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := fp_bypass
LOCAL_SRC_FILES := fp_bypass.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_LDLIBS := -llog -lstdc++
include $(BUILD_SHARED_LIBRARY)
