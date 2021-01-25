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


# Memo:
#  This test is actually a base project for integrating JNI concerns step by step.
#  If the execution succeeds, it is a good symptom for this correctness of JNI executions.
#
#  It has been set up for restoring the Java library execution on a 64 bits architecture.
#  libclijava.mak, jni.mak and native.mak have temporarily adujsted to compile and check the files in java/src/cli and java/native one by one.
#
#  See java/src/cli/test/TestNative.java for details on tests executed.


# Default goal
.DEFAULT_GOAL = check
.PHONY: test_native.default
test_native.default: $(.DEFAULT_GOAL) ;


# Includes
CLI_DIR := ../../..
include _vars.mak
include _build.mak
SRC_DIR = $(CLI_DIR)/java/src/cli/test
JAVA_FILES = $(SRC_DIR)/TestNative.java
PROJECT_DEPS += libclijava.mak
PROJECT_DEPS += jni.mak
PROJECT_DEPS += native.mak

.PHONY: check
check: run ;

.PHONY: run
run: build
	$(call RunJava,cli.test.TestNative)
