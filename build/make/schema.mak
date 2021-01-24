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
.DEFAULT_GOAL = check
.PHONY: schema.default
schema.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../..
include _vars.mak

# Variables
SCHEMA_DIR = $(CLI_DIR)/schema
XML_FILES = $(shell find $(CLI_DIR)/samples -name "*.xml" -print)

# Rules
.PHONY: check
check: $(XML_FILES) ;

.PHONY: %.xml
%.xml: force
	xmllint --schema $(SCHEMA_DIR)/cli.xsd --noout $@
	xmllint --relaxng $(SCHEMA_DIR)/cli.rng --noout $@

.PHONY: force
force: ;

.PHONY: deps
deps: ;

.PHONY: clean
clean: ;

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/schema.help
help: $(CLI_DIR)/build/make/schema.help
$(CLI_DIR)/build/make/schema.help:
	$(call PrintHelp, check,    Check all sample files)

.PHONY: $(CLI_DIR)/build/make/schema.vars
vars: $(CLI_DIR)/build/make/schema.vars
$(CLI_DIR)/build/make/schema.vars:
	$(call ShowVariables,SCHEMA_DIR XML_FILES)
