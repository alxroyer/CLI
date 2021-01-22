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


//! @file
//! @author Alexis Royer
//! @brief CcliConsole class definition.

#ifndef _CLI_CONSOLE_H_
#define _CLI_CONSOLE_H_

#include "cli_io_device.h"


//! @brief Console intput/output device class.
class CcliConsole : public CcliIODevice
{
public:
    //! @brief Constructor.
    CcliConsole(void);

    //! @brief Destructor.
    virtual ~CcliConsole(void);

public:
    //! @brief Open device handler.
    virtual const bool OpenDevice(void);
    //! @brief Close device handler.
    virtual const bool CloseDevice(void);
    //! @brief Character input handler.
    virtual const CLI_CHAR GetChar(void) const;

protected:
    //! @brief Output handler.
    virtual void PutString(const std::string& STR_Out) const;
public:
    //! @brief Beep handler.
    virtual void Beep(void) const;
};

#endif // _CLI_CONSOLE_H_

