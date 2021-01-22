/*
    Copyright (c) 2006-2007, Alexis Royer

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

