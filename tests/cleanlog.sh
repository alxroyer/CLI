#!/bin/sh

# This file is part of the CLI library.  The CLI library aims to provide
# facilities in making text-based user interfaces.
# Copyright (C) 2006 Alexis Royer.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


CLI_LOG="$1"

# Get the test result
CLI_LOG_DATA=$(cat "$CLI_LOG")

# Find out new lines
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | cat -v | sed -e "s/\^M\^\[\[B/\n/g")

# Other form of new lines
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | cat -v | sed -e "s/\^\[\[B\^H/\n/g")

# Backspaces
while [ $(echo "$CLI_LOG_DATA" | cat -v | grep ".\^H ^H" | wc -l) -gt 0 ]
do
    CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | cat -v | sed -e "s/.\^H ^H//")
done

# Remove the first line
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | cat -v | sed -e "s/^\^\[7\^\[\[?47h\^\[\[0;10m\^\[\[4l\^\[\[H\^\[\[J//")

# Remove the last line
CLI_LOG_DATA=$(echo "$CLI_LOG_DATA" | cat -v | sed -e "s/^\^\[\[..;1H\^\[\[2J\^\[\[?47l\^\[8//")

# Overwrite the log file
echo "$CLI_LOG_DATA" > "$CLI_LOG"

