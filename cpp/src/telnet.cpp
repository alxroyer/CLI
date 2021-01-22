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


#include <sys/types.h> // socket, bind, listen
#include <sys/socket.h> // socket, bind, listen
#include <netinet/in.h> // sockaddr_in
#include <stdio.h> // close
#include <iostream>

#include "cli/telnet.h"
#include "cli/shell.h"

using namespace cli;


#ifndef CLI_TELNET_MULTI_CLIENT
    #define CLI_TELNET_MULTI_CLIENT 0
#endif


TelnetServer::TelnetServer(Shell& CLI_Shell, const unsigned long UL_TcpPort)
  : m_cliShell(CLI_Shell),
    m_ulTcpPort(UL_TcpPort)
{
}

TelnetServer::~TelnetServer(void)
{
}

void TelnetServer::StartServer(void)
{
    const int i_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (i_ServerSocket < 0)
    {
        m_cliShell.GetStream(ERROR_STREAM) << "socket failed" << endl;
        return;
    }

    struct sockaddr_in s_SockAddr;
    s_SockAddr.sin_family = AF_INET;
    s_SockAddr.sin_port = htons((unsigned short) m_ulTcpPort);
    s_SockAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(i_ServerSocket, (struct sockaddr*) & s_SockAddr, sizeof(s_SockAddr)) < 0)
    {
        close(i_ServerSocket);
        m_cliShell.GetStream(ERROR_STREAM) << "bind failed" << endl;
        return;
    }

    if (listen(i_ServerSocket, 5) < 0)
    {
        close(i_ServerSocket);
        m_cliShell.GetStream(ERROR_STREAM) << "listen failed" << endl;
        return;
    }

    while (1)
    {
        std::cout << "Waiting for clients..." << std::endl;
        const int i_ConnectionSocket = accept(i_ServerSocket, NULL, NULL);

        if (i_ConnectionSocket < 0)
        {
            close(i_ServerSocket);
            m_cliShell.GetStream(ERROR_STREAM) << "accept failed" << endl;
            return;
        }

        std::cout << "Accepting telnet connection" << std::endl;
        if (! AcceptConnection(i_ConnectionSocket))
        {
            return;
        }
    }
}

const bool TelnetServer::AcceptConnection(const int I_ConnectionSocket)
{
#if CLI_TELNET_MULTI_CLIENT
    if (int i_Pid = fork())
    {
        // Server side.
        return true;
    }
    else
#endif
    {
        // Connection side.
        std::cout << "Connection " << getpid() << " started" << std::endl;
        TelnetConnection* const pcli_Connection = new TelnetConnection(m_cliShell, I_ConnectionSocket, true);
        m_cliShell.Run(*pcli_Connection);
        std::cout << "Connection " << getpid() << " done" << std::endl;
#if CLI_TELNET_MULTI_CLIENT
        exit(0);
#endif
        return true;
    }
}


TelnetConnection::TelnetConnection(
        Shell& CLI_Shell,
        const int I_ConnectionSocket,
        const bool B_AutoDelete)
  : IODevice("telnet", "\r\n", B_AutoDelete),
    m_cliShell(CLI_Shell),
    m_iSocket(I_ConnectionSocket)
{
}

TelnetConnection::~TelnetConnection(void)
{
}

const bool TelnetConnection::OpenDevice(void)
{
    return true;
}

const bool TelnetConnection::CloseDevice(void)
{
#if CLI_TELNET_MULTI_CLIENT
    close(m_iSocket);
#endif
    delete this;
    return true;
}

const KEY TelnetConnection::GetKey(void) const
{
    if (! m_qiChars.empty())
    {
        // Dequeue the received characters
        const int i_Front = m_qiChars.front();
        m_qiChars.pop_front();

        // Special character management.
        switch (i_Front)
        {
        case 27:
            if (! m_qiChars.empty())
            {
                if ((m_qiChars.front() == 91) && (m_qiChars.size() >= 2))
                {
                    m_qiChars.pop_front();
                    const int i_Special = m_qiChars.front();
                    m_qiChars.pop_front();

                    switch (i_Special)
                    {
                    case 65: return KEY_UP;
                    case 66: return KEY_DOWN;
                    case 67: return KEY_RIGHT;
                    case 68: return KEY_LEFT;
                    default:
                        // Unhandled.
                        m_qiChars.push_front(i_Special);
                        m_qiChars.push_front(91);
                    }
                }
            }
            break;

        case 8:
            return BACKSPACE;
        }

        const KEY e_Char = IODevice::GetKey(i_Front);
        if (e_Char != NULL_KEY)
        {
            return e_Char;
        }
        else
        {
            return GetKey();
        }
    }
    else
    {
        // Wait for characters
        char str_Buffer[256];
        const int i_Len = recv(m_iSocket, str_Buffer, 256, 0);
        if (i_Len <= 0)
        {
            m_cliShell.GetStream(ERROR_STREAM) << "recv failed" << endl;
            return NULL_KEY;
        }
        else
        {
            for (int i=0; i<i_Len; i++)
            {
                if (str_Buffer[i] != '\0')
                {
                    m_qiChars.push_back(str_Buffer[i]);
                }
            }
            return GetKey();
        }
    }
}

void TelnetConnection::PutString(const std::string& STR_Out) const
{
    const int i_Len = send(m_iSocket, STR_Out.c_str(), STR_Out.size(), 0);
    if (i_Len < 0)
    {
        m_cliShell.GetStream(ERROR_STREAM) << "send failed" << endl;
    }
    else if ((unsigned) i_Len < STR_Out.size())
    {
        m_cliShell.GetStream(ERROR_STREAM)
            << "send uncomplete"
            << " (only " << i_Len << " characters sent"
            << " over " << STR_Out.size() << ")" << endl;
    }
    else if ((unsigned) i_Len > STR_Out.size())
    {
        m_cliShell.GetStream(ERROR_STREAM)
            << "weird send return value"
            << " (" << i_Len << " for " << STR_Out.size() << " characters sent)" << endl;
    }
}

void TelnetConnection::Beep(void) const
{
}
