/*
    Copyright (c) 2006-2009, Alexis Royer

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
#include "cli/shell.h"
#include "cli/file_device.h"


int main(int I_Args, char** ARSTR_Args)
{
    // Check arguments.
    if (I_Args != 3)
    {
        cli::OutputDevice::GetStdOut() << "USAGE" << cli::endl;
        cli::OutputDevice::GetStdOut() << "   " << ARSTR_Args[0] << " <input file> <output file>" << cli::endl;
        return -1;
    }

    // Look for a CLI to launch.
    cli::Cli::List cli_List(10);
    cli::Cli::FindFromName(cli_List, ".*");
    if (cli_List.IsEmpty())
    {
        cli::OutputDevice::GetStdErr() << "No CLI found" << cli::endl;
        return -1;
    }
    else if (cli_List.GetCount() > 1)
    {
        cli::OutputDevice::GetStdErr() << "Several CLI found" << cli::endl;
    }

    // Create a shell.
    cli::Shell cli_Shell(*cli_List.GetHead());

    // Create devices.
    cli::OutputFileDevice cli_Output(ARSTR_Args[2], false);
    cli::InputFileDevice cli_Input(ARSTR_Args[1], cli_Output, false);
    cli_Input.EnableSpecialCharacters(true);

    // Redirect only echo, prompt, output and error streams.
    cli_Shell.SetStream(cli::WELCOME_STREAM, cli::OutputDevice::GetNullDevice());

    // Launch it.
    cli_Shell.Run(cli_Input);

    // Successful return.
    return 0;
}
