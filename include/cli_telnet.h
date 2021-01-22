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
//! @brief CcliTelnetServer and CcliTelnetConnection classes definition.

#ifndef _CLI_TELNET_H_
#define _CLI_TELNET_H_

#include <string>
#include <deque>

#include "cli_io_device.h"


// Forward declarations.
class CcliShell;


//! @brief Telnet server class.
class CcliTelnetServer
{
public:
    //! @brief Constructor.
    CcliTelnetServer(
        CcliShell& CLI_Shell,           //!< Corresponding shell.
        const unsigned long UL_TcpPort  //!< TCP port to listen onto.
        );

    //! @brief Destructor.
    virtual ~CcliTelnetServer(void);

public:
    //! @brief Starts the server.
    void StartServer(void);
private:
    //! @brief Accepts a connection.
    const bool AcceptConnection(
        const int I_Socket      //!< Connection socket handler.
        );
private:
    //! Shell reference.
    CcliShell& m_cliShell;
    //! Listened TCP port.
    const unsigned long m_ulTcpPort;
};


//! @brief Telnet connection input/output device.
class CcliTelnetConnection : public CcliIODevice
{
public:
    //! @brief Constructor.
    CcliTelnetConnection(
        CcliShell& CLI_Shell,   //!< Shell reference.
        const int I_Socket      //!< Connection socket handler.
        );

    //! @brief Destructor.
    virtual ~CcliTelnetConnection(void);

public:
    //! @brief Open device handler.
    virtual const bool OpenDevice(void);
    //! @brief Close device handler.
    virtual const bool CloseDevice(void);
    //! @brief Character input handler.
    virtual const CLI_CHAR GetChar(void) const;

private:
    //! @brief Output handler.
    virtual void PutString(const std::string& STR_Out) const;
public:
    virtual void Beep(void) const;

private:
    //! Connection socket handler.
    const int m_iSocket;
    //! Shell reference.
    CcliShell& m_cliShell;
    //! Character input buffer.
    mutable std::deque<int> m_qiChars;
};

#endif // _CLI_TELNET_H_

