include $(call all-subdir-makefiles)

LOCAL_PATH  := $(call my-dir)

# libeblearn
include $(CLEAR_VARS)

LOCAL_LDLIBS     := -lm -llog -ljnigraphics
LOCAL_MODULE     := libeblearn
SRC              := ../../core/libeblearn/src
LOCAL_C_INCLUDES := $(CORE)/libeblearn/include/ \
		    $(CORE)/libidx/include/
LOCAL_SRC_FILES  := \
	       	    $(SRC)/ebl_arch.cpp \
		    $(SRC)/ebl_basic.cpp \
		    $(SRC)/ebl_cost.cpp \
		    $(SRC)/ebl_layers.cpp \
		    $(SRC)/ebl_logger.cpp \
		    $(SRC)/ebl_machines.cpp \
		    $(SRC)/ebl_nonlinearity.cpp \
		    $(SRC)/ebl_states.cpp \
		    $(SRC)/ebl_transfer.cpp \
		    $(SRC)/ebl_utils.cpp \
		    $(SRC)/detector.cpp \
		    entries.cpp

		    # $(SRC)/ebl_trainer.cpp \
		    # $(SRC)/datasource.cpp \

LOCAL_SHARED_LIBRARIES := libidx

include $(BUILD_SHARED_LIBRARY)
