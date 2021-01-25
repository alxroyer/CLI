# Copyright (c) 2006-2018, Alexis Royer, http://alexis.royer.free.fr/CLI
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
#       and/or other materials provided with the distribution.
#     * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
#       without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# Default goal
.DEFAULT_GOAL = install
.PHONY: native.default
native.default: $(.DEFAULT_GOAL) ;


# Includes
CLI_DIR := ../../..
include _vars.mak
include $(CLI_DIR)/cpp/build/make/_vars.mak

PROJECT = native
PRODUCT_TYPE = DYN_LIB
PRODUCT = $(JAVA_DYN_LIB)
PROJ_DEPS = jni.mak

# Sources
SRC_DIR = $(NATIVE_DIR)
CPP_FILES += $(wildcard $(CLI_DIR)/cpp/src/*.cpp)
ifeq ($(TARGET),Cygwin)
	CPP_FILES := $(filter-out $(CLI_DIR)/cpp/src/ncurses_console.cpp,$(CPP_FILES))
endif
ifeq ($(TARGET),Linux)
	CPP_FILES := $(filter-out $(CLI_DIR)/cpp/src/win_console.cpp,$(CPP_FILES))
endif
CPP_FILES += $(wildcard $(SRC_DIR)/*.cpp)

# Compilation flags
ifeq ($(TARGET),Cygwin)
	# See https://stackoverflow.com/questions/6034390/compiling-with-cython-and-mingw-produces-gcc-error-unrecognized-command-line-o#6035864
	# "It sounds like GCC 4.7.0 has finally removed the deprecated -mno-cygwin option"
	#PROJ_CPP_FLAGS += -mno-cygwin
	PROJ_CPP_FLAGS += -DCLI_NO_REGEX -DCLI_WIN_NETWORK
	# See https://stackoverflow.com/questions/27891478/error-when-compiling-in-cygwin
	#PROJ_CPP_FLAGS += -D__int64=int64_t
	# See: https://stackoverflow.com/questions/8413290/using-java-jni-on-cygwin#47069933
	# "I couldn't get JNI to work with Cygwin's g++ -- that induces a dependency on cygwin1.dll, which clashes with the JNI mechanism, causing a crash. The -mno-cygwin flag is no longer supported. But using /bin/x86_64-w64-mingw32-g++.exe fixed the problem for me."
	CXX = x86_64-w64-mingw32-g++
endif
CPP_FLAGS = $(CPP_DEBUG_FLAG) $(CPP_OS_FLAGS) $(PROJ_CPP_FLAGS)

# Include directories
PROJ_INCLUDES += -I$(CLI_DIR)/cpp/include
PROJ_INCLUDES += -isystem "$(JDK_DIR)/include"
ifeq ($(TARGET),Cygwin)
	PROJ_INCLUDES += -isystem "$(JDK_DIR)/include/win32"
deps: PROJ_INCLUDES += -isystem /usr/include
endif
ifeq ($(TARGET),Linux)
	PROJ_INCLUDES += -isystem "$(JDK_DIR)/include/linux"
endif
INCLUDES = $(PROJ_INCLUDES)

# Linking
ifeq ($(TARGET),Linux)
	PROJ_LIBS = -lncurses
endif
ifeq ($(TARGET),Cygwin)
	# See https://stackoverflow.com/questions/6034390/compiling-with-cython-and-mingw-produces-gcc-error-unrecognized-command-line-o/6035864#6035864
	# "It sounds like GCC 4.7.0 has finally removed the deprecated -mno-cygwin option"
	#PROJ_LIBS += -mno-cygwin -L/cygdrive/c/cygwin/lib/gcc/i686-pc-mingw32/3.4.4
	PROJ_LIBS = -lwsock32
endif

# Other project variables
AUTO_DEPS = yes
INT_DIR = $(OUT_DIR)/$(TARGET)$(CXX)/__$(PROJECT)
CLEAN_DIR = $(INT_DIR) $(OUT_DIR)/$(TARGET)$(CXX)

# _build.mak inclusion
include $(CLI_DIR)/cpp/build/make/_build.mak

.PHONY: install
install: build ;

# Because of the compilation with x86_64-w64-mingw32-g++, the cli.dll library depends on a couple of mingw32 libs.
# We choose to install these libraries next to the one generated in as much as you would like to deliver them as well with your final application.
ifeq ($(TARGET),Cygwin)
install: $(OUT_DIR)/libstdc++-6.dll
install: $(OUT_DIR)/libgcc_s_seh-1.dll
clean: clean.cygwin

MINGW_BIN_DIR = /cygdrive/c/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/bin

$(OUT_DIR)/libstdc++-6.dll: $(MINGW_BIN_DIR)/libstdc++-6.dll
	cp $< $@

$(OUT_DIR)/libgcc_s_seh-1.dll: $(MINGW_BIN_DIR)/libgcc_s_seh-1.dll
	cp $< $@

.PHONY: clean.cygwin
clean.cygwin:
	rm -f $(OUT_DIR)/libstdc++-6.dll
	rm -f $(OUT_DIR)/libgcc_s_seh-1.dll
endif

# Dependencies.
deps: deps.pre
# Ensure dependencies for filtered-out sources will be computed whatever
ifeq ($(TARGET),Cygwin)
deps: CPP_FILES += $(CLI_DIR)/cpp/src/ncurses_console.cpp
endif
ifeq ($(TARGET),Linux)
deps: CPP_FILES += $(CLI_DIR)/cpp/src/win_console.cpp
endif

.PHONY: deps.pre
deps.pre:
	$(MAKE) -f libclijava.mak
	$(MAKE) -f jni.mak

include $(AUTO_DEPS_FILE)
