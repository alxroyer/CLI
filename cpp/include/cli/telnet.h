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
//! @brief TelnetServer and TelnetConnection classes definition.

#ifndef _CLI_TELNET_H_
#define _CLI_TELNET_H_

#include <string>
#include <deque>

#include <cli/io_device.h>


namespace cli {

    // Forward declarations.
    class Shell;


    //! @brief Telnet server class.
    class TelnetServer
    {
    public:
        //! @brief Constructor.
        TelnetServer(
            Shell& CLI_Shell,               //!< Corresponding shell.
            const unsigned long UL_TcpPort  //!< TCP port to listen onto.
            );

        //! @brief Destructor.
        virtual ~TelnetServer(void);

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
        Shell& m_cliShell;
        //! Listened TCP port.
        const unsigned long m_ulTcpPort;
    };


    //! @brief Telnet connection input/output device.
    class TelnetConnection : public IODevice
    {
    public:
        //! @brief Constructor.
        TelnetConnection(
            Shell& CLI_Shell,       //!< Shell reference.
            const int I_Socket,     //!< Connection socket handler.
            const bool B_AutoDelete //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~TelnetConnection(void);

    protected:
        //! @brief Open device handler.
        virtual const bool OpenDevice(void);
        //! @brief Close device handler.
        virtual const bool CloseDevice(void);
    public:
        //! @brief Character input handler.
        virtual const KEY GetKey(void) const;
        //! @brief Output handler.
        virtual void PutString(const std::string& STR_Out) const;
        //! @brief Beep handler.
        virtual void Beep(void) const;

    private:
        //! Shell reference.
        Shell& m_cliShell;
        //! Connection socket handler.
        const int m_iSocket;
        //! Character input buffer.
        mutable std::deque<int> m_qiChars;
    };

};

#endif // _CLI_TELNET_H_

