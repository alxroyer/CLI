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
