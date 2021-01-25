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
.DEFAULT_GOAL = run
.PHONY: telnet.default
telnet.default: $(.DEFAULT_GOAL) ;

# Includes
CLI_DIR := ../../..
include _vars.mak
PRODUCT = telnet
SRC_DIR = $(CLI_DIR)/java/src/cli/test
JAVA_FILES = $(CLI_DIR)/java/src/cli/test/TestTelnet.java
PROJECT_DEPS = libclijava.mak jni.mak native.mak
include _build.mak

# Rules
run: build
	java $(JAVA_PATH) $(JAVA_LIBS) cli.test.TestTelnet

TestSimple TestUI MultiConnection StopServer: build
	java $(JAVA_PATH) $(JAVA_LIBS) cli.test.TestTelnet $@

.PHONY: deps
deps: ;

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/java/build/make/telnet.help
help: $(CLI_DIR)/java/build/make/telnet.help
$(CLI_DIR)/java/build/make/telnet.help:
	$(call PrintHelp, run, 	 Run the telnet sample)

.PHONY: $(CLI_DIR)/java/build/make/telnet.vars
vars: $(CLI_DIR)/java/build/make/telnet.vars
$(CLI_DIR)/java/build/make/telnet.vars: ;

# Dependencies
build: $(JAVA_FILES)
