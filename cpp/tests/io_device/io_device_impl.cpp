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
#include <cli/cli.h>
#include <cli/file_device.h>
#include <cli/traces.h>

#include "io_device_impl.h"


const cli::Cli& GetCli(void)
{
    cli::CliList cli_List;
    if ((cli::Cli::FindFromName(cli_List, ".*") > 0)
        && (! cli_List.empty()))
    {
        // Return first CLI if any.
        return *cli_List[0];
    }
    else
    {
        // Should not happen.
        assert(false);
        class MyCli : public cli::Cli { public:
            MyCli(void) : cli::Cli("", cli::Help()) {}
        };
        static MyCli cli_MyCli;
        return cli_MyCli;
    }
}

cli::Shell& GetShell(void)
{
    // Ensure IOMux creation.
    GetIOMux();

    static cli::Shell cli_Shell(GetCli());
    return cli_Shell;
}

cli::IOMux& GetIOMux(void)
{
    static cli::IOMux cli_IOMux(false);
    return cli_IOMux;
}

int main(int I_Args, char* ARSTR_Args[])
{
    if (I_Args != 3)
    {
        std::cerr << "USAGE:" << std::endl;
        std::cerr << "   io_device <input-file> <traces-output-file>" << std::endl;
        return -1;
    }

    // Enable traces.
    cli::GetTraces().Trace(cli::TraceClass("IO_DEVICE_INSTANCES", ""));
    cli::GetTraces().SetFilter(cli::TraceClass("IO_DEVICE_INSTANCES", ""), true);
    cli::GetTraces().Trace(cli::TraceClass("IO_DEVICE_OPENING", ""));
    cli::GetTraces().SetFilter(cli::TraceClass("IO_DEVICE_OPENING", ""), true);
    cli::GetTraces().SetStream(*new cli::OutputFileDevice(ARSTR_Args[2], true));

    // Input.
    GetIOMux().AddInput(new cli::InputFileDevice(ARSTR_Args[1], cli::OutputDevice::GetStdOut(), true));

    // Execute.
    GetShell().Run(GetIOMux());

    return 0;
}

const bool SetDevice(
        const bool B_ShellTarget,
        cli::STREAM_TYPE E_Use,
        cli::OutputDevice* const PCLI_Device)
{
    bool b_Res = false;

    // Check input parameters.
    if ((E_Use >= 0) && (E_Use <= cli::STREAM_TYPES_COUNT)
        && (PCLI_Device != NULL))
    {
        // First of all, lock the device instance.
        PCLI_Device->UseInstance(__CALL_INFO__);

        if (E_Use < cli::STREAM_TYPES_COUNT)
        {
            // Output streams.
            if (B_ShellTarget)
            {
                b_Res = GetShell().SetStream(E_Use, *PCLI_Device);
            }
            else
            {
                b_Res = GetIOMux().SetOutput(E_Use, PCLI_Device);
            }
        }
        else if (cli::IODevice* const pcli_IODevice = dynamic_cast<cli::IODevice*>(PCLI_Device))
        {
            // Input streams.
            if (B_ShellTarget)
            {
                GetShell().GetStream(cli::ERROR_STREAM) << "Cannot set input for shell" << cli::endl;
            }
            else
            {
                b_Res = GetIOMux().AddInput(pcli_IODevice);
            }
        }
        else
        {
            GetShell().GetStream(cli::ERROR_STREAM) << "Invalid parameters" << cli::endl;
        }

        // Eventually free the device instance.
        PCLI_Device->FreeInstance(__CALL_INFO__);
    }
    else
    {
        GetShell().GetStream(cli::ERROR_STREAM) << "Invalid parameters" << cli::endl;
    }

    return b_Res;
}

