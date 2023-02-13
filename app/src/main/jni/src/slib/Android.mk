LOCAL_PATH := $(call my-dir)

MINI_VM_VERSION := 2011
VM_VERSION := 2018

#精简版虚拟机
include $(CLEAR_VARS)
LOCAL_MODULE := vm_mini_prebuilt
LOCAL_SRC_FILES := libmr_vm_mini.a
LOCAL_CFLAGS := -DMR_VERSION=$(MINI_VM_VERSION)
include $(PREBUILT_STATIC_LIBRARY)

#完整版虚拟机
include $(CLEAR_VARS)
LOCAL_MODULE := vm_full_prebuilt
LOCAL_SRC_FILES := libmr_vm_full.a
LOCAL_CFLAGS := -DMR_VERSION=$(VM_VERSION)
include $(PREBUILT_STATIC_LIBRARY)


#完整版虚拟机
include $(CLEAR_VARS)
LOCAL_MODULE := kuaixie
LOCAL_SRC_FILES := libapi.a
LOCAL_CFLAGS := -DMR_VERSION=$(VM_VERSION)
include $(PREBUILT_STATIC_LIBRARY)
