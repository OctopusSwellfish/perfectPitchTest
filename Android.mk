LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := fpga-push-switch-jni
LOCAL_SRC_FILES := fpga-push-switch-jni.c
LOCAL_LDLIBS    := -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)
