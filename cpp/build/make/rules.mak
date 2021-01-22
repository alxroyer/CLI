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


.PHONY: product
product: dirs $(PRODUCT)
	@#

$(PRODUCT): $(PROJ_DEPS) $(OBJS)
ifeq ($(shell echo "$(PRODUCT_TYPE)" | grep "BIN" | wc -l),1)
	$(CXX) -o $(PRODUCT) $(OBJS) $(LIBS)
endif
ifeq ($(shell echo "$(PRODUCT_TYPE)" | grep "LIB" | wc -l),1)
	$(AR) -cru $(PRODUCT) $(OBJS)
	ranlib $(PRODUCT)
endif

.PHONY: dirs
dirs:
	@if [ ! -d $(OUT_DIR) ] ; then \
		echo "mkdir -p $(OUT_DIR)" ; \
		mkdir -p $(OUT_DIR) ; \
	fi
	@if [ ! -d $(INT_DIR) ] ; then \
		echo "mkdir -p $(INT_DIR)" ; \
		mkdir -p $(INT_DIR) ; \
	fi

.PHONY: help
help:
	@echo "make help        Get help"
	@echo "make debug       Debug Makefile variables"
	@echo "make deps        Update $(PROJECT).deps"
	@echo "make clean       Clean up output objects and library"
	@echo "make             Generate $(PRODUCT)"

.PHONY: vars
vars:
	@echo "Platform identification"
	@echo " OS = $(OS)"
	@echo " TARGET = $(TARGET)"
	@echo "Tools"
	@echo " CXX = $(CXX)"
	@echo " AR = $(AR)"
	@echo "Project"
	@echo " PROJECT = $(PROJECT)"
	@echo "Target"
	@echo " DEBUG = $(DEBUG)"
	@echo "Input sources"
	@echo " SRC_DIR = $(SRC_DIR)"
	@echo " CPP_FILES = $(CPP_FILES)"
	@echo "C++ compilation options"
	@echo " CPP_DEBUG_FLAG = $(CPP_DEBUG_FLAG)"
	@echo " PROJ_CPP_FLAGS = $(PROJ_CPP_FLAGS)"
	@echo " CPP_FLAGS = $(CPP_FLAGS)"
	@echo " PROJ_INCLUDES = $(PROJ_INCLUDES)"
	@echo " INCLUDES = $(INCLUDES)"
	@echo "Link options"
	@echo " PROJ_DEPS = $(PROJ_DEPS)"
	@echo " PROJ_LIBS = $(PROJ_LIBS)"
	@echo " LIBS = $(LIBS)"
	@echo "Output"
	@echo " INT_DIR = $(INT_DIR)"
	@echo " OBJS = $(OBJS)"
	@echo " OUT_DIR = $(OUT_DIR)"
	@echo " PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@echo " PRODUCT = $(PRODUCT)"

.PHONY: deps cleandeps %.dep
deps: cleandeps $(patsubst %.cpp,%.dep,$(CPP_FILES))
	@# Dependecies are generated through the two following rules.

cleandeps:
	@if [ \"$(AUTO_DEPS)\" = \"yes\" ] ; then \
		echo "rm -f $(DEPS) && touch $(DEPS)" ; \
		rm -f $(DEPS) && touch $(DEPS) ; \
	fi

%.dep:
	@if [ \"$(AUTO_DEPS)\" = \"yes\" ] ; then \
		echo "$(CXX) $(CPP_FLAGS) $(INCLUDES) -MT $(patsubst $(SRC_DIR)/%.dep,\$$(INT_DIR)/%.o,$@) -MM $(patsubst %.dep,%.cpp,$@) >> $(DEPS)" ; \
		$(CXX) $(CPP_FLAGS) $(INCLUDES) -MT $(patsubst $(SRC_DIR)/%.dep,$$\(INT_DIR\)/%.o,$@) -MM $(patsubst %.dep,%.cpp,$@) >> $(DEPS) ; \
	fi

%.o:
	$(CXX) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(PRODUCT) $(PROJ_DEPS) $(PROJ_CLEAN)

