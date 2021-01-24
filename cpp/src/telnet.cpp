/*
    Copyright (c) 2006-2008, Alexis Royer

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


#include "cli/pch.h"

#ifndef CLI_WIN_NETWORK
    #include <sys/types.h> // socket, bind, listen
    #include <sys/socket.h> // socket, bind, listen
    #include <netinet/in.h> // sockaddr_in
    #include <unistd.h> // getpid
#else
    #include <winsock2.h> // Windows sockets
    #define close closesocket
#endif
#include <stdio.h> // close
#include <string.h>

#include "cli/telnet.h"
#include "cli/shell.h"
#include "cli/traces.h"

CLI_NS_USE(cli)


#ifndef CLI_TELNET_INPUT_BUFFER_SIZE
    #define CLI_TELNET_INPUT_BUFFER_SIZE 1024
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
        const ResourceString cli_Error = ResourceString()
            .SetString(ResourceString::LANG_EN, "socket() failed")
            .SetString(ResourceString::LANG_FR, "Echec de la fonction socket()");
        m_cliShell.GetStream(ERROR_STREAM) << cli_Error.GetString(m_cliShell.GetLang()) << endl;
        return;
    }

    struct sockaddr_in s_SockAddr;
    s_SockAddr.sin_family = AF_INET;
    s_SockAddr.sin_port = htons((unsigned short) m_ulTcpPort);
    s_SockAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(i_ServerSocket, (struct sockaddr*) & s_SockAddr, sizeof(s_SockAddr)) < 0)
    {
        close(i_ServerSocket);
        const ResourceString cli_Error = ResourceString()
            .SetString(ResourceString::LANG_EN, "bind() failed")
            .SetString(ResourceString::LANG_FR, "Echec de la fonction bind()");
        m_cliShell.GetStream(ERROR_STREAM) << cli_Error.GetString(m_cliShell.GetLang()) << endl;
        return;
    }

    if (listen(i_ServerSocket, 5) < 0)
    {
        close(i_ServerSocket);
        const ResourceString cli_Error = ResourceString()
            .SetString(ResourceString::LANG_EN, "listen() failed")
            .SetString(ResourceString::LANG_FR, "Echec de la fonction listen()");
        m_cliShell.GetStream(ERROR_STREAM) << cli_Error.GetString(m_cliShell.GetLang()) << endl;
        return;
    }

    while (1)
    {
        const ResourceString cli_InfoWaiting = ResourceString()
            .SetString(ResourceString::LANG_EN, "Waiting for clients...")
            .SetString(ResourceString::LANG_FR, "En attente de clients...");
        m_cliShell.GetStream(OUTPUT_STREAM) << cli_InfoWaiting.GetString(m_cliShell.GetLang()) << endl;
        const int i_ConnectionSocket = accept(i_ServerSocket, NULL, NULL);

        if (i_ConnectionSocket < 0)
        {
            close(i_ServerSocket);
            const ResourceString cli_Error = ResourceString()
                .SetString(ResourceString::LANG_EN, "accept() failed")
                .SetString(ResourceString::LANG_FR, "Echec de la fonction accept()");
            m_cliShell.GetStream(ERROR_STREAM) << cli_Error.GetString(m_cliShell.GetLang()) << endl;
            return;
        }

        const ResourceString cli_InfoClient = ResourceString()
            .SetString(ResourceString::LANG_EN, "Accepting telnet connection")
            .SetString(ResourceString::LANG_FR, "Connexion telnet acceptée");
        m_cliShell.GetStream(OUTPUT_STREAM) << cli_InfoClient.GetString(m_cliShell.GetLang()) << endl;
        if (! AcceptConnection(i_ConnectionSocket))
        {
            return;
        }
    }
}

const bool TelnetServer::AcceptConnection(const int I_ConnectionSocket)
{
    int i_Pid = 0;

#ifdef CLI_TELNET_MULTI_CLIENT
    if (i_Pid = fork())
    {
        // Server side.
        return true;
    }
    else
#endif
    {
        // Connection side.
#ifndef CLI_WIN_NETWORK
        i_Pid = getpid();
#else
        i_Pid = GetCurrentThreadId();
#endif
        const ResourceString cli_InfoStart = ResourceString()
            .SetString(ResourceString::LANG_EN, "Connection started")
            .SetString(ResourceString::LANG_FR, "Début de connexion");
        m_cliShell.GetStream(OUTPUT_STREAM)
            << cli_InfoStart.GetString(m_cliShell.GetLang())
            << " " << i_Pid << cli::endl;
        TelnetConnection* const pcli_Connection = new TelnetConnection(m_cliShell, I_ConnectionSocket, true);
        m_cliShell.Run(*pcli_Connection);
        const ResourceString cli_InfoEnd = ResourceString()
            .SetString(ResourceString::LANG_EN, "Connection done")
            .SetString(ResourceString::LANG_FR, "Début de connexion");
        m_cliShell.GetStream(OUTPUT_STREAM)
            << cli_InfoEnd.GetString(m_cliShell.GetLang())
            << " " << i_Pid << cli::endl;
#ifdef CLI_TELNET_MULTI_CLIENT
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
    m_iSocket(I_ConnectionSocket),
    m_qChars(CLI_TELNET_INPUT_BUFFER_SIZE)
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
#ifdef CLI_TELNET_MULTI_CLIENT
    close(m_iSocket);
#endif
    delete this;
    return true;
}

const KEY TelnetConnection::GetKey(void) const
{
    if (! m_qChars.IsEmpty())
    {
        // Dequeue the received characters
        const int i_Front = m_qChars.RemoveHead();

        // Special character management.
        switch (i_Front)
        {
        case 27:
            if (! m_qChars.IsEmpty())
            {
                if ((m_qChars.GetHead() == 91) && (m_qChars.GetCount() >= 2))
                {
                    m_qChars.RemoveHead();
                    const int i_Special = m_qChars.RemoveHead();

                    switch (i_Special)
                    {
                    case 65: return KEY_UP;
                    case 66: return KEY_DOWN;
                    case 67: return KEY_RIGHT;
                    case 68: return KEY_LEFT;
                    default:
                        // Unhandled.
                        if ((! m_qChars.AddHead(i_Special))
                            || (! m_qChars.AddHead(91)))
                        {
                            GetTraces().Trace(INTERNAL_ERROR)
                                << "TelnetConnection::GetKey(): "
                                << "Unable to restore previously removed character from m_qChars"
                                << endl;
                        }
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
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "Telnet reception error")
                .SetString(ResourceString::LANG_FR, "Echec de réception en telnet");
            return NULL_KEY;
        }
        else
        {
            for (int i=0; i<i_Len; i++)
            {
                if (str_Buffer[i] != '\0')
                {
                    if (! m_qChars.AddTail(str_Buffer[i]))
                    {
                        GetTraces().Trace(INTERNAL_ERROR)
                            << "TelnetConnection::GetKey(): "
                            << "Could not append character '" << str_Buffer[i] << "' "
                            << "to m_qChars" << endl;
                    }
                }
            }
            return GetKey();
        }
    }
}

void TelnetConnection::PutString(const char* const STR_Out) const
{
    if (STR_Out != NULL)
    {
        const int i_Len = send(m_iSocket, STR_Out, (int) strlen(STR_Out), 0);
        if (i_Len != (signed) strlen(STR_Out))
        {
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "Telnet emission error")
                .SetString(ResourceString::LANG_FR, "Echec d'émission en telnet");

            if (i_Len < 0)
            {
                GetTraces().Trace(INTERNAL_ERROR) << "send failed" << endl;
            }
            else if ((unsigned) i_Len < strlen(STR_Out))
            {
                GetTraces().Trace(INTERNAL_ERROR)
                    << "send incomplete"
                    << " (only " << i_Len << " characters sent"
                    << " over " << (unsigned int) strlen(STR_Out) << ")" << endl;
            }
            else if ((unsigned) i_Len > strlen(STR_Out))
            {
                GetTraces().Trace(INTERNAL_ERROR)
                    << "strange send return value"
                    << " (" << i_Len << " for " << (unsigned int) strlen(STR_Out) << " characters sent)" << endl;
            }
        }
    }
}

void TelnetConnection::Beep(void) const
{
}

/*const OutputDevice& TelnetConnection::GetErrorStream(void) const
{
    // First try to send error to the device set in the shell.
    if (const OutputDevice* const pcli_ErrorDevice = & m_cliShell.GetStream(ERROR_STREAM))
    {
        if (pcli_ErrorDevice != this)
        {
            // Do not report error to this device which has caused the error.
            return *pcli_ErrorDevice;
        }
    }

    // Default to standard error.
    return OutputDevice::GetStdErr();
}*/
