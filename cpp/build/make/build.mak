# Copyright (c) 2006-2008, Alexis Royer
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

ifndef __CPP_BUILD__
__CPP_BUILD__ = 1


##############
# Default goal
DEFAULT_GOAL ?= build
.PHONY: cppbuild.default
cppbuild.default: build ;


######################
# Preliminary includes
CLI_DIR ?= ../../..
include $(CLI_DIR)/build/make/vars.mak
include $(CPP_DIR)/build/make/vars.mak


##############
# Parameters
	# Project
		# Project name. It is generally similar to the makefile name.
		PROJECT ?= $(patsubst %.mak,%,$(firstword $(MAKEFILE_LIST)))
		# Project dependencies. Projects this one relies on. Build and clean rules will be proprgated to them.
		PROJECT_DEPS ?=
		# PRODUCT_TYPE can be one of (BIN|STATIC_LIB|DYN_LIB).
		PRODUCT_TYPE ?= BIN
		# Product file.
		PRODUCT ?= $(OUT_DIR)/$($(PRODUCT_TYPE)_PREFIX)$(PROJECT)$($(PRODUCT_TYPE)_SUFFIX)
	# Input sources
		# Input source directory.
		SRC_DIR ?= .
		# Input source list.
		CPP_FILES ?= $(wildcard $(SRC_DIR)/*.cpp)
		# Corresponding objects.
		OBJS ?= $(patsubst %.cpp,$(INT_DIR)/%.o,$(notdir $(CPP_FILES)))
	# Dependencies (.deps files)
		# Choose whether automatic dependencies should be performed.
		AUTO_DEPS ?= yes
		# Name of the automatic dependencies file.
		AUTO_DEPS_FILE ?= $(PROJECT).deps
	# C++ compilation options
		# Project C++ flags.
		PROJ_CPP_FLAGS ?=
		# Debug and optimization flags.
ifdef _DEBUG
		CPP_OPT_FLAGS ?=
		CPP_DEBUG_FLAG ?= -g -D_DEBUG
else
		CPP_OPT_FLAGS ?= -O2
		CPP_DEBUG_FLAG ?=
endif
ifneq ($(LINUX),)
		CPP_OS_FLAGS = -D_LINUX
endif
ifneq ($(CYGWIN),)
		CPP_OS_FLAGS = -D_CYGWIN
endif
		# All C++ flags
		CPP_FLAGS ?= -Wall $(CPP_OPT_FLAGS) $(CPP_DEBUG_FLAG) $(CPP_OS_FLAGS) $(PROJ_CPP_FLAGS)
		# Project includes
		PROJ_INCLUDES ?=
		# All includes
		INCLUDES ?= -I/usr/include $(PROJ_INCLUDES)
	# Link options
		RANLIB ?= ranlib
		PROJ_LIBS ?=
		LIBS ?= -L/usr/lib $(PROJ_LIBS)
ifneq ($(LINUX),)
		LIBS += -ldl
endif
	# Output
		OUT_DIR ?= $(TARGET)$(CXX)/$(RDX)
		INT_DIR ?= $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
	# Cleanup
		# Additional cleanup items
		PROJ_CLEAN ?=
		# Final cleanup item list
		CLEAN ?= $(OBJS) $(PRODUCT) $(PROJ_CLEAN)


##############
# Includes
CLI_DIR ?= ../../..
include $(CLI_DIR)/build/make/vars.mak


##############
# Rules

# Build
.PHONY: build
build: build_depends dirs $(PRODUCT) ;

.PHONY: build_depends
build_depends:
	$(call MkDispatch,$(PROJECT_DEPS),)

$(PRODUCT): $(OBJS)
ifeq ($(PRODUCT_TYPE),BIN)
	$(CXX) -o $(PRODUCT) $(OBJS) $(LIBS)
endif
ifeq ($(PRODUCT_TYPE),STATIC_LIB)
	$(AR) -cru $(PRODUCT) $(OBJS)
	$(RANLIB) $(PRODUCT)
endif
ifeq ($(PRODUCT_TYPE),DYN_LIB)
ifeq ($(TARGET),Cygwin)
	$(CXX) -shared -Wl,--add-stdcall-alias -o $(PRODUCT) $(OBJS) $(LIBS)
endif
ifeq ($(TARGET),Linux)
	$(CXX) -shared -Wl -o $(PRODUCT) $(OBJS) $(LIBS)
endif
endif

%.o:
	$(CXX) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

# Automatic dependencies
.PHONY: deps
ifeq ($(AUTO_DEPS),yes)
MkDepFile = $(CXX) $(CPP_FLAGS) $(INCLUDES) -MT $(patsubst %.cpp,$$\(INT_DIR\)/%.o,$(notdir $(1))) -MM $(1) >> $(AUTO_DEPS_FILE)
deps:
	$(RM) $(AUTO_DEPS_FILE) && touch $(AUTO_DEPS_FILE)
	$(call Map,MkDepFile,$(CPP_FILES))
else
# Do nothing
deps: ;
endif

# Clean up and directories
.PHONY: dirs
dirs:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(INT_DIR)

.PHONY: clean
clean:
	$(call MkDispatch,$(PROJECT_DEPS),clean)
	$(RM) $(CLEAN)

# Debug and help
.PHONY: $(CPP_DIR)/build/make/build.help
$(CPP_DIR)/build/make/build.help:
	$(call PrintHelp,   build,  Build $(notdir $(PRODUCT)))
	$(call PrintHelp,   dirs,   Check intermediate and output directories)
ifeq ($(AUTO_DEPS),yes)
	$(call PrintHelp,   deps,   Update dependencies file ($(AUTO_DEPS_FILE)))
endif
	$(call PrintHelp,   clean,  Clean intermediate and output files)

.PHONY: $(CPP_DIR)/build/make/build.vars
$(CPP_DIR)/build/make/build.vars: MY_VARS += MAKE CXX AR RANLIB
$(CPP_DIR)/build/make/build.vars: MY_VARS += PROJECT PROJECT_DEPS PRODUCT_TYPE PRODUCT
$(CPP_DIR)/build/make/build.vars: MY_VARS += SRC_DIR CPP_FILES OBJS
$(CPP_DIR)/build/make/build.vars: MY_VARS += AUTO_DEPS AUTO_DEPS_FILE
$(CPP_DIR)/build/make/build.vars: MY_VARS += PROJ_CPP_FLAGS CPP_OPT_FLAG CPP_DEBUG_FLAG CPP_OS_FLAGS CPP_FLAGS PROJ_INCLUDES INCLUDES
$(CPP_DIR)/build/make/build.vars: MY_VARS += PROJ_LIBS LIBS
$(CPP_DIR)/build/make/build.vars: MY_VARS += INT_DIR OUT_DIR
$(CPP_DIR)/build/make/build.vars: MY_VARS += PROJ_CLEAN CLEAN
$(CPP_DIR)/build/make/build.vars:
	$(call ShowVariables,$(MY_VARS))


endif
# __CPP_BUILD__
