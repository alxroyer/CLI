/*
    Copyright (c) 2006-2010, Alexis Royer, http://alexis.royer.free.fr/CLI

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
        * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

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


#include "cli/pch.h"
#include "cli/common.h"
#include "cli/telnet.h"

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

    cli::Cli::List cli_List(10);
    const int i_Clis = cli::Cli::FindFromName(cli_List, ".*");
    if (i_Clis == 0)
    {
        cli::OutputDevice::GetStdErr() << "Error: No CLI found." << cli::endl;
        return -1;
    }
    else if (i_Clis > 1)
    {
        cli::OutputDevice::GetStdErr() << "Warning: Several CLIs found. Executing only the first one." << cli::endl;
    }

    if (const cli::Cli* const pcli_Cli = cli_List.GetHead())
    {
        cli::Shell cli_MyShell(*pcli_Cli);
        cli::OutputDevice::GetStdOut() << "Starting server on port " << ul_Port << "." << cli::endl;
        cli::TelnetServer cli_Server(cli_MyShell, ul_Port);
        cli_Server.StartServer();
        cli::OutputDevice::GetStdOut() << "Server is done." << cli::endl;
        return 0;
    }
    else
    {
        cli::OutputDevice::GetStdErr() << "Internal error." << cli::endl;
        return -1;
    }
}
