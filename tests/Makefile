# This file is part of the CLI library.  The CLI library aims to provide
# facilities in making text-based user interfaces.
# Copyright (C) 2006 Alexis Royer.
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


# Parameters
CLI_XML_RES = sample/clisample.xml
CPP_DIR = ..

# Variables computation
CLI_XML_RES_BASE = $(shell basename $(CLI_XML_RES))
CLI_XML_RES_DIR = $(shell dirname $(CLI_XML_RES))
CLI_XSL = $(CPP_DIR)/xsl/cppclic.xsl
CLI_CPP = $(patsubst %.xml, %.cpp, $(OUTPUT)/$(CLI_XML_RES_BASE))
CLI_OBJ = $(patsubst %.xml, %.o, $(OUTPUT)/$(CLI_XML_RES_BASE))
CLI_TEST_SAMPLE_CPP = $(CPP_DIR)/tests/testsample.cpp
CLI_TEST_SAMPLE_OBJ = $(OUTPUT)/testsample.o
CLI_EXE = $(patsubst %.xml, %.exe, $(OUTPUT)/$(CLI_XML_RES_BASE))
CLI_TEST = $(CLI_XML_RES_DIR)/$(CLI_XML_RES_BASE).test
CLI_LOG = $(patsubst %.xml, %.log, $(OUTPUT)/$(CLI_XML_RES_BASE))
CLI_CHECK = $(CLI_XML_RES_DIR)/$(CLI_XML_RES_BASE).check

# Behavior
ifeq ($(shell ls $(CLI_CHECK) 2> /dev/null | wc -l), 1)
	MOST_ADV_RULE = check
else
	ifeq ($(shell ls $(CLI_TEST) 2> /dev/null | wc -l), 1)
		MOST_ADV_RULE = $(CLI_LOG)
	else
		MOST_ADV_RULE = run
	endif
endif


include ../make/Makefile.vars

# Default variables overriding
CPP_DIR = ..
CPP_FILES = $(CLI_CPP) $(CLI_TEST_SAMPLE_CPP)
OBJS = $(CLI_OBJ) $(CLI_TEST_SAMPLE_OBJ)
PROJ_INCLUDES = -I$(CPP_DIR)/include -I$(CLI_XML_RES_DIR)
PROJ_DEPS = $(OUTPUT)/libclicpp.a
PROJ_LIBS = $(OUTPUT)/libclicpp.a /usr/lib/libncurses.a
PRODUCT_TYPE = BIN
PRODUCT = $(CLI_EXE)
PROJ_CLEAN = $(CLI_CPP) $(CLI_LOG)


# Rules
.PHONY: all
all: $(MOST_ADV_RULE)
	@#

include ../make/Makefile.rules

$(OUTPUT)/libclicpp.a:
	cd $(CPP_DIR)/src && make

$(CLI_CPP): $(CLI_XML_RES) $(CLI_XSL)
	xsltproc $(CLI_XSL) $(CLI_XML_RES) > $(CLI_CPP)

.PHONY: run
run: $(CLI_EXE)
	$(CLI_EXE)

$(CLI_LOG): $(CLI_EXE) $(CLI_TEST)
	$(CLI_EXE) < $(CLI_TEST) > $(CLI_LOG)
	$(CPP_DIR)/tests/cleanlog.sh $(CLI_LOG)

.PHONY: check
check: $(CLI_LOG) $(CLI_CHECK)
	diff --strip-trailing-cr $(CLI_LOG) $(CLI_CHECK)

# Dependencies
$(CLI_OBJ): $(CLI_CPP) $(wildcard $(CPP_DIR)/include/*.h)
$(CLI_TEST_SAMPLE_OBJ): $(CLI_TEST_SAMPLE_CPP)

