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
CLI_XML_RES = clisample.xml

# Variables computation
CLI_XML_RES_BASE = clisample.xml
CLI_XML_RES_DIR = $(shell dirname $(CLI_XML_RES))
CLI_XSL = $(CPP_DIR)/xsl/cppclic.xsl
CLI_CPP = $(patsubst %.xml, %.cpp, $(CLI_XML_RES_BASE))
CLI_OBJ = $(patsubst %.xml, %.o, $(CLI_XML_RES_BASE))
CLI_TEST_SAMPLE_CPP = telnet.cpp
CLI_TEST_SAMPLE_OBJ = telnet.o
CLI_EXE = $(patsubst %.xml, %.exe, $(CLI_XML_RES_BASE))

include ../../make/Makefile.vars

# Default variables overriding
CPP_DIR = ../..
SRC_DIR = .
CPP_FILES = $(CLI_CPP) $(CLI_TEST_SAMPLE_CPP)
PROJ_INCLUDES = -I$(CPP_DIR)/include
PROJ_DEPS = $(OUTPUT)/libclicpp.a
PROJ_LIBS = $(OUTPUT)/libclicpp.a /usr/lib/libncurses.a
PRODUCT_TYPE = BIN
PRODUCT = $(CLI_EXE)
PROJ_CLEAN = $(CLI_CPP) $(CLI_LOG)


# Rules
include ../../make/Makefile.rules

$(OUTPUT)/libclicpp.a:
	cd $(CPP_DIR)/src && make

$(CLI_CPP): $(CLI_XML_RES) $(CLI_XSL)
	xsltproc $(CLI_XSL) $(CLI_XML_RES) > $(CLI_CPP)

.PHONY: check
check: $(CLI_LOG) $(CLI_CHECK)
	diff --strip-trailing-cr $(CLI_LOG) $(CLI_CHECK)

# Dependencies
$(CLI_OBJ): $(CLI_CPP) $(wildcard $(CPP_DIR)/include/*.h)
$(CLI_TEST_SAMPLE_OBJ): $(CLI_TEST_SAMPLE_CPP) $(wildcard $(CPP_DIR)/include/*.h)
