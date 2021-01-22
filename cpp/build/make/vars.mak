# This file is part of the CLI library.  The CLI library aims to provide
# facilities in making text-based user interfaces.
# Copyright (C) 2006-2007 Alexis Royer.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


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
