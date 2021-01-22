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


#ifndef _CLI_GO_IO_DEVICE_H_
#define _CLI_GO_IO_DEVICE_H_

#include <cli/shell.h>
#include <cli/io_mux.h>


//! @brief The CLI of this program.
const cli::Cli& GetCli(void);

//! @brief The shell of this program.
cli::Shell& GetShell(void);

//! @brief This program input / output multiplexer.
cli::IOMux& GetIOMux(void);

//! @brief Device setting method.
const bool SetDevice(
    const bool B_ShellTarget,               //!< true if the target of the device is the shell,
                                            //!< false if it is the multiplexer above.
    cli::STREAM_TYPE E_Use,                 //!< Indicates what the device should be used for.
                                            //!< STREAM_TYPES_COUNT means input here.
    cli::OutputDevice* const PCLI_Device    //!< Device set.
    );

#endif // _CLI_GO_IO_DEVICE_H_

