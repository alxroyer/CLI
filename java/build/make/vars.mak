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


ifndef __JAVA_VARS__
__JAVA_VARS__ = 1


CLI_DIR ?= ../../..

include $(CLI_DIR)/build/make/vars.mak
include $(CPP_DIR)/build/make/vars.mak


# Directories
ifeq ($(JAVA_HOME),)
$(error Please set JAVA_HOME to your SDK installation directory)
endif
JDK_DIR ?= $(JAVA_HOME)
JAVA_SRC_DIR = $(JAVA_DIR)/src
NATIVE_DIR = $(JAVA_DIR)/native
OUT_DIR = $(RDX)

# Java flags
JAVA_PATH = -classpath $(OUT_DIR)
ifdef _DEBUG
	JAVAC_FLAGS = -g
else
	JAVAC_FLAGS =
endif
JAVAC_FLAGS += -d $(OUT_DIR)

# Products
JAVA_ARCHIVE = $(OUT_DIR)/cli.jar
JAVA_DYN_LIB = $(OUT_DIR)/$(DYN_LIB_PREFIX)cli$(DYN_LIB_SUFFIX)

# Debug and help
.PHONY: $(JAVA_DIR)/build/make/vars.help
$(JAVA_DIR)/build/make/vars.help: ;

.PHONY: $(JAVA_DIR)/build/make/vars.vars
$(JAVA_DIR)/build/make/vars.vars:
	$(call ShowVariables,JDK_DIR JAVA_SRC_DIR NATIVE_DIR OUT_DIR JAVA_PATH JAVAC_FLAGS JAVA_ARCHIVE JAVA_DYN_LIB)

endif
# __JAVA_VARS__
