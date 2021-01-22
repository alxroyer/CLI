/*
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006-2007 Alexis Royer.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
