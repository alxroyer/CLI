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
#include "cli/file_device.h"
#include "cli/telnet.h"


int main(int I_Args, char** ARSTR_Args)
{
    // Check arguments.
    if (I_Args != 3)
    {
        std::cout << "USAGE" << std::endl;
        std::cout << "   " << ARSTR_Args[0] << " <input file> <output file>" << std::endl;
        return -1;
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

    // Create devices.
    cli::OutputFileDevice cli_Output(ARSTR_Args[2], false);
    cli::InputFileDevice cli_Input(ARSTR_Args[1], cli_Output, false);

    // Redirect only echo, prompt, output and error streams.
    cli_Shell.SetStream(cli::WELCOME_STREAM, cli::OutputDevice::GetNullDevice());

    // Launch it.
    cli_Shell.Run(cli_Input);

    // Successful return.
    return 0;
}
