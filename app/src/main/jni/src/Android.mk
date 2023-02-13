LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := NativeActivity
LOCAL_STATIC_LIBRARIES := vm_full_prebuilt \
    kuaixie

LOCAL_LDLIBS := -ljnigraphics
LOCAL_SRC_FILES := main.c \
	keypad.c \
	emulator.c \
	vm.c \
	msgqueue.c \
	screen.c \
	tools/network.c \
	tools/TimeUtils.c \
	tools/font_sky16.c \
	tools/encode.c \
	interface.c
	
include $(BUILD_SHARED_LIBRARY)

