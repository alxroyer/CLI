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

ifndef __ROOT_HELP__
__ROOT_HELP__ = 1


ROOT_DIR ?= ../..

# Functions
	# Print help.
	# $(1): Rule name.
	# $(2): Rule Description.
	PrintHelp =     @echo -n "$(MAKE)" \
	                && echo -n "$(if $(strip $(1)), $(strip $(1)))" \
	                && echo -n "$(if $(filter $(.DEFAULT_GOAL),$(1)), [default])" \
	                && echo -n ": $(strip $(2))" \
	                && echo ""
	# Show variables for the current makefile.
	ShowVariables = @echo "$(patsubst %Makefile.mak,%Makefile,$(patsubst %.vars,%.mak,$@)):" \
	                $(foreach a,$(1), && echo "    $(a) = $($(a))") \
	                && echo ""

# Rules
HELP_RULES = $(patsubst Makefile,Makefile.help,$(patsubst %.mak,%.help,$(MAKEFILE_LIST)))
.PHONY: help %.help
help: $(HELP_RULES) ;
# This rule should simply redirect to the correct file when no path is given
#%.help: $(patsubst $(realpath $(ROOT_DIR))/%,$(ROOT_DIR)/%,$(realpath .))/%.help ;
%.help: $(ROOT_DIR)/build/make/%.help ;
%.help: $(CPP_DIR)/build/make/%.help ;
%.help: $(JAVA_DIR)/build/make/%.help ;

VAR_RULES = $(filter-out $(ROOT_DIR)/build/make/utils.vars $(ROOT_DIR)/build/make/help.vars,$(patsubst Makefile,Makefile.vars,$(patsubst %.mak,%.vars,$(MAKEFILE_LIST))))
.PHONY: vars
vars: $(VAR_RULES) ;
# This rule should simply redirect to the correct file when no path is given
#%.vars: $(patsubst $(realpath $(ROOT_DIR))/%,$(ROOT_DIR)/%,$(realpath .))/%.vars ;
%.vars: $(ROOT_DIR)/build/make/%.vars ;
%.vars: $(CPP_DIR)/build/make/%.vars ;
%.vars: $(JAVA_DIR)/build/make/%.vars ;

# Debug and help
.PHONY: $(ROOT_DIR)/build/make/help.help
$(ROOT_DIR)/build/make/help.help:
	$(call PrintHelp, help, Print help)
	$(call PrintHelp, vars, Show variables)

.PHONY: $(ROOT_DIR)/build/make/help.vars
$(ROOT_DIR)/build/make/help.vars:
	$(call ShowVariables,MAKEFILE_LIST VAR_RULES HELP_RULES)


endif
# __ROOT_HELP__
