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


# Default goal
.DEFAULT_GOAL = build
.PHONY: hello.default
hello.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
include _vars.mak
PRODUCT = hello
SRC_DIR = $(CLI_DIR)/java/src/cli/test
JAVA_FILES = $(CLI_GO_JAVA) $(CLI_JAVA)
PROJECT_DEPS = native.mak
include _build.mak
PROJ_CLEAN += $(CLI_JAVA)

# Variables
CLI_XML_RES = $(CLI_DIR)/samples/user-guide/hello.xml
CLI_XSL = $(CLI_DIR)/java/xsl/javaclic.xsl
CLI_JAVA = $(patsubst %.xml,$(CLI_DIR)/java/src/cli/test/%.java,$(notdir $(CLI_XML_RES)))
CLI_JAVA_CLASS_NAME = $(subst -,_,$(patsubst %.java,%,$(notdir $(CLI_JAVA))))
CLI_GO_JAVA = $(CLI_DIR)/java/src/cli/test/GoHello.java

# Rules
run:
	cd $(OUT_DIR) && java cli.test.GoHello

$(CLI_JAVA): $(CLI_XML_RES) $(CLI_XSL)
	mkdir -p $(dir $(OUT_DIR))
	echo "package cli.test;" > $(CLI_JAVA)
	xsltproc --stringparam STR_CliClassName $(CLI_JAVA_CLASS_NAME) $(CLI_XSL) $(CLI_XML_RES) >> $(CLI_JAVA)

.PHONY: deps
deps: ;

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/java/build/make/hello.help
help: $(CLI_DIR)/java/build/make/hello.help
$(CLI_DIR)/java/build/make/hello.help:
	$(call PrintHelp, run, 	 Run the Hello sample)

.PHONY: $(CLI_DIR)/java/build/make/hello.vars
vars: $(CLI_DIR)/java/build/make/hello.vars
$(CLI_DIR)/java/build/make/hello.vars:
	$(call ShowVariables,CLI_XML_RES CLI_XSL CLI_JAVA CLI_JAVA_CLASS_NAME CLI_GO_JAVA)

# Dependencies
build: $(JAVA_FILES)
