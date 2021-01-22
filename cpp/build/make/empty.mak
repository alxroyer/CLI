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


# Variables computation
CLI_XML_RES = $(SAMPLES_DIR)/empty.xml
CLI_XSL = $(CPP_DIR)/xsl/cppclic.xsl
CLI_CPP = $(patsubst $(SAMPLES_DIR)/%.xml,$(INT_DIR)/%.cpp,$(CLI_XML_RES))
CLI_OBJ = $(patsubst $(SAMPLES_DIR)/%.xml,$(INT_DIR)/%.o,$(CLI_XML_RES))
CLI_GO_CPP = $(SRC_DIR)/goempty.cpp
CLI_GO_OBJ = $(INT_DIR)/goempty.o

include vars.mak

# Default variables overriding
PROJECT = empty
AUTO_DEPS = no
CPP_DIR = ../..
SAMPLES_DIR = $(CPP_DIR)/../web/user-guide/samples
SRC_DIR = $(CPP_DIR)/tests/empty
CPP_FILES = $(CLI_CPP) $(CLI_GO_CPP)
OBJS = $(CLI_OBJ) $(CLI_GO_OBJ)
PROJ_INCLUDES = -I$(CPP_DIR)/include
PROJ_DEPS = $(OUT_DIR)/libclicpp.a
PROJ_LIBS = $(OUT_DIR)/libclicpp.a /usr/lib/libncurses.a
PROJ_CLEAN = $(CLI_CPP)
PRODUCT_TYPE = BIN
PRODUCT = $(OUT_DIR)/$(PROJECT)$(BIN_SUFFIX)


# Rules
include rules.mak

$(OUT_DIR)/libclicpp.a:
	make -f libclicpp.mak

$(CLI_CPP): $(CLI_XML_RES) $(CLI_XSL)
	xsltproc $(CLI_XSL) $(CLI_XML_RES) > $(CLI_CPP)

$(CLI_OBJ):
	$(CXX) $(CPP_FLAGS) $(INCLUDES) -Wno-unused-label -c $< -o $@

# Dependencies
$(CLI_OBJ): $(CLI_CPP) $(wildcard $(CPP_DIR)/include/cli/*.h)
$(CLI_GO_OBJ): $(CLI_GO_CPP) $(wildcard $(CPP_DIR)/include/cli/*.h)
