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


# Includes
CLI_DIR := ../../..
SRC_DIR = ../../tests/tuning
CLI_XML_RES = $(SRC_DIR)/tuning.xml
SRC_DIR = $(CPP_DIR)/tests/tuning
INT_DIR = $(OUT_DIR)/__tuning
include _check.mak
CPP_FILES += $(SRC_DIR)/gotuning.cpp

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CPP_DIR)/build/make/tuning.help
help: $(CPP_DIR)/build/make/tuning.help
$(CPP_DIR)/build/make/tuning.help: ;

# Dependencies
$(INT_DIR)/gotuning.o: $(SRC_DIR)/gotuning.cpp
$(PRODUCT): $(INT_DIR)/gotuning.o
