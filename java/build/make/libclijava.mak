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


# Default goal
DEFAULT_GOAL ?= build
.PHONY: libclijava.default
libclijava.default: build ;

# Variables.
CLI_DIR := ../../..
include vars.mak

JAVA_FILES = $(wildcard $(JAVA_DIR)/src/cli/*.java)
JAVA_CLASSES = $(patsubst %.java,$(OUT_DIR)/cli/%.class,$(notdir $(JAVA_FILES)))

# Rules.
.PHONY: build
build: dirs $(JAVA_ARCHIVE) ;

.PHONY: dirs
dirs:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(OUT_DIR)/cli

$(JAVA_ARCHIVE): $(JAVA_FILES)
	javac $(JAVAC_FLAGS) $(JAVA_FILES)
	cd $(RDX) && jar $(JAR_FLAGS) -cf $(notdir $(JAVA_ARCHIVE)) cli

%.class: %.java
	javac $<

.PHONY: deps
deps: ;

.PHONY: clean
clean:
	$(RM) $(JAVA_ARCHIVE) $(JAVA_CLASSES)

# Debug and help
include $(CLI_DIR)/build/make/help.mak

.PHONY: $(JAVA_DIR)/build/make/libclijava.help
$(JAVA_DIR)/build/make/libclijava.help:
	$(call PrintHelp, build, Generate Java library)
	$(call PrintHelp, dirs, Check output and intermediate directories)
	$(call PrintHelp, clean, Clean intermediate and output files)

.PHONY: $(JAVA_DIR)/build/make/libclijava.vars
$(JAVA_DIR)/build/make/libclijava.vars:
	$(call ShowVariables,JAVA_FILES JAVA_CLASSES)

