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


# Default goal
.DEFAULT_GOAL = check
.PHONY: telnet.default
telnet.default: $(.DEFAULT_GOAL) ;

# Avoid parallel rules execution
.NOTPARALLEL: ;

# Variables
CLI_DIR := ../../..
SRC_DIR = $(CLI_DIR)/cpp/tests/telnet
include _vars.mak
include $(CLI_DIR)/build/make/_utils.mak
PROJECT = test_telnet_multi_clients
INT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
OUT_DIR = $(TARGET)$(CXX)/$(RDX)/__$(PROJECT)
SERVER_BINARY = $(TARGET)$(CXX)/$(RDX)/__test_telnet_server/$(BIN_PREFIX)test_telnet_server$(BIN_SUFFIX)
CLIENT_BINARY = $(TARGET)$(CXX)/$(RDX)/__test_telnet_client/$(BIN_PREFIX)test_telnet_client$(BIN_SUFFIX)
TELNET_PORT = 9012
TELNET_INPUT = $(INT_DIR)/empty.test
TELNET_LOG = $(INT_DIR)/empty.log
TELNET_CHECK = $(INT_DIR)/empty.check

# Rules

# Main rule
# Memo: The server only accepts 2 connections at once.
# Scenario: Start the server, make 3 connections one after the other, stop the server
.PHONY: check
check: start-server check.1 check.2 check.3 stop-server ;

.PHONY: start-server
start-server: $(SERVER_BINARY)
	@echo "  === Starting server ==="
	$(call CheckSh,test_telnet.kill.sh)
	./test_telnet.kill.sh
	$(SERVER_BINARY) $(TELNET_PORT) &
# Let the time for the server to start
	sleep 1

.PHONY: stop-server
stop-server:
	@echo "  === Stopping server ==="
	$(call CheckSh,test_telnet.kill.sh)
	./test_telnet.kill.sh

.PHONY: check.%
check.%: $(TELNET_LOG).% $(TELNET_CHECK)
	@echo "  === Checking result ==="
	diff $< $(TELNET_CHECK)

# No final 'quit' line in order to make the clients terminate with a connection error
# Also remove last blank line in order to avoid a prompt repetition (see: https://stackoverflow.com/questions/4881930/remove-the-last-line-from-a-file-in-bash)
$(TELNET_INPUT): $(CLI_DIR)/samples/user-guide/empty.test test_telnet_multi_clients.mak
	@echo "  === Builing $@ ==="
	rm -f $@
	$(call CheckDir,$(dir $@))
	echo "" >> $@
	echo "bad" >> $@
	echo "hello" >> $@
	cat $< | grep -v "^quit$$" | sed "$$ d" >> $@
	dos2unix $@ 2> /dev/null

# No final 'quit' line neither
$(TELNET_CHECK): $(CLI_DIR)/samples/user-guide/empty.check test_telnet_multi_clients.mak
	@echo "  === Builing $@ ==="
	rm -f $@
	$(call CheckDir,$(dir $@))
	echo "Enter password: " >> $@
	echo "Enter password: ***" >> $@
	echo "Wrong password!" >> $@
	echo "Enter password: *****" >> $@
	cat $< | sed -e "s/[^>]*: Syntax error next to /Syntax error next to /" | sed -e "s/^cli-config>quit$$/cli-config>/" >> $@

# Avoid output logs being considered as intermediate files
.PRECIOUS: $(TELNET_LOG).1 $(TELNET_LOG).2 $(TELNET_LOG).3
$(TELNET_LOG).%: $(CLIENT_BINARY) $(TELNET_INPUT) $(CLI_DIR)/tools/clean_outlog.py test_telnet_multi_clients.mak
	@echo "  === Builing $@ ==="
	$(call CheckDir,$(dir $@))
	$(CLIENT_BINARY) $(TELNET_PORT) $(TELNET_INPUT) > $@
	python $(CLI_DIR)/tools/clean_outlog.py $@
# Let the time for the server to terminate the connection
	sleep 1

.PHONY: server $(SERVER_BINARY)
server: $(SERVER_BINARY)
$(SERVER_BINARY):
	@echo "  === Builing server ==="
	$(MAKE) -f test_telnet.mak TELNET_GOAL=server

.PHONY: client $(CLIENT_BINARY)
client: $(CLIENT_BINARY)
$(CLIENT_BINARY):
	@echo "  === Builing client ==="
	$(MAKE) -f test_telnet.mak TELNET_GOAL=client

.PHONY: deps
deps:
	$(MAKE) -f test_telnet.mak TELNET_GOAL=server deps
	$(MAKE) -f test_telnet.mak TELNET_GOAL=client deps

.PHONY: clean
clean:
	$(MAKE) -C $(CLI_DIR)/cpp/build/make -f test_telnet.mak TELNET_GOAL=server clean
	$(MAKE) -C $(CLI_DIR)/cpp/build/make -f test_telnet.mak TELNET_GOAL=client clean
	rm -f $(TELNET_INPUT) $(TELNET_LOG).1 $(TELNET_LOG).2 $(TELNET_LOG).3 $(TELNET_CHECK)
	$(call RemoveDir,$(INT_DIR))

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/cpp/build/make/telnet.help
help: $(CLI_DIR)/cpp/build/make/telnet.help
$(CLI_DIR)/cpp/build/make/telnet.help:
	$(call PrintHelp, check,  Check test output)
	$(call PrintHelp, server, Generate the server program)
	$(call PrintHelp, client, Generate the client program)
	$(call PrintHelp, clean,  Clean outputs)

.PHONY: $(CLI_DIR)/cpp/build/make/telnet.vars
vars: $(CLI_DIR)/cpp/build/make/telnet.vars
$(CLI_DIR)/cpp/build/make/telnet.vars:
	$(call ShowVariables,TELNET_GOAL SRC_DIR INT_DIR SERVER_BINARY CLIENT_BINARY TELNET_PORT TELNET_INPUT TELNET_LOG TELNET_CHECK)
