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

#include <cli/common.h>
#include <cli/console.h>
#include <cli/traces.h>


int main(void) {
    // Find the empty CLI.
    cli::CliList cli_CLIs;
    if ((cli::Cli::FindFromName(cli_CLIs, ".*") <= 0)
        || (cli_CLIs.empty()))
    {
        std::cerr << "No CLI found" << std::endl;
        return -1;
    }

    // Launch it.
    cli::Shell cli_Shell(*cli_CLIs[0]);
    cli::Console cli_Console(false);
    cli_Shell.Run(cli_Console);
}
