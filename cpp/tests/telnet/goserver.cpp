/*
    Copyright (c) 2006-2018, Alexis Royer, http://alexis.royer.free.fr/CLI

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
          and/or other materials provided with the distribution.
        * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
          without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <stdlib.h> // atoi

#include "cli/pch.h"
#include "cli/common.h"
#include "cli/telnet.h"
#include "pwd_shell.h"
#include "empty.h"


class TestServer : public cli::TelnetServer
{
public:
    explicit TestServer(const unsigned long UL_Port) : TelnetServer(2, UL_Port, cli::ResourceString::LANG_EN) {}
protected:
    virtual cli::ExecutionContext* const OnNewConnection(const cli::TelnetConnection& CLI_NewConnection)
    {
        if (const cli::Cli* const pcli_Cli = new EmptyCli())
        {
            if (cli::ExecutionContext* const pcli_NewContext = new PwdShellContext(*pcli_Cli, "hello"))
            {
                pcli_NewContext->SetStream(cli::WELCOME_STREAM, cli::OutputDevice::GetNullDevice());
                return pcli_NewContext;
            }
        }
        return NULL;
    }
    virtual void OnCloseConnection(const cli::TelnetConnection& CLI_ConnectionClosed, cli::ExecutionContext* const PCLI_Context)
    {
        if (PwdShellContext* const pcli_PwdShell = dynamic_cast<PwdShellContext*>(PCLI_Context))
        {
            const cli::Cli* const pcli_Cli = & pcli_PwdShell->GetShell().GetCli();

            // Delete the shell before the cli for consistency.
            delete pcli_PwdShell;
            if (pcli_Cli != NULL)
            {
                delete pcli_Cli;
            }
        }
    }
};

int main(int I_ArgCount, char* ARSTR_Args[])
{
    unsigned long ul_Port = 0;
    if ((I_ArgCount < 2) || (ARSTR_Args[0] == NULL))
    {
        cli::OutputDevice::GetStdErr() << "USAGE: telnet <port>" << cli::endl;
        cli::OutputDevice::GetStdErr() << "   port: TCP port to listen onto" << cli::endl;
        return -1;
    }
    else
    {
        ul_Port = atoi(ARSTR_Args[1]);
        if (ul_Port == 0)
        {
            cli::OutputDevice::GetStdErr() << "Invalid port " << ARSTR_Args[0] << cli::endl;
            return -1;
        }
    }

    static const cli::TraceClass CLI_TELNET_SERVER("CLI_TELNET_SERVER", cli::Help());
    static const cli::TraceClass CLI_TELNET_IN("CLI_TELNET_IN", cli::Help());
    static const cli::TraceClass CLI_TELNET_OUT("CLI_TELNET_OUT", cli::Help());
    static const cli::TraceClass CLI_EXEC_CTX("CLI_EXEC_CTX", cli::Help());
    class _Trace { public:
        explicit _Trace() {
            cli::GetTraces().SetStream(cli::OutputDevice::GetStdErr());
            cli::GetTraces().Declare(CLI_TELNET_SERVER);    cli::GetTraces().SetFilter(CLI_TELNET_SERVER, false);
            cli::GetTraces().Declare(CLI_TELNET_IN);        cli::GetTraces().SetFilter(CLI_TELNET_IN, false);
            cli::GetTraces().Declare(CLI_TELNET_OUT);       cli::GetTraces().SetFilter(CLI_TELNET_OUT, false);
            cli::GetTraces().Declare(CLI_EXEC_CTX);         cli::GetTraces().SetFilter(CLI_EXEC_CTX, false);
        }
        ~_Trace() {
            cli::GetTraces().UnsetStream(cli::OutputDevice::GetStdErr());
        }
    } guard;

    TestServer cli_Server(ul_Port);
    cli_Server.StartServer();
    return 0;
}
