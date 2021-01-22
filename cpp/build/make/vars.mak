# Copyright (c) 2006-2007, Alexis Royer
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#     * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
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


# Common overridable variables
#   Project name
PROJECT = Makefile
DEBUG = no
#   Input sources
SRC_DIR = .
CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)
#   Automatic dependencies (.deps files)
AUTO_DEPS = yes
#   C++ compilation options
ifeq ($(DEBUG),no)
	CPP_DEBUG_FLAG =
else
	CPP_DEBUG_FLAG = -g -D_DEBUG
endif
PROJ_CPP_FLAGS =
PROJ_INCLUDES =
#   Link options
PROJ_DEPS =
PROJ_LIBS = $(PROJ_DEPS)
#   Output
#   PRODUCT_TYPE can be one of (LIB|BIN)
PRODUCT_TYPE = LIB
# Cleanup
# Aditional cleanup items
PROJ_CLEAN =


# Plateform depending configuration
#   Linux
ifeq ($(shell echo "$$OSTYPE" | grep -i "linux" | wc -l),1)
	OS = _LINUX
	TARGET = Linux
	STL_LIB = -ldl
	BIN_SUFFIX =
endif
#   Cygwin
ifeq ($(shell echo "$$OSTYPE" | grep -i "cygwin" | wc -l),1)
	OS = _WINDOWS
	TARGET = Win32
	STL_LIB =
	BIN_SUFFIX = .exe
endif


# Automatic variables computation
ifeq ($(DEBUG),no)
	OUT_DIR = $(TARGET)$(CXX)/Release
else
	OUT_DIR = $(TARGET)$(CXX)/Debug
endif
INT_DIR = $(OUT_DIR)/__$(PROJECT)
DEPS = $(PROJECT).deps
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(INT_DIR)/%.o,$(CPP_FILES))
CPP_FLAGS = -Wall -O2 $(CPP_DEBUG_FLAG) -D$(OS) $(PROJ_CPP_FLAGS)
INCLUDES = -I/usr/include $(PROJ_INCLUDES)
LIBS = $(PROJ_LIBS) $(STL_LIB)
PRODUCT = $(OUT_DIR)/$(PROJECT)
