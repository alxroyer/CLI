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


# Default goal
.DEFAULT_GOAL ?= headers
.PHONY: jni.default
jni.default: headers ;

# Variables.
include vars.mak

JAVA_FILES = $(wildcard $(JAVA_SRC_DIR)/cli/*.java)
JAVA_CLASSES = $(patsubst $(JAVA_SRC_DIR)/cli/%.java,$(OUT_DIR)/cli/%.class,$(JAVA_FILES))
CPP_HEADERS = $(patsubst $(OUT_DIR)/cli/%.class,$(NATIVE_DIR)/cli_%.h,$(JAVA_CLASSES))


# Rules.
.PHONY: headers
headers: libclijava $(CPP_HEADERS) ;

.PHONY: libclijava
libclijava:
	$(call MkCall,libclijava.mak,)

$(NATIVE_DIR)/cli_%.h: $(JAVA_SRC_DIR)/cli/%.java
	javah -classpath $(OUT_DIR) -d $(NATIVE_DIR) $(patsubst $(JAVA_SRC_DIR)/cli/%.java,cli.%,$<) && touch $@

.PHONY: deps
deps: ;

.PHONY: clean
clean:
	$(RM) $(CPP_HEADERS)

# Debug and help
include $(ROOT_DIR)/build/make/help.mak

.PHONY: $(JAVA_DIR)/build/make/jni.help
$(JAVA_DIR)/build/make/jni.help:
	$(call PrintHelp, headers, Generate native header files)
	$(call PrintHelp, clean, Clean output files)

.PHONY: $(JAVA_DIR)/build/make/jni.vars
$(JAVA_DIR)/build/make/jni.vars:
	$(call ShowVariables,JAVA_FILES JAVA_CLASSES CPP_HEADERS)

