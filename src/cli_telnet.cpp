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


#include <sys/types.h> // socket, bind, listen
#include <sys/socket.h> // socket, bind, listen
#include <netinet/in.h> // sockaddr_in
#include <stdio.h> // close
#include <iostream>

#include "cli_telnet.h"
#include "cli_shell.h"


#ifndef CLI_TELNET_MULTI_CLIENT
    #define CLI_TELNET_MULTI_CLIENT 0
#endif


CcliTelnetServer::CcliTelnetServer(CcliShell& CLI_Shell, const unsigned long UL_TcpPort)
  : m_cliShell(CLI_Shell),
    m_ulTcpPort(UL_TcpPort)
{
}

CcliTelnetServer::~CcliTelnetServer(void)
{
}

void CcliTelnetServer::StartServer(void)
{
    const int i_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (i_ServerSocket < 0)
    {
        m_cliShell.GetErrorStream() << "socket failed" << CLI_ENDL;
        return;
    }

    struct sockaddr_in s_SockAddr;
    s_SockAddr.sin_family = AF_INET;
    s_SockAddr.sin_port = htons((unsigned short) m_ulTcpPort);
    s_SockAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(i_ServerSocket, (struct sockaddr*) & s_SockAddr, sizeof(s_SockAddr)) < 0)
    {
        close(i_ServerSocket);
        m_cliShell.GetErrorStream() << "bind failed" << CLI_ENDL;
        return;
    }

    if (listen(i_ServerSocket, 5) < 0)
    {
        close(i_ServerSocket);
        m_cliShell.GetErrorStream() << "listen failed" << CLI_ENDL;
        return;
    }

    while (1)
    {
        std::cout << "Waiting for clients..." << std::endl;
        const int i_ConnectionSocket = accept(i_ServerSocket, NULL, NULL);

        if (i_ConnectionSocket < 0)
        {
            close(i_ServerSocket);
            m_cliShell.GetErrorStream() << "accept failed" << CLI_ENDL;
            return;
        }

        std::cout << "Accepting telnet connection" << std::endl;
        if (! AcceptConnection(i_ConnectionSocket))
        {
            return;
        }
    }
}

const bool CcliTelnetServer::AcceptConnection(const int I_ConnectionSocket)
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
        CcliTelnetConnection* const pcli_Connection = new CcliTelnetConnection(m_cliShell, I_ConnectionSocket);
        m_cliShell.Run(*pcli_Connection);
        std::cout << "Connection " << getpid() << " done" << std::endl;
#if CLI_TELNET_MULTI_CLIENT
        exit(0);
#endif
        return true;
    }
}


CcliTelnetConnection::CcliTelnetConnection(CcliShell& CLI_Shell, const int I_ConnectionSocket)
  : CcliIODevice("\r\n"),
    m_cliShell(CLI_Shell),
    m_iSocket(I_ConnectionSocket)
{
}

CcliTelnetConnection::~CcliTelnetConnection(void)
{
}

const bool CcliTelnetConnection::OpenDevice(void)
{
    return true;
}

const bool CcliTelnetConnection::CloseDevice(void)
{
#if CLI_TELNET_MULTI_CLIENT
    close(m_iSocket);
#endif
    delete this;
}

const CLI_CHAR CcliTelnetConnection::GetChar(void) const
{
    if (! m_qiChars.empty())
    {
        // Dequeue the received characters
        const int i_Front = m_qiChars.front();
        m_qiChars.pop_front();
        const CLI_CHAR e_Char = CcliIODevice::GetChar(i_Front);
        if (e_Char != CLI_NULL_KEY)
        {
            return e_Char;
        }
        else
        {
            return GetChar();
        }
    }
    else
    {
        // Wait for characters
        char str_Buffer[256];
        const int i_Len = recv(m_iSocket, str_Buffer, 256, 0);
        if (i_Len < 0)
        {
            m_cliShell.GetErrorStream() << "recv failed" << CLI_ENDL;
            return CLI_NULL_KEY;
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
            return GetChar();
        }
    }
}

void CcliTelnetConnection::PutString(const std::string& STR_Out) const
{
    const int i_Len = send(m_iSocket, STR_Out.c_str(), STR_Out.size(), 0);
    if (i_Len < 0)
    {
        m_cliShell.GetErrorStream() << "send failed" << CLI_ENDL;
    }
    else if (i_Len < STR_Out.size())
    {
        m_cliShell.GetErrorStream()
            << "send uncomplete"
            << " (only " << i_Len << " characters sent"
            << " over " << STR_Out.size() << ")" << CLI_ENDL;
    }
    else if (i_Len > STR_Out.size())
    {
        m_cliShell.GetErrorStream()
            << "weird send return value"
            << " (" << i_Len << " for " << STR_Out.size() << " characters sent)" << CLI_ENDL;
    }
}

void CcliTelnetConnection::Beep(void) const
{
}
