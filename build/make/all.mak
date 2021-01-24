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

ifndef __ROOT_ALL__
__ROOT_ALL = 1


# Default goal
DEFAULT_GOAL ?= default
.PHONY: all.default
all.default: default ;

# Parameters
PROJECTS ?=

# Includes
CLI_DIR ?= ../..
include $(CLI_DIR)/build/make/vars.mak

# Rules
.PHONY: default
default:
	$(call MkDispatch,$(PROJECTS),)

.PHONY: deps
deps:
	$(call MkDispatch,$(PROJECTS),deps)

.PHONY: clean
clean:
	$(call MkDispatch,$(PROJECTS),clean)

# Debug and help
.PHONY: $(CLI_DIR)/build/make/all.help
$(CLI_DIR)/build/make/all.help:
	$(call PrintHelp, default,   Dispatch default rule on sub-projects)
	$(call PrintHelp, deps,      Dispatch deps rule on sub-projects)
	$(call PrintHelp, clean,     Dispatch clean rule on sub-projects)

.PHONY: $(CLI_DIR)/build/make/all.vars
$(CLI_DIR)/build/make/all.vars:
	$(call ShowVariables,PROJECTS)

endif
# __ROOT_ALL__
