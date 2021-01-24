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

#include <conio.h>

#include "cli/console.h"
#include "cli/traces.h"

CLI_NS_USE(cli)


//! @brief Windows console trace class singleton redirection.
#define CLI_WIN_CONSOLE GetWinConsoleTraceClass()
//! @brief Windows console trace class singleton.
static const TraceClass& GetWinConsoleTraceClass(void)
{
    static const TraceClass cli_WinConsoleTraceClass("CLI_WIN_CONSOLE", Help()
        .AddHelp(Help::LANG_EN, "CLI Windows console traces")
        .AddHelp(Help::LANG_FR, "Traces de la console Windows du CLI"));
    return cli_WinConsoleTraceClass;
}


Console::Console(const bool B_AutoDelete)
  : IODevice("win-console", "\n", B_AutoDelete),
    m_pData(NULL) // unused
{
}

Console::~Console(void)
{
    Console::CloseDevice();
}

const bool Console::OpenDevice(void)
{
    return true;
}

const bool Console::CloseDevice(void)
{
    return true;
}

const KEY Console::GetKey(void) const
{
    while (1)
    {
        const int i_Char = getch();
        GetTraces().Trace(CLI_WIN_CONSOLE) << "i_Char = " << i_Char << endl;
        switch (i_Char)
        {
            case 8:
            {
                return BACKSPACE;
            }
            case 224: // Escaped characters.
            {
                const int i_Char2 = getch();
                GetTraces().Trace(CLI_WIN_CONSOLE) << "i_Char2 = " << i_Char2 << endl;
                switch (i_Char2)
                {
                    case 71:    return KEY_BEGIN;
                    case 72:    return KEY_UP;
                    case 73:    return PAGE_UP;
                    case 75:    return KEY_LEFT;
                    case 77:    return KEY_RIGHT;
                    case 79:    return KEY_END;
                    case 80:    return KEY_DOWN;
                    case 81:    return PAGE_DOWN;
                    //case 83:    return DELETE;
                }
                break;
            }
            default:
            {
                const KEY e_Char = IODevice::GetKey(i_Char);
                if (e_Char != NULL_KEY)
                {
                    return e_Char;
                }
            }
        }
    }
}

void Console::PutString(const char* const STR_Out) const
{
    _cputs(STR_Out);
}

void Console::Beep(void) const
{
}
