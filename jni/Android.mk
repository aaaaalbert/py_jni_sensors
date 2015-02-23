# File based on code samples licensed as follows:
#
# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)



# Include `libpython2.7.so` as a prebuilt module.
# We will later use the LOCAL_MODULE name to refer to it as a shared lib.
#
# (See file:///home/aaaaa/devel/android-ndk-r10d/docs/Programmers_Guide/html/md_3__key__topics__libraries__p_r_e_b_u_i_l_t_s.html )
include $(CLEAR_VARS)

LOCAL_MODULE := python2.7
LOCAL_SRC_FILES := ../python2.7-libs/lib/libpython2.7.so
# This is where the header files declaring the module's functions live:
LOCAL_EXPORT_C_INCLUDES := python2.7-libs/include

include $(PREBUILT_SHARED_LIBRARY)



# Build our module
include $(CLEAR_VARS)

# LOCAL_C_INCLUDES are automatically set up through LOCAL_EXPORT_C_INCLUDES 
# above, and our `#include`ing Python.h.

# XXX Make python2.7 a static include so as to work around load_library errors
# https://stackoverflow.com/questions/12873261/ld-library-path-ignored-on-android-sometimes/12927020#12927020
LOCAL_STATIC_LIBRARIES := python2.7
LOCAL_LDLIBS    := -llog
LOCAL_MODULE    := embedded_python_test
LOCAL_SRC_FILES := embedded_python_test.c

include $(BUILD_SHARED_LIBRARY)

