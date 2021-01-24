# Copyright (c) 2006-2009, Alexis Royer, http://alexis.royer.free.fr/CLI
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
include _vars.mak

# Variables
CLI_XML_RES = $(CLI_DIR)/samples/user-guide/hello.xml
CLI_XSL = $(JAVA_DIR)/xsl/javaclic.xsl
CLI_JAVA = $(patsubst %.xml,$(JAVA_DIR)/src/cli/test/%.java,$(notdir $(CLI_XML_RES)))
CLI_JAVA_CLASS_NAME = $(subst -,_,$(patsubst %.java,%,$(notdir $(CLI_JAVA))))
CLI_CLASS = $(OUT_DIR)/cli/test/$(CLI_JAVA_CLASS_NAME).class
CLI_GO_JAVA = $(JAVA_DIR)/src/cli/test/GoHello.java
CLI_GO_CLASS = $(OUT_DIR)/cli/test/GoHello.class

# Rules
.PHONY: depends
depends:
	$(call MkDispatch,libclijava.mak native.mak)

.PHONY: build
build: depends $(CLI_CLASS) $(CLI_GO_CLASS) ;

run:
	cd $(OUT_DIR) && java cli.test.GoHello

$(CLI_GO_CLASS): $(CLI_GO_JAVA) $(wildcard $(JAVA_DIR)/src/cli/*.java)
	@mkdir -p $(OUT_DIR)
	javac $(JAVA_PATH) $(JAVAC_FLAGS) $<

$(CLI_CLASS): $(CLI_JAVA) $(wildcard $(JAVA_DIR)/src/cli/*.java)
	@mkdir -p $(OUT_DIR)
	javac $(JAVA_PATH) $(JAVAC_FLAGS) $<

$(CLI_JAVA): $(CLI_XML_RES) $(CLI_XSL)
	@mkdir -p $(OUT_DIR)
	echo "package cli.test;" > $(CLI_JAVA)
	xsltproc --stringparam STR_CliClassName $(CLI_JAVA_CLASS_NAME) $(CLI_XSL) $(CLI_XML_RES) >> $(CLI_JAVA)

.PHONY: deps
deps: ;

.PHONY: clean
clean:
	$(RM) $(CLI_JAVA) $(CLI_CLASS) $(CLI_TEST_SAMPLE_CLASS) $(CLI_LOG)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(JAVA_DIR)/build/make/hello.help
help: $(JAVA_DIR)/build/make/hello.help
$(JAVA_DIR)/build/make/hello.help:
	$(call PrintHelp, build, Build the Hello sample project)
	$(call PrintHelp, run, 	 Run the Hello sample)
	$(call PrintHelp, clean, Clean intermediate and output files)

.PHONY: $(JAVA_DIR)/build/make/hello.vars
vars: $(JAVA_DIR)/build/make/hello.vars
$(JAVA_DIR)/build/make/hello.vars:
	$(call ShowVariables,CLI_XML_RES CLI_XSL CLI_JAVA CLI_JAVA_CLASS_NAME CLI_CLASS CLI_GO_JAVA CLI_GO_CLASS)

