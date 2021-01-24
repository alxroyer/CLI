# Copyright (c) 2006-2009, Alexis Royer, http://alexis.royer.free.fr/CLI
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

ifndef __CPP_MKRES__
__CPP_MKRES__ = 1

# Parameters
CLI_XML_RES ?= $(CLI_DIR)/samples/clisample/clisample.xml
CLI_XML_CPP ?= $(patsubst %.xml,$(INT_DIR)/%.cpp,$(notdir $(CLI_XML_RES)))
CLI_XML_OBJ = $(patsubst %.cpp,%.o,$(CLI_XML_CPP))
CLI_MAIN_CPP ?= $(CPP_DIR)/tests/testsample.cpp
CLI_MAIN_OBJ ?= $(patsubst %.cpp,$(INT_DIR)/%.o,$(notdir $(CLI_MAIN_CPP)))
CLI_BINARY ?= $(OUT_DIR)/$(PROJECT)$(BIN_SUFFIX)
CLI_XSL ?= $(CPP_DIR)/xsl/cppclic.xsl
CLI_XSLT_OPTS ?=

# Includes
PROJECT ?= $(patsubst %.xml,%,$(notdir $(CLI_XML_RES)))
PROJECT_DEPS ?= libclicpp.mak
PRODUCT ?= $(CLI_BINARY)
CPP_FILES ?= $(CLI_XML_CPP) $(CLI_MAIN_CPP)
AUTO_DEPS ?= no
PROJ_INCLUDES ?= -I$(CPP_DIR)/include -I$(dir $(CLI_XML_RES))
PROJ_LIBS ?= -L$(dir $(CPP_LIB)) -lclicpp -lncurses
PROJ_CLEAN += $(CLI_XML_CPP)
include _build.mak

# Rules
$(CLI_XML_CPP): $(CLI_XML_RES) $(CLI_XSL)
	@mkdir -p $(dir $(CLI_XML_CPP))
	xsltproc $(CLI_XSLT_OPTS) $(CLI_XSL) $(CLI_XML_RES) > $(CLI_XML_CPP)

$(CLI_XML_OBJ): CPP_FLAGS += -Wno-unused-label

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CPP_DIR)/build/make/_mkres.vars
vars: $(CPP_DIR)/build/make/_mkres.vars
$(CPP_DIR)/build/make/_mkres.vars:
	$(call ShowVariables,CLI_XML_RES CLI_XML_CPP CLI_XML_OBJ CLI_MAIN_CPP CLI_MAIN_OBJ CLI_BINARY CLI_XSL CLI_XSLT_OPTS)

# Dependencies
$(CLI_XML_OBJ): $(CLI_XML_CPP) $(wildcard $(CPP_DIR)/include/cli/*.h)
$(CLI_MAIN_OBJ): $(CLI_MAIN_CPP) $(wildcard $(CPP_DIR)/include/cli/*.h)
$(PRODUCT): $(CPP_LIB)


endif
# __CPP_MKRES__
