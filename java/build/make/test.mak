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
DEFAULT_GOAL ?= check
.PHONY: javatest.default
javatest.default: check ;

# Parameters
CLI_XML_RES ?= $(CLI_DIR)/samples/clisample/clisample.xml

# Includes
include vars.mak

# Variables
CLI_XSL = $(JAVA_DIR)/xsl/javaclic.xsl
CLI_JAVA = $(patsubst %.xml,$(JAVA_DIR)/src/cli/test/%.java,$(notdir $(CLI_XML_RES)))
CLI_JAVA_CLASS_NAME = $(subst -,_,$(patsubst %.java,%,$(notdir $(CLI_JAVA))))
CLI_CLASS = $(OUT_DIR)/cli/test/$(CLI_JAVA_CLASS_NAME).class
CLI_TEST_SAMPLE_JAVA = $(JAVA_DIR)/src/cli/test/TestSample.java
CLI_TEST_SAMPLE_CLASS = $(OUT_DIR)/cli/test/TestSample.class
CLI_TEST = $(patsubst %.xml,%.test,$(CLI_XML_RES))
CLI_LOG = $(patsubst %.class,%.log,$(CLI_CLASS))
CLI_CHECK = $(patsubst %.xml,%.check,$(CLI_XML_RES))

# Rules
.PHONY: check
check: depends $(CLI_LOG) $(CLI_CHECK)
	diff $(CLI_LOG) $(CLI_CHECK)

.PHONY: log
log: depends $(CLI_LOG) ;
$(CLI_LOG): $(JAVA_ARCHIVE) $(JAVA_DYN_LIB) $(CLI_TEST_SAMPLE_CLASS) $(CLI_CLASS) $(CLI_TEST) $(CPP_DIR)/tests/cleanlog.sh
	java $(JAVA_PATH) -Djava.library.path=$(OUT_DIR) cli.test.TestSample cli.test.$(CLI_JAVA_CLASS_NAME) $(CLI_TEST) $(CLI_LOG)
	$(CPP_DIR)/tests/cleanlog.sh $(CLI_LOG)

.PHONY: depends
depends:
	$(call MkDispatch,libclijava.mak native.mak)

$(CLI_TEST_SAMPLE_CLASS): $(CLI_TEST_SAMPLE_JAVA) $(wildcard $(JAVA_DIR)/src/cli/*.java)
	@mkdir -p $(OUT_DIR)
	javac $(JAVA_PATH) $(JAVAC_FLAGS) $<

$(CLI_CLASS): $(CLI_JAVA) $(wildcard $(JAVA_DIR)/src/cli/*.java)
	@mkdir -p $(OUT_DIR)
	javac $(JAVA_PATH) $(JAVAC_FLAGS) $<

$(CLI_JAVA): $(CLI_XML_RES) $(CLI_XSL)
	@mkdir -p $(OUT_DIR)
	echo "package cli.test;" > $(CLI_JAVA)
	xsltproc --stringparam STR_CliClassName $(CLI_JAVA_CLASS_NAME) $(CLI_XSL) $(CLI_XML_RES) >> $(CLI_JAVA)

.PHONY: clean
clean:
	$(RM) $(CLI_JAVA) $(CLI_CLASS) $(CLI_TEST_SAMPLE_CLASS) $(CLI_LOG)

# Debug and help
include $(CLI_DIR)/build/make/help.mak

.PHONY: $(JAVA_DIR)/build/make/test.help
$(JAVA_DIR)/build/make/test.help:
	$(call PrintHelp, check, Check test output)
	$(call PrintHelp, log, Generate $(notdir $(CLI_LOG)))
	$(call PrintHelp, clean, Clean intermediate and output files)

.PHONY: $(JAVA_DIR)/build/make/test.vars
$(JAVA_DIR)/build/make/test.vars:
	$(call ShowVariables,CLI_XML_RES CLI_XSL CLI_JAVA CLI_JAVA_CLASS_NAME CLI_CLASS CLI_TEST_SAMPLE_JAVA CLI_TEST_SAMPLE_CLASS CLI_TEST CLI_LOG CLI_CHECK)

