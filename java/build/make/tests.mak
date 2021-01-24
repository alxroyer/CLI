# Copyright (c) 2006-2009, Alexis Royer
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
DEFAULT_GOAL ?= tests
.PHONY: javatests.default
javatests.default: tests ;


# Variables
CLI_DIR := ../../..
include vars.mak

JAVA_SAMPLE_FILES = $(shell find $(SAMPLES_DIR) -name "*.java")
JAVA_SAMPLE_CLASSES = $(patsubst %.java,$(OUT_DIR)/cli/test/%.class,$(notdir $(JAVA_SAMPLE_FILES)))

XML_FILES = $(patsubst %.check,%.xml,$(CHECK_FILES))
TEST_FILES = $(patsubst %.xml,%.test,$(XML_FILES))
CHECK_FILES = $(shell find $(SAMPLES_DIR) -name "*.check")


# Rules
DoTestDefault = $(MAKE) -s --no-print-directory -f test.mak CLI_XML_RES=$(1)
.PHONY: tests
tests:
	@$(MAKE) --no-print-directory -f tests.mak java
	$(call Map,DoTestDefault,$(XML_FILES))

.PHONY: java
java: $(JAVA_SAMPLE_CLASSES) ;

$(OUT_DIR)/cli/test/%.class: $(JAVA_SRC_DIR)/cli/test/%.java
	javac $(JAVAC_FLAGS) $<

$(JAVA_SRC_DIR)/cli/test/%.java: JAVA_SRC_FILE = $@
$(JAVA_SRC_DIR)/cli/test/%.java: JAVA_SAMPLE_FILE = $(SAMPLES_DIR)/user-guide/$(notdir $@)
$(JAVA_SRC_DIR)/cli/test/%.java: $(SAMPLES_DIR)/user-guide/%.java
	echo "package cli.test;" > $(JAVA_SRC_FILE)
	cat $(JAVA_SAMPLE_FILE) >> $(JAVA_SRC_FILE)

.PHONY: dirs
dirs: ;

DoTestClean = $(DoTestDefault) clean
.PHONY: clean
clean:
	$(RM) $(JAVA_SAMPLE_CLASSES)
	$(call Map,DoTestClean,$(XML_FILES))

.PHONY: deps
deps: ;

# Debug and help
include $(CLI_DIR)/build/make/help.mak

.PHONY: $(JAVA_DIR)/build/make/tests.help
$(JAVA_DIR)/build/make/tests.help:
	$(call PrintHelp, tests, Launch tests on each sample file)
	$(call PrintHelp, clean, Clean intermediate files)

.PHONY: $(JAVA_DIR)/build/make/tests.vars
$(JAVA_DIR)/build/make/tests.vars:
	$(call ShowVariables, JAVA_SAMPLE_FILES JAVA_SAMPLE_CLASSES XML_FILES TEST_FILES CHECK_FILES)
