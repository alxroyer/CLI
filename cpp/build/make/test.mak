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


# Parameters
CLI_XML_RES = $(CPP_DIR)/tests/sample/clisample.xml

# Variables computation
CLI_XML_RES_DIR = $(shell dirname $(CLI_XML_RES))
CLI_XSL = $(CPP_DIR)/xsl/cppclic.xsl
CLI_CPP = $(patsubst $(CLI_XML_RES_DIR)/%.xml,$(INT_DIR)/%.cpp,$(CLI_XML_RES))
CLI_OBJ = $(patsubst %.cpp,%.o,$(CLI_CPP))
CLI_TEST_SAMPLE_CPP = $(CPP_DIR)/tests/testsample.cpp
CLI_TEST_SAMPLE_OBJ = $(INT_DIR)/testsample.o
CLI_EXE = $(patsubst $(CLI_XML_RES_DIR)/%.xml,$(INT_DIR)/%$(BIN_SUFFIX),$(CLI_XML_RES))
CLI_TEST = $(patsubst %.xml,%.test,$(CLI_XML_RES))
CLI_LOG = $(patsubst $(CLI_XML_RES_DIR)/%.xml,$(INT_DIR)/%.log,$(CLI_XML_RES))
CLI_CHECK = $(patsubst %.xml,%.check,$(CLI_XML_RES))


include vars.mak

# Default variables overriding
PROJECT = test
CPP_DIR = ../..
CPP_FILES = $(CLI_CPP) $(CLI_TEST_SAMPLE_CPP)
OBJS = $(CLI_OBJ) $(CLI_TEST_SAMPLE_OBJ)
PROJ_INCLUDES = -I$(CPP_DIR)/include -I$(CLI_XML_RES_DIR)
PROJ_DEPS = $(OUT_DIR)/libclicpp.a
PROJ_LIBS = $(OUT_DIR)/libclicpp.a /usr/lib/libncurses.a
PRODUCT_TYPE = BIN
PRODUCT = $(CLI_EXE)
PROJ_CLEAN = $(CLI_CPP) $(CLI_LOG)


# Rules
.PHONY: default
default: check
	@#

include rules.mak

$(OUT_DIR)/libclicpp.a:
	make -f libclicpp.mak

$(CLI_CPP): $(CLI_XML_RES) $(CLI_XSL)
	mkdir -p $(INT_DIR)
	mkdir -p $(OUT_DIR)
	xsltproc $(CLI_XSL) $(CLI_XML_RES) > $(CLI_CPP)

$(CLI_OBJ):
	$(CXX) $(CPP_FLAGS) -Wno-unused-label $(INCLUDES) -c $< -o $@

$(CLI_LOG): $(CLI_EXE) $(CLI_TEST)
	$(CLI_EXE) $(CLI_TEST) $(CLI_LOG)
	$(CPP_DIR)/tests/cleanlog.sh $(CLI_LOG)

.PHONY: check
check: $(CLI_LOG) $(CLI_CHECK)
	diff $(CLI_LOG) $(CLI_CHECK)

.PHONY: more-vars
more-vars:
	@echo "CLI_XML_RES = $(CLI_XML_RES)"
	@echo "CLI_XML_RES_DIR = $(CLI_XML_RES_DIR)"
	@echo "CLI_XSL = $(CLI_XSL)"
	@echo "CLI_CPP = $(CLI_CPP)"
	@echo "CLI_OBJ = $(CLI_OBJ)"
	@echo "CLI_TEST_SAMPLE_CPP = $(CLI_TEST_SAMPLE_CPP)"
	@echo "CLI_TEST_SAMPLE_OBJ = $(CLI_TEST_SAMPLE_OBJ)"
	@echo "CLI_EXE = $(CLI_EXE)"
	@echo "CLI_TEST = $(CLI_TEST)"
	@echo "CLI_LOG = $(CLI_LOG)"
	@echo "CLI_CHECK = $(CLI_CHECK)"

# Dependencies
$(CLI_OBJ): $(CLI_CPP) $(wildcard $(CPP_DIR)/include/cli/*.h)
$(CLI_TEST_SAMPLE_OBJ): $(CLI_TEST_SAMPLE_CPP) $(wildcard $(CPP_DIR)/include/cli/*.h)

