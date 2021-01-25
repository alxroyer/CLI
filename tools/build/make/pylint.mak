# Copyright (c) 2006-2018, Alexis Royer, http://alexis.royer.free.fr/CLI
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
#       and/or other materials provided with the distribution.
#     * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
#       without specific prior written permission.
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


# This makefile provides a mean to call pylint checkings defined in _python.mak.
# Pylint checkings were originally automatically called from other makefiles in this directory,
# but have been eventually removed, due to the wide spread of pylint versions encountered in the various environment developments,
# resulting in incompatible behaviours and configurations.
# As a consequence, pylint checkings are not mandatory anymore, but called on demand, thanks to this makefile.

# Default goal
.DEFAULT_GOAL = check.stop
.PHONY: pylint.default
pylint.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
OUT_DIR = __pylint
include _vars.mak
include _python.mak
include $(CLI_DIR)/build/make/_utils.mak

# Rules
.PHONY: check
check: check.nostop ;

.PHONY: check.nostop
check.nostop:
	make -f pylint.mak check.stop || true

.PHONY: check.stop
check.stop: $(PYLINT_RESULT) ;

.PHONY: deps
deps: ;

.PHONY: clean
clean: clean.pylint
	$(call RemoveDir,$(OUT_DIR))

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/build/make/pylint.help
help: $(CLI_DIR)/build/make/pylint.help
$(CLI_DIR)/build/make/pylint.help:
	$(call PrintHelp, check,        Regular 'make check' rule from to main Makefile falls back to check.nostop)
	$(call PrintHelp, check.nostop, Pylint check: no error code on pylint errors, pylint errors are only displayed)
	$(call PrintHelp, check.nostop, Pylint check: error code returned on pylint errors)
	$(call PrintHelp, clean,        Clean all intermediate files)

.PHONY: $(CLI_DIR)/build/make/cli2cpp.vars
vars: $(CLI_DIR)/build/make/cli2cpp.vars
$(CLI_DIR)/build/make/cli2cpp.vars: ;
