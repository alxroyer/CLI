#!/bin/sh

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


CLI_LOG="$1"

# Get the test result
CLI_LOG_DATA=$(cat "$CLI_LOG")

# Backspaces
while [ $(echo "$CLI_LOG_DATA" | cat -v | grep ".\^H\(\^H\)*\( \)* \^H\(\^H\)*" | wc -l) -gt 0 ]
do
    # CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | cat -v | sed -e "s/.\^H\(\^H\)*\( \)* \^H\(\^H\)*/\1\2\3/")
    CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | cat -v | sed -e "s/.\(\(\^H\)*\)\^H \(\( \)*\)\(\(\^H\)*\)\^H/\1\3\5/g")
done

# Windows-like end of lines
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | sed -e "s/\^M//g")

# Special characters
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | sed -e "s/M-g/�/g")
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | sed -e "s/M-i/�/g")

# Remove traces menu (debug mode)
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | grep -v "traces          Traces menu")
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | grep -v "traces          Menu de configuration de traces")
# Remove check menu (debug mode)
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | grep -v "check           Check CLI stuff")
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | grep -v "check           V�rifications du CLI")

# Check line endings
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | sed -e "s/\r\n/\n/g")

# Overwrite the log file
echo "$CLI_LOG_DATA" > "$CLI_LOG"

