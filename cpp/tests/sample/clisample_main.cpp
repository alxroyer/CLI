/*
    Copyright (c) 2006-2007, Alexis Royer

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


#include <iostream>

#include "cli/common.h"
#include "cli/shell.h"
#include "cli/console.h"
#include "cli/telnet.h"


//! @brief Usage display.
void PrintHelp(std::ostream& STD_Stream)
{
    STD_Stream << "USAGE:" << std::endl;
    STD_Stream << "   clisample        : run in a console" << std::endl;
    STD_Stream << "   clisample [port] : run as a tcp server on the given port" << std::endl;
}

int main(int I_Args, char** ARSTR_Args)
{
    unsigned long ul_TcpPort = 0;
    if (I_Args > 1)
    {
        if (const unsigned long ul_Tmp = atoi(ARSTR_Args[0]))
        {
            ul_TcpPort = ul_Tmp;
        }
        else if (   (std::string(ARSTR_Args[0]) == "-help")
                    || (std::string(ARSTR_Args[0]) == "-h")
                    || (std::string(ARSTR_Args[0]) == "--help")
                    || (std::string(ARSTR_Args[0]) == "-?"))
        {
            PrintHelp(std::cout);
            return 0;
        }
        else
        {
            std::cerr << "Unknown option '" << ARSTR_Args[0] << "'" << std::endl;
            PrintHelp(std::cerr);
            return -1;
        }
    }

    // Look for a CLI to launch.
    cli::CliList cli_List;
    cli::Cli::FindFromName(cli_List, ".*");
    if (cli_List.size() == 0)
    {
        std::cerr << "No CLI found" << std::endl;
        return -1;
    }
    else if (cli_List.size() != 1)
    {
        std::cerr << "Several CLI found" << std::endl;
    }

    // Create a shell.
    cli::Shell cli_Shell(*cli_List[0]);

    // Disable following lines if you wish to disable streams.
    // You can also redirect to something else.
    //  cli_Shell.SetStream(cli::WELCOME_STREAM, cli::OutputDevice::GetNullDevice());
    //  cli_Shell.SetStream(cli::PROMPT_STREAM, cli::OutputDevice::GetNullDevice());
    //  cli_Shell.SetStream(cli::ECHO_STREAM, cli::OutputDevice::GetNullDevice());
    //  cli_Shell.SetStream(cli::OUTPUT_STREAM, cli::OutputDevice::GetNullDevice());
    //  cli_Shell.SetStream(cli::ERROR_STREAM, cli::OutputDevice::GetNullDevice());

    // Launch it.
    if (ul_TcpPort != 0)
    {
        // Run as a telnet server.
        cli::TelnetServer cli_TelnetServer(cli_Shell, ul_TcpPort);
        std::cout << "Starting server on port " << ul_TcpPort << std::endl;
        cli_TelnetServer.StartServer();
    }
    else
    {
        // Run in a console.
        cli::Console cli_Console(false);
        cli_Shell.Run(cli_Console);
    }

    // Successful return.
    return 0;
}
