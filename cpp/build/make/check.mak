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


ifndef __CPP_CHECK__
__CPP_CHECK__ = 1


# Default goal
DEFAULT_GOAL ?= check
.PHONY: cppcheck.default
cppcheck.default: check ;

# Variables
CLI_TEST = $(patsubst %.xml,%.test,$(CLI_XML_RES))
CLI_LOG = $(patsubst %.xml,$(INT_DIR)/%.log,$(notdir $(CLI_XML_RES)))
CLI_CHECK = $(patsubst %.xml,%.check,$(CLI_XML_RES))

# Includes
PROJ_CLEAN += $(CLI_LOG)
include mkres.mak

# Rules
.PHONY: log
log: build_depends $(CLI_LOG) ;

$(CLI_LOG): $(CLI_BINARY) $(CLI_BINARY) $(CLI_TEST)
	$(CLI_BINARY) $(CLI_TEST) $(CLI_LOG) 1> /dev/null 2> /dev/null
	chmod a+x $(CPP_DIR)/tests/cleanlog.sh
	$(CPP_DIR)/tests/cleanlog.sh $(CLI_LOG)

.PHONY: check
check: build_depends $(CLI_LOG) $(CLI_CHECK)
	diff $(CLI_LOG) $(CLI_CHECK)

# Debug and help
include $(CLI_DIR)/build/make/help.mak

.PHONY: $(CPP_DIR)/build/make/check.help
$(CPP_DIR)/build/make/check.help:
	$(call PrintHelp, check, Check test output)
	$(call PrintHelp, log, Generate $(notdir $(CLI_LOG)))

.PHONY: $(CPP_DIR)/build/make/check.vars
$(CPP_DIR)/build/make/check.vars:
	$(call ShowVariables,CLI_TEST CLI_LOG CLI_CHECK)

endif
# __CPP_CHECK__
