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

