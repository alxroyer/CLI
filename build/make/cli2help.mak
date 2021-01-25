# Copyright (c) 2006-2011, Alexis Royer, http://alexis.royer.free.fr/CLI
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
.DEFAULT_GOAL = default
.PHONY: cli2help.default
cli2help.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../..
include _vars.mak

# Variables
XSL_DIR = $(CLI_DIR)/xsl
OUT_DIR = cli2help
XML_FILES = $(shell find $(CLI_DIR)/samples -name "*.xml" -print)
HTML_FILES += $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.en.html,$(XML_FILES))
HTML_FILES += $(patsubst $(CLI_DIR)/samples/%.xml,$(OUT_DIR)/%.fr.html,$(XML_FILES))

# Rules
.PHONY: default
default: $(HTML_FILES) ;

$(OUT_DIR)/%.en.html: $(CLI_DIR)/samples/%.xml $(XSL_DIR)/cli2help.xsl
	@mkdir -p $(dir $@)
	xsltproc --stringparam STR_Lang 'en' $(XSL_DIR)/cli2help.xsl $< > $@

$(OUT_DIR)/%.fr.html: $(CLI_DIR)/samples/%.xml $(XSL_DIR)/cli2help.xsl
	@mkdir -p $(dir $@)
	xsltproc --stringparam STR_Lang 'fr' $(XSL_DIR)/cli2help.xsl $< > $@

.PHONY: deps
deps: ;

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/cli2help.help
help: $(CLI_DIR)/build/make/cli2help.help
$(CLI_DIR)/build/make/cli2help.help:
	$(call PrintHelp, default,  Generate help files from samples)

.PHONY: $(CLI_DIR)/build/make/cli2help.vars
vars: $(CLI_DIR)/build/make/cli2help.vars
$(CLI_DIR)/build/make/cli2help.vars:
	$(call ShowVariables,XSL_DIR OUT_DIR XML_FILES HTML_FILES)
