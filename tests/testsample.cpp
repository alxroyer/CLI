/*
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006 Alexis Royer.

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

#include "cli_common.h"
#include "cli_shell.h"
#include "cli_console.h"
#include "cli_telnet.h"


int main(int I_Args, char** ARSTR_Args)
{
    // Look for a CLI to launch.
    CcliCliList cli_List;
    CcliCli::FindFromName(cli_List, ".*");
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
    CcliShell cli_Shell(*cli_List[0]);
    // Enable only echo, prompt, output and error streams.
    cli_Shell.SetWelcomeStream(CcliOutputDevice::GetNullDevice());

    // Launch it.
    CcliConsole cli_Console;
    cli_Shell.Run(cli_Console);

    // Successful return.
    return 0;
}
