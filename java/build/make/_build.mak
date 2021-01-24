# Copyright (c) 2006-2010, Alexis Royer, http://alexis.royer.free.fr/CLI
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


ifndef __JAVA_BUILD__
__JAVA_BUILD__ = 1


# Default rule
ifndef .DEFAULT_GOAL
.DEFAULT_GOAL = build
endif
.PHONY: build.default
build.default: $(.DEFAULT_GOAL) ;


# Includes
include _vars.mak


# Variables
PRODUCT ?= project
SRC_DIR ?= .
JAVA_FILES ?= $(wildcard $(SRC_DIR)/*.java)
JAVA_OBJS ?= $(patsubst $(SRC_DIR)/%.java,$(OUT_DIR)/%.class,$(JAVA_FILES))
ifndef JAVAC_OPTS
JAVAC_OPTS ?= -Xlint:deprecation
ifdef _DEBUG
	JAVAC_OPTS += -g
endif
endif
JAR_OBJ ?= build.java
JAR_OPTS ?=
JAR_SRC_DIR ?= $(sort $(dir $(patsubst $(SRC_DIR)/%.java,$(OUT_DIR)/%.java,$(filter-out %/package-info.java,$(JAVA_FILES)))))
JAR_SRC_FILES ?= $(patsubst %,%/*.class,$(JAR_SRC_DIR))
PROJECT_DEPS ?=
ifeq ($(JAR_OBJ),build.java)
PROJ_CLEAN ?= $(JAVA_OBJS)
else
PROJ_CLEAN ?= $(JAVA_OBJS) $(JAR_OBJ)
endif


# Rules
.PHONY: build
build: dirs build.depends $(JAR_OBJ)
build: ;

.PHONY: dirs
dirs:
	@mkdir -p $(OUT_DIR)

.PHONY: build.depends
build.depends:
	$(call MkDispatch, $(PROJECT_DEPS))

.PHONY: build.java
build.java: $(JAVA_FILES)
	javac $(JAVAC_OPTS) -d $(OUT_DIR) -classpath $(OUT_DIR) $(JAVA_FILES)

ifneq ($(JAR_OBJ),build.java)
$(JAR_OBJ): $(JAVA_FILES)
	javac $(JAVAC_OPTS) -d $(OUT_DIR) -classpath $(OUT_DIR) $(JAVA_FILES)
	cd $(OUT_DIR) && jar $(JAR_OPTS) -cf $(notdir $(JAR_OBJ)) $(patsubst $(OUT_DIR)/%,%,$(JAR_SRC_FILES))
endif


# Cleanup
.PHONY: clean
clean:
	$(call MkDispatch, $(PROJECT_DEPS), clean)
	rm -rf $(JAVA_OBJS)
	rm -rf $(PROJ_CLEAN)


# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(JAVA_DIR)/build/make/_build.help
help: $(JAVA_DIR)/build/make/_build.help
$(JAVA_DIR)/build/make/_build.help: ;
	$(call PrintHelp,   dirs,   Check intermediate directories)
	$(call PrintHelp,   build,  Build $(notdir $(PRODUCT)))
	$(call PrintHelp,   clean,  Clean $(notdir $(PRODUCT)) outputs)

.PHONY: $(JAVA_DIR)/build/make/_build.vars
vars: $(JAVA_DIR)/build/make/_build.vars
$(JAVA_DIR)/build/make/_build.vars:
	$(call ShowVariables,PRODUCT SRC_DIR JAVA_FILES JAVA_OBJS JAVAC_OPTS JAR_OBJ JAR_OPTS JAR_SRC_DIR JAR_SRC_FILES PROJECT_DEPS PROJ_CLEAN)


endif
# __JAVA_BUILD__