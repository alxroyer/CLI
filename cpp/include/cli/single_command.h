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


//! @file
//! @author Alexis Royer
//! @brief SingleCommand class definition.

#ifndef _CLI_SINGLE_COMMAND_H_
#define _CLI_SINGLE_COMMAND_H_

#include <string>

#include <cli/io_device.h>


namespace cli {

    //! @brief Single command device.
    class SingleCommand : public IODevice
    {
    public:
        //! @brief Constructor.
        SingleCommand(
            const std::string& STR_Command,     //!< Command.
            OutputDevice& CLI_Output,           //!< Output.
            const bool B_AutoDelete             //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~SingleCommand(void);

    public:
        //! @brief Command accessor.
        const std::string& GetCommand(void) const;

    protected:
        virtual const bool OpenDevice(void);
        virtual const bool CloseDevice(void);
    public:
        virtual const KEY GetKey(void) const;
        virtual void PutString(const std::string& STR_Out) const;
        virtual void Beep(void) const;

    private:
        //! Output device.
        OutputDevice& m_cliOutput;

        //! The command.
        const std::string m_strCommand;

        //! Current position in the command.
        mutable int m_iPosition;
    };

};

#endif // _CLI_SINGLE_COMMAND_H_
