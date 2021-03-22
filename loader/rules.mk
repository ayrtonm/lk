LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_SRCS += \
	$(LOCAL_DIR)/loader.c

MODULE_DEPS := \
	lib/elf \
	lib/fs

include make/module.mk
