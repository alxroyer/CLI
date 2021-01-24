/*
    Copyright (c) 2006-2010, Alexis Royer, http://alexis.royer.free.fr/CLI

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
    #include <errno.h> // errno
    #define socket_errno errno
    #include <sys/types.h> // socket, bind, listen
    #include <sys/socket.h> // socket, bind, listen
    #include <netinet/in.h> // sockaddr_in
    #include <unistd.h> // getpid
#else
    #include <winsock2.h> // Windows sockets
    #define socket_errno WSAGetLastError()
    #define socklen_t int
    #define EADDRINUSE WSAEADDRINUSE
    #define SHUT_RD SD_RECEIVE
    #define SHUT_WR SD_SEND
    #define SHUT_RDWR SD_BOTH
    #define close closesocket
    #define sleep(seconds) Sleep((seconds)*1000)
    // Disable disturbing macros.
    #undef DELETE
#endif
#include <stdio.h> // close
#include <string.h>

#include "cli/telnet.h"
#include "cli/shell.h"
#include "cli/traces.h"

CLI_NS_USE(cli)


//! @brief Telnet input buffer size.
#ifndef CLI_TELNET_INPUT_BUFFER_SIZE
    #define CLI_TELNET_INPUT_BUFFER_SIZE 1024
#endif

//! @brief Telnet server trace class singleton redirection.
#define CLI_TELNET_SERVER GetTelnetServerTraceClass()
static const TraceClass& GetTelnetServerTraceClass(void)
{
    static const TraceClass cli_TelnetServerTraceClass("CLI_TELNET_SERVER", Help()
        .AddHelp(Help::LANG_EN, "CLI telnet server traces")
        .AddHelp(Help::LANG_FR, "Traces de serveur telnet CLI"));
    return cli_TelnetServerTraceClass;
}
//! @brief Telnet connection trace class singleton redirection.
#define CLI_TELNET_IN GetTelnetInTraceClass()
//! @brief Telnet connection trace class singleton.
static const TraceClass& GetTelnetInTraceClass(void)
{
    static const TraceClass cli_TelnetInTraceClass("CLI_TELNET_IN", Help()
        .AddHelp(Help::LANG_EN, "CLI telnet connection input traces")
        .AddHelp(Help::LANG_FR, "Traces d'entrées d'une connexion telnet CLI"));
    return cli_TelnetInTraceClass;
}
//! @brief Telnet connection trace class singleton redirection.
#define CLI_TELNET_OUT GetTelnetOutTraceClass()
//! @brief Telnet connection trace class singleton.
static const TraceClass& GetTelnetOutTraceClass(void)
{
    static const TraceClass cli_TelnetOutTraceClass("CLI_TELNET_OUT", Help()
        .AddHelp(Help::LANG_EN, "CLI telnet connection output traces")
        .AddHelp(Help::LANG_FR, "Traces de sorties d'une connexion telnet CLI"));
    return cli_TelnetOutTraceClass;
}


TelnetServer::TelnetServer(Shell& CLI_Shell, const unsigned long UL_TcpPort)
  : m_cliShell(CLI_Shell),
    m_ulTcpPort(UL_TcpPort)
{
    GetTraces().Declare(CLI_TELNET_SERVER);
    GetTraces().Declare(CLI_TELNET_IN);
    GetTraces().Declare(CLI_TELNET_OUT);
}

TelnetServer::~TelnetServer(void)
{
}

void TelnetServer::StartServer(void)
{
    GetTraces().Trace(CLI_TELNET_SERVER) << "Starting server on port " << m_ulTcpPort << endl;

    const int i_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
#ifdef CLI_WIN_NETWORK
    // Winsock needs to be initialized.
    if (i_ServerSocket < 0)
    {
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        int err = WSAStartup(wVersionRequested, & wsaData);
        if (err == 0)
        {
            ((int&) i_ServerSocket) = socket(AF_INET, SOCK_STREAM, 0);
        }
    }
#endif
    if (i_ServerSocket < 0)
    {
        const ResourceString cli_Error = ResourceString()
            .SetString(ResourceString::LANG_EN, "socket() failed")
            .SetString(ResourceString::LANG_FR, "Echec de la fonction socket()");
        OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
        GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
        return;
    }
    GetTraces().Trace(CLI_TELNET_SERVER) << "socket() successful (i_ServerSocket = " << i_ServerSocket << ")" << endl;

    // So that we can re-bind to it without TIME_WAIT problems
    int i_ReuseAddr = 1;
    if (setsockopt(i_ServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*) & i_ReuseAddr, sizeof(i_ReuseAddr)) != 0)
    {
        close(i_ServerSocket);
        const ResourceString cli_Error = ResourceString()
            .SetString(ResourceString::LANG_EN, "setsockopt() failed")
            .SetString(ResourceString::LANG_FR, "Echec de la fonction setsockopt()");
        OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
        GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
        return;
    }

    // Bind the socket
    struct sockaddr_in s_SockAddr;
    s_SockAddr.sin_family = AF_INET;
    s_SockAddr.sin_port = htons((unsigned short) m_ulTcpPort);
    s_SockAddr.sin_addr.s_addr = INADDR_ANY;

    int i_BindTries = 0;
    while (bind(i_ServerSocket, (struct sockaddr*) & s_SockAddr, sizeof(s_SockAddr)) < 0)
    {
        i_BindTries ++;
        if ((socket_errno == EADDRINUSE) && (i_BindTries < 2))
        {
            sleep(500);
        }
        else
        {
            close(i_ServerSocket);
            const ResourceString cli_Error = ResourceString()
                .SetString(ResourceString::LANG_EN, "bind() failed")
                .SetString(ResourceString::LANG_FR, "Echec de la fonction bind()");
            OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
            GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
            return;
        }
    }
    GetTraces().Trace(CLI_TELNET_SERVER) << "bind() successful" << endl;

    // Make the server listen up to 5 clients
    if (listen(i_ServerSocket, 5) < 0)
    {
        close(i_ServerSocket);
        const ResourceString cli_Error = ResourceString()
            .SetString(ResourceString::LANG_EN, "listen() failed")
            .SetString(ResourceString::LANG_FR, "Echec de la fonction listen()");
        OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
        GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
        return;
    }
    GetTraces().Trace(CLI_TELNET_SERVER) << "listen() successful" << endl;

    while (1)
    {
        // Wait for a client
        GetTraces().Trace(CLI_TELNET_SERVER) << "Waiting for clients..." << endl;
        const int i_ConnectionSocket = accept(i_ServerSocket, NULL, NULL);
        if (i_ConnectionSocket < 0)
        {
            close(i_ServerSocket);
            const ResourceString cli_Error = ResourceString()
                .SetString(ResourceString::LANG_EN, "accept() failed")
                .SetString(ResourceString::LANG_FR, "Echec de la fonction accept()");
            OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
            GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
            return;
        }
        GetTraces().Trace(CLI_TELNET_SERVER) << "accept() successful (i_ConnectionSocket = " << i_ConnectionSocket << ")" << endl;

        // Accept the client
        const ResourceString cli_InfoClient = ResourceString()
            .SetString(ResourceString::LANG_EN, "Accepting telnet connection")
            .SetString(ResourceString::LANG_FR, "Connexion telnet acceptée");
        GetTraces().Trace(CLI_TELNET_SERVER) << cli_InfoClient.GetString(m_cliShell.GetLang()) << endl;
        if (! AcceptConnection(i_ConnectionSocket))
        {
            return;
        }
    }

    // Release server resources
    if (close(i_ServerSocket) != 0)
    {
        close(i_ServerSocket);
        const ResourceString cli_Error = ResourceString()
            .SetString(ResourceString::LANG_EN, "close() failed")
            .SetString(ResourceString::LANG_FR, "Echec de la fonction close()");
        OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
        GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
        return;
    }
    GetTraces().Trace(CLI_TELNET_SERVER) << "close(" << i_ServerSocket << ") successful" << endl;

    GetTraces().Trace(CLI_TELNET_SERVER) << "Server is done." << endl;
}

const bool TelnetServer::AcceptConnection(const int I_ConnectionSocket)
{
    int i_Pid = 0;

#ifdef CLI_TELNET_MULTI_CLIENT
    #ifndef CLI_WIN_NETWORK
    if (i_Pid = fork())
    #else
#error CLI_TELNET_MULTI_CLIENT / CLI_WIN_NETWORK incompatibility. Do not use CLI_TELNET_MULTI_CLIENT when compiling for non POSIX operating systems.
    #endif
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
        GetTraces().Trace(CLI_TELNET_SERVER) << "Execution started (pid = " << i_Pid << ")" << endl;

        // Configure LINGER option
        linger t_Linger;
        t_Linger.l_onoff = 1; // enable LINGER
        t_Linger.l_linger = 1; // seconds
        if (setsockopt(I_ConnectionSocket, SOL_SOCKET, SO_LINGER, (const char*) & t_Linger, sizeof(t_Linger)) != 0)
        {
            close(I_ConnectionSocket);
            const ResourceString cli_Error = ResourceString()
                .SetString(ResourceString::LANG_EN, "setsockopt(SO_LINGER) failed")
                .SetString(ResourceString::LANG_FR, "Echec de la fonction setsockopt(SO_LINGER)");
            OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
            GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
            return false;
        }
        GetTraces().Trace(CLI_TELNET_SERVER) << "setsockopt(" << I_ConnectionSocket << ", SO_LINGER) successful" << endl;

        if (TelnetConnection* const pcli_Connection = new TelnetConnection(m_cliShell, I_ConnectionSocket, true))
        {
            pcli_Connection->UseInstance(__CALL_INFO__);
            m_cliShell.Run(*pcli_Connection);
            pcli_Connection->FreeInstance(__CALL_INFO__);
        }
        GetTraces().Trace(CLI_TELNET_SERVER) << "Execution done (pid = " << i_Pid << ")" << endl;

#ifdef CLI_TELNET_MULTI_CLIENT
        exit(0);
#else
        // Did not manage to correctly end up the connection on the server side,
        // neither on Windows nor Linux systems.
        // This a hack until I provide another implementation.
        {
            linger t_Linger; socklen_t i_Len = sizeof(t_Linger);
            if (getsockopt(I_ConnectionSocket, SOL_SOCKET, SO_LINGER, (char*) & t_Linger, & i_Len) != 0)
            {
                close(I_ConnectionSocket);
                const ResourceString cli_Error = ResourceString()
                    .SetString(ResourceString::LANG_EN, "getsockopt(SO_LINGER) failed")
                    .SetString(ResourceString::LANG_FR, "Echec de la fonction getsockopt(SO_LINGER)");
                OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
                GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
                return false;
            }

            if (t_Linger.l_onoff)
            {
                sleep(t_Linger.l_linger);
            }
        }

        // Stop sending characters
        if (shutdown(I_ConnectionSocket, SHUT_WR) != 0)
        {
            close(I_ConnectionSocket);
            const ResourceString cli_Error = ResourceString()
                .SetString(ResourceString::LANG_EN, "shutdown() failed")
                .SetString(ResourceString::LANG_FR, "Echec de la fonction shutdown()");
            OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
            GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
            return false;
        }
        GetTraces().Trace(CLI_TELNET_SERVER) << "shutdown(" << I_ConnectionSocket << ") successful" << endl;

        // Release resources
        if (close(I_ConnectionSocket) != 0)
        {
            close(I_ConnectionSocket);
            const ResourceString cli_Error = ResourceString()
                .SetString(ResourceString::LANG_EN, "close() failed")
                .SetString(ResourceString::LANG_FR, "Echec de la fonction close()");
            OutputDevice::GetStdErr() << cli_Error.GetString(m_cliShell.GetLang()) << endl;
            GetTraces().Trace(CLI_TELNET_SERVER) << "errno = " << socket_errno << endl;
            return false;
        }
        GetTraces().Trace(CLI_TELNET_SERVER) << "close(" << I_ConnectionSocket << ") successful" << endl;
#endif

        return true;
    }
}


TelnetConnection::TelnetConnection(
        Shell& CLI_Shell,
        const int I_ConnectionSocket,
        const bool B_AutoDelete)
  : IODevice("telnet", B_AutoDelete),
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
    return true;
}

const KEY TelnetConnection::GetKey(void) const
{
    // Wait for characters if needed
    bool b_ReceiveCharacters = false;
    if (m_qChars.IsEmpty())
    {
        GetTraces().Trace(CLI_TELNET_IN) << "m_qChars.GetCount() = " << m_qChars.GetCount() << endl;

        b_ReceiveCharacters = true;
    }
    else if (m_qChars.GetCount() < 10)
    {
        GetTraces().Trace(CLI_TELNET_IN) << "m_qChars.GetCount() = " << m_qChars.GetCount() << endl;

        // Just peek the socket to read if there are characters remaining
        fd_set fd_read; FD_ZERO(& fd_read); FD_SET(m_iSocket, & fd_read);
        timeval t_Timeout; t_Timeout.tv_sec = 0; t_Timeout.tv_usec = 10000;
        const int i_Select = select(FD_SETSIZE, & fd_read, NULL, NULL, & t_Timeout);
        GetTraces().Trace(CLI_TELNET_IN) << "select() returned " << i_Select << endl;
        GetTraces().Trace(CLI_TELNET_IN) << "FD_ISSET(m_iSocket, & fd_read) = " << FD_ISSET(m_iSocket, & fd_read) << endl;
        if ((i_Select > 0) && FD_ISSET(m_iSocket, & fd_read))
        {
            b_ReceiveCharacters = true;
        }
        if (i_Select < 0)
        {
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "Telnet reception error")
                .SetString(ResourceString::LANG_FR, "Echec de réception en telnet");
            GetTraces().Trace(CLI_TELNET_SERVER) << "select() failed, errno = " << socket_errno << endl;
            return NULL_KEY;
        }
    }
    if (b_ReceiveCharacters)
    {
        // Dequeue characters from the socket
        GetTraces().Trace(CLI_TELNET_IN) << "calling recv()..." << endl;
        char str_Buffer[256];
        const int i_Len = recv(m_iSocket, str_Buffer, sizeof(str_Buffer), 0);
        GetTraces().Trace(CLI_TELNET_IN) << "recv() returned " << i_Len << endl;
        if (i_Len <= 0)
        {
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "Telnet reception error")
                .SetString(ResourceString::LANG_FR, "Echec de réception en telnet");
            GetTraces().Trace(CLI_TELNET_SERVER) << "recv() failed, errno = " << socket_errno << endl;
            return NULL_KEY;
        }

        // Queue them in the internal fifo
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
    }

    // Process characters
    if (! m_qChars.IsEmpty())
    {
        // Dequeue the received characters
        const int i_Front = m_qChars.RemoveHead();
        GetTraces().Trace(CLI_TELNET_IN) << "i_Char = " << (char) i_Front << " (" << i_Front << ")" << endl;

        // Special character management.
        switch (i_Front)
        {
        case 1:     return KEY_BEGIN;   // CTRL+A
        case 3:     return BREAK;       // CTRL+C
        case 4:     return LOGOUT;      // CTRL+D
        case 5:     return KEY_END;     // CTRL+E
        case 8:     return BACKSPACE;   // CTRL+H
        case 13:
            if (! m_qChars.IsEmpty())
            {
                GetTraces().Trace(CLI_TELNET_IN) << "i_Char2 = " << (char) m_qChars.GetHead() << " (" << m_qChars.GetHead() << ")" << endl;
                if (m_qChars.GetHead() == 10)
                {
                    // Process only the character 13 when the sequence is 13-10.
                    m_qChars.RemoveHead();
                    return ENTER;
                }
            }
            break;
        case 12:    return CLS;         // CTRL+L
        case 14:    return NEXT;        // CTRL+N
        case 16:    return PREVIOUS;    // CTRL+P
        case 25:    return REDO;        // CTRL+Y
        case 26:    return UNDO;        // CTRL+Z
        case 27:
            if (! m_qChars.IsEmpty())
            {
                GetTraces().Trace(CLI_TELNET_IN) << "i_Char2 = " << (char) m_qChars.GetHead() << " (" << m_qChars.GetHead() << ")" << endl;
                if (m_qChars.GetHead() == 91)
                {
                    m_qChars.RemoveHead();

                    if (! m_qChars.IsEmpty())
                    {
                        const int i_Special = m_qChars.RemoveHead();
                        GetTraces().Trace(CLI_TELNET_IN) << "i_Char3 = " << (char) i_Special << " (" << i_Special << ")" << endl;

                        switch (i_Special)
                        {
                        // All characters that will be followed by a 126 character.
                        case 49: // INSERT
                        case 50: // KEY_BEGIN;
                        case 51: // PAGE_UP;
                        case 52: // DELETE;
                        case 53: // KEY_END;
                        case 54: // PAGE_DOWN;
                            if (! m_qChars.IsEmpty())
                            {
                                const int i_Finish = m_qChars.RemoveHead();
                                GetTraces().Trace(CLI_TELNET_IN) << "i_Char4 = " << (char) i_Finish << " (" << i_Finish << ")" << endl;

                                if (i_Finish == 126)
                                {
                                    switch (i_Special)
                                    {
                                    case 49: return INSERT;
                                    case 50: return KEY_BEGIN;
                                    case 51: return PAGE_UP;
                                    case 52: return DELETE;
                                    case 53: return KEY_END;
                                    case 54: return PAGE_DOWN;
                                    }
                                }
                                else if ((i_Special == 49) && (! m_qChars.IsEmpty()))
                                {
                                    // Function keys.
                                    const int i_Function = i_Finish;
                                    const int i_FunctionFinish = m_qChars.RemoveHead();
                                    GetTraces().Trace(CLI_TELNET_IN) << "i_Char5 = " << (char) i_FunctionFinish << " (" << i_FunctionFinish << ")" << endl;

                                    if (i_FunctionFinish == 126)
                                    {
                                        switch (i_Function)
                                        {
                                        case 49: return F1;
                                        case 50: return F2;
                                        case 51: return F3;
                                        case 52: return F4;
                                        case 53: return F5;
                                        case 55: return F6;
                                        case 56: return F7;
                                        case 57: return F8;
                                        }
                                    }

                                    // Unhandled.
                                    if (! m_qChars.AddHead(i_FunctionFinish))
                                    {
                                        GetTraces().Trace(INTERNAL_ERROR)
                                            << "TelnetConnection::GetKey(): "
                                            << "Unable to restore previously removed character from m_qChars"
                                            << endl;
                                    }
                                }
                                else if ((i_Special == 50) && (! m_qChars.IsEmpty()))
                                {
                                    // Function keys.
                                    const int i_Function = i_Finish;
                                    const int i_FunctionFinish = m_qChars.RemoveHead();
                                    GetTraces().Trace(CLI_TELNET_IN) << "i_Char5 = " << (char) i_FunctionFinish << " (" << i_FunctionFinish << ")" << endl;

                                    if (i_FunctionFinish == 126)
                                    {
                                        switch (i_Function)
                                        {
                                        case 48: return F9;
                                        case 49: return F10;
                                        case 51: return F11;
                                        case 52: return F12;
                                        }
                                    }

                                    // Unhandled.
                                    if (! m_qChars.AddHead(i_FunctionFinish))
                                    {
                                        GetTraces().Trace(INTERNAL_ERROR)
                                            << "TelnetConnection::GetKey(): "
                                            << "Unable to restore previously removed character from m_qChars"
                                            << endl;
                                    }
                                }

                                // Unhandled.
                                if (! m_qChars.AddHead(i_Finish))
                                {
                                    GetTraces().Trace(INTERNAL_ERROR)
                                        << "TelnetConnection::GetKey(): "
                                        << "Unable to restore previously removed character from m_qChars"
                                        << endl;
                                }
                            }
                        case 65: return KEY_UP;
                        case 66: return KEY_DOWN;
                        case 67: return KEY_RIGHT;
                        case 68: return KEY_LEFT;
                        }

                        // Unhandled.
                        if (! m_qChars.AddHead(i_Special))
                        {
                            GetTraces().Trace(INTERNAL_ERROR)
                                << "TelnetConnection::GetKey(): "
                                << "Unable to restore previously removed character from m_qChars"
                                << endl;
                        }
                    }

                    // Unhandled.
                    if (! m_qChars.AddHead(91))
                    {
                        GetTraces().Trace(INTERNAL_ERROR)
                            << "TelnetConnection::GetKey(): "
                            << "Unable to restore previously removed character from m_qChars"
                            << endl;
                    }
                }
            }
            else
            {
                return ESCAPE;
            }
            break;
        case 96:    return BACK_QUOTE;
        case 126:   return TILDE;
        case 127:   return DELETE;

        // Accentuated characters.
        case -31:   return KEY_aacute;
        case -32:   return KEY_agrave;
        case -28:   return KEY_auml;
        case -30:   return KEY_acirc;
        case -25:   return KEY_ccedil;
        case -23:   return KEY_eacute;
        case -24:   return KEY_egrave;
        case -21:   return KEY_euml;
        case -22:   return KEY_ecirc;
        case -19:   return KEY_iacute;
        case -20:   return KEY_igrave;
        case -17:   return KEY_iuml;
        case -18:   return KEY_icirc;
        case -13:   return KEY_oacute;
        case -14:   return KEY_ograve;
        case -10:   return KEY_ouml;
        case -12:   return KEY_ocirc;
        case -6:    return KEY_uacute;
        case -7:    return KEY_ugrave;
        case -4:    return KEY_uuml;
        case -5:    return KEY_ucirc;
        // Accentuated characters copied from the output itself.
        case -96:   return KEY_aacute;
        case -123:  return KEY_agrave;
        case -124:  return KEY_auml;
        case -125:  return KEY_acirc;
        case -121:  return KEY_ccedil;
        case -126:  return KEY_eacute;
        case -118:  return KEY_egrave;
        case -119:  return KEY_euml;
        case -120:  return KEY_ecirc;
        case -95:   return KEY_iacute;
        case -115:  return KEY_igrave;
        case -117:  return KEY_iuml;
        case -116:  return KEY_icirc;
        case -94:   return KEY_oacute;
        case -107:  return KEY_ograve;
        case -108:  return KEY_ouml;
        case -109:  return KEY_ocirc;
        //case -93:   return KEY_uacute;    // Conflicts with POUND!
        case -105:  return KEY_ugrave;
        case -127:  return KEY_uuml;
        case -106:  return KEY_ucirc;

        // Special characters.
        case -75:   return MICRO;
        case -26:   return MICRO;       // Copied from the output itself.
        case -78:   return SQUARE;
        case -3:    return SQUARE;      // Copied from the output itself.
        case -80:   return DEGREE;
        case -8:    return DEGREE;      // Copied from the output itself.
        case -87:   return COPYRIGHT;
        case -72:   return COPYRIGHT;   // Copied from the output itself.
        case -89:   return PARAGRAPH;
        case -11:   return PARAGRAPH;   // Copied from the output itself.
        case -93:   return POUND;
        case -100:  return POUND;       // Copied from the output itself.
        case -128:  return EURO;
        case -79:   return EURO;    // Copied from the output itself.
        }

        // Unhandled
        // Use default behaviour.
        const KEY e_Char = IODevice::GetKey(i_Front);
        if (e_Char != NULL_KEY)
        {
            return e_Char;
        }
        else
        {
            // Unknown character.
            return GetKey();
        }
    }

    return NULL_KEY;
}

void TelnetConnection::PutString(const char* const STR_Out) const
{
    if (STR_Out != NULL)
    {
        const int i_InLen = (int) strlen(STR_Out);
        int i_OutLen = i_InLen;
        for (int i=0; i<i_InLen; i++) {
            if (STR_Out[i] == '\n') {
                i_OutLen ++;
            }
        }
        if (i_OutLen > 0)
        if (char* const str_Out = new char[i_OutLen + 1])
        {
            memset(str_Out, '\0', i_OutLen + 1);

            // Conversions.
            for (int i=0, o=0; i<i_InLen; i++)
            {
                switch (STR_Out[i])
                {
                case KEY_aacute:    str_Out[o++] = (char) -96;  break;
                case KEY_agrave:    str_Out[o++] = (char) -123; break;
                case KEY_auml:      str_Out[o++] = (char) -124; break;
                case KEY_acirc:     str_Out[o++] = (char) -125; break;
                case KEY_ccedil:    str_Out[o++] = (char) -121; break;
                case KEY_eacute:    str_Out[o++] = (char) -126; break;
                case KEY_egrave:    str_Out[o++] = (char) -118; break;
                case KEY_euml:      str_Out[o++] = (char) -119; break;
                case KEY_ecirc:     str_Out[o++] = (char) -120; break;
                case KEY_iacute:    str_Out[o++] = (char) -95;  break;
                case KEY_igrave:    str_Out[o++] = (char) -115; break;
                case KEY_iuml:      str_Out[o++] = (char) -117; break;
                case KEY_icirc:     str_Out[o++] = (char) -116; break;
                case KEY_oacute:    str_Out[o++] = (char) -94;  break;
                case KEY_ograve:    str_Out[o++] = (char) -107; break;
                case KEY_ouml:      str_Out[o++] = (char) -108; break;
                case KEY_ocirc:     str_Out[o++] = (char) -109; break;
                case KEY_uacute:    str_Out[o++] = (char) -93;  break;
                case KEY_ugrave:    str_Out[o++] = (char) -105; break;
                case KEY_uuml:      str_Out[o++] = (char) -127; break;
                case KEY_ucirc:     str_Out[o++] = (char) -106; break;
                case SQUARE:        str_Out[o++] = (char) -3;   break;
                case EURO:          str_Out[o++] = (char) -79;  break;
                case POUND:         str_Out[o++] = (char) -100; break;
                case MICRO:         str_Out[o++] = (char) -26;  break;
                case PARAGRAPH:     str_Out[o++] = (char) -11;  break;
                case DEGREE:        str_Out[o++] = (char) -8;   break;
                case COPYRIGHT:     str_Out[o++] = (char) -72;  break;
                case '\n':          str_Out[o++] = '\r'; str_Out[o++] = '\n'; break;
                default:            str_Out[o++] = STR_Out[i];  break;
                }
            }

            // Send the buffer.
            if (OutputDevice* const pcli_TraceStream = & const_cast<OutputDevice&>(GetTraces().GetStream()))
            {
                for (int r=0; r<i_OutLen; r++)
                {
                    GetTraces().Trace(CLI_TELNET_OUT) << "sending " << (char) str_Out[r] << endl;
                }
                GetTraces().SetStream(*pcli_TraceStream);
            }
            const int i_Len = send(m_iSocket, str_Out, i_OutLen, 0);
            if (i_Len != i_OutLen)
            {
                // Something has failed regarding output
                // Check whether the trace stream is not this output stream.
                // If it is, change to stderr in order to avoid infinite loops.
                if (& GetTraces().GetStream().GetActualDevice() == this)
                {
                    GetTraces().SetStream(GetStdErr());
                    GetTraces().Trace(CLI_TELNET_OUT) << "Trace stream changed to stderr" << endl;
                }

                m_cliLastError
                    .SetString(ResourceString::LANG_EN, "Telnet emission error")
                    .SetString(ResourceString::LANG_FR, "Echec d'émission en telnet");

                if (i_Len < 0)
                {
                    GetTraces().Trace(CLI_TELNET_OUT) << "send failed" << endl;
                }
                else if (i_Len < i_OutLen)
                {
                    GetTraces().Trace(CLI_TELNET_OUT)
                        << "send incomplete"
                        << " (only " << i_Len << " characters sent"
                        << " over " << i_OutLen << ")" << endl;
                }
                else if (i_Len > i_OutLen)
                {
                    GetTraces().Trace(CLI_TELNET_OUT)
                        << "strange send return value"
                        << " (" << i_Len << " for " << i_OutLen << " characters sent)" << endl;
                }
            }

            // Free the local output buffer.
            delete [] str_Out;
        }
    }
}

void TelnetConnection::Beep(void) const
{
}

