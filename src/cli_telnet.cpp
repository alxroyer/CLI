/*
    Copyright 2006 Alexis Royer

    This file is part of the CLI library.

    The CLI library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <sys/types.h> // socket, bind, listen
#include <sys/socket.h> // socket, bind, listen
#include <netinet/in.h> // sockaddr_in
#include <stdio.h> // close
#include <iostream>

#include "cli_telnet.h"
#include "cli_shell.h"


CcliTelnetServer::CcliTelnetServer(CcliShell& CLI_Shell, unsigned long UL_TcpPort)
  : m_cliShell(CLI_Shell)
{
	m_ulTcpPort = UL_TcpPort;
}

CcliTelnetServer::~CcliTelnetServer(void)
{
}

void CcliTelnetServer::StartServer(void)
{
	int i_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (i_ServerSocket < 0)
	{
		m_cliShell.GetErrorStream() << "socket failed" << CLI_ENDL;
		return;
	}

	struct sockaddr_in s_SockAddr;
	s_SockAddr.sin_family = AF_INET;
	s_SockAddr.sin_port = htons((unsigned short) m_ulTcpPort); // 0;  /* System will assign port #  */
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
		int i_ConnectionSocket = accept(i_ServerSocket, NULL, NULL);
		std::cout << "accept returned " << i_ConnectionSocket << std::endl;
		
		if (i_ConnectionSocket < 0)
		{
			close(i_ServerSocket);
			m_cliShell.GetErrorStream() << "accept failed" << CLI_ENDL;
			return;
		}

		std::cout << "accepting telnet connection" << std::endl;
		if (! AcceptConnection(i_ConnectionSocket))
		{
			return;
		}
	}
}

bool CcliTelnetServer::AcceptConnection(int I_ConnectionSocket)
{
	CcliTelnetConnection* pcli_Connection = new CcliTelnetConnection(m_cliShell, I_ConnectionSocket);
	m_cliShell.Run(*pcli_Connection);
}


CcliTelnetConnection::CcliTelnetConnection(CcliShell& CLI_Shell, int I_ConnectionSocket)
  : CcliIODevice("\r\n"),
    m_cliShell(CLI_Shell)
{
	m_iSocket = I_ConnectionSocket;
}

CcliTelnetConnection::~CcliTelnetConnection(void)
{
}

bool CcliTelnetConnection::OpenDevice(void)
{
	return true;
}

bool CcliTelnetConnection::CloseDevice(void)
{
	close(m_iSocket);
	delete this;
}

int CcliTelnetConnection::GetChar(void) const
{
	if (! m_qiChars.empty())
	{
		// Dequeue the received characters
		int i_Front = m_qiChars.front();
		m_qiChars.pop_front();
		return i_Front;
	}
	else
	{
		// Wait for characters
		char str_Buffer[256];
		int i_Len = recv(m_iSocket, str_Buffer, 256, 0);
		if (i_Len < 0)
		{
			m_cliShell.GetErrorStream() << "recv failed" << CLI_ENDL;
			return -1;
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
	int i_Len = send(m_iSocket, STR_Out.c_str(), STR_Out.size(), 0);
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

