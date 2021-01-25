/*
    Copyright (c) 2006-2018, Alexis Royer, http://alexis.royer.free.fr/CLI

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
          and/or other materials provided with the distribution.
        * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
          without specific prior written permission.

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

#ifdef WIN32
// ifdef defined for dependency computation
#include <windows.h>
#include <conio.h>
#undef DELETE
#endif

#include "cli/console.h"
#include "cli/traces.h"
#include "cli/assert.h"

CLI_NS_USE(cli)


//! @brief Windows console trace class singleton redirection.
#define CLI_WIN_CONSOLE_IN GetWinConsoleInTraceClass()
//! @brief Windows console trace class singleton.
static const TraceClass& GetWinConsoleInTraceClass(void)
{
    static const TraceClass cli_WinConsoleInTraceClass("CLI_WIN_CONSOLE_IN", Help()
        .AddHelp(Help::LANG_EN, "CLI Windows console input traces")
        .AddHelp(Help::LANG_FR, "Traces d'entrées de la console Windows du CLI"));
    return cli_WinConsoleInTraceClass;
}
//! @brief Windows console output trace class singleton redirection.
#define CLI_WIN_CONSOLE_OUT GetWinConsoleOutTraceClass()
//! @brief Windows console output trace class singleton.
static const TraceClass& GetWinConsoleOutTraceClass(void)
{
    static const TraceClass cli_WinConsoleOutTraceClass("CLI_WIN_CONSOLE_OUT", Help()
        .AddHelp(Help::LANG_EN, "CLI Windows console output traces")
        .AddHelp(Help::LANG_FR, "Traces de sorties la console Windows du CLI"));
    return cli_WinConsoleOutTraceClass;
}

//! @brief Window console handle retrieval.
//! @return Window console handle.
static const HANDLE GetWinConsoleHandle(void)
{
    return GetStdHandle(STD_OUTPUT_HANDLE);
}


Console::Console(const bool B_AutoDelete)
  : IODevice("win-console", B_AutoDelete),
    m_pData(NULL)
{
    GetTraces().Declare(CLI_WIN_CONSOLE_IN);
    GetTraces().Declare(CLI_WIN_CONSOLE_OUT);
}

Console::~Console(void)
{
    Console::CloseDevice();
}

const bool Console::OpenDevice(void)
{
    // Windows console Code Page management
    // SetConsoleCP() => see https://docs.microsoft.com/en-us/windows/console/setconsolecp
    // SetConsoleOutputCP() => see https://docs.microsoft.com/en-us/windows/console/setconsoleoutputcp
    // Code Page Identifiers => see https://msdn.microsoft.com/library/windows/desktop/dd317756 or https://en.wikipedia.org/wiki/Code_page
    // 850 = Latin-1
    // 858 = Latin-1 with euro symbol
    // 65001 = utf-8
    // 28591 = iso-8859-1

    // Ensure euro symbol management
    SetConsoleCP(858);
    SetConsoleOutputCP(858);
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
        const int i_Char = _getch(); // [contrib: Oleg Smolsky, 2010, based on CLI 2.5]
        GetTraces().SafeTrace(CLI_WIN_CONSOLE_IN, *this) << "i_Char = " << i_Char << endl;
        switch (i_Char)
        {
            case 0: // Escaped characters
            {
                const int i_Char2 = _getch(); // [contrib: Oleg Smolsky, 2010, based on CLI 2.5]
                GetTraces().SafeTrace(CLI_WIN_CONSOLE_IN, *this) << "i_Char2 = " << i_Char2 << endl;
                switch (i_Char2)
                {
                case 17:    return UNDO;        // CTRL+ALT+Z
                case 21:    return REDO;        // CTRL+ALT+Y
                case 45:    return CUT;         // CTRL+ALT+X
                case 46:    return COPY;        // CTRL+ALT+C
                case 47:    return PASTE;       // CTRL+ALT+V
                case 59:    return F1;
                case 60:    return F2;
                case 61:    return F3;
                case 62:    return F4;
                case 63:    return F5;
                case 64:    return F6;
                case 65:    return F7;
                case 66:    return F8;
                case 67:    return F9;
                case 68:    return F10;
                case 121:   return TILDE;
                case 126:   return BACK_QUOTE;
                case 155:   return PREVIOUS;    // ALT+LEFT
                case 157:   return NEXT;        // ALT+RIGHT
                }
                break;
            }
            case 1:     return KEY_BEGIN;   // CTRL+A
            case 3:     return BREAK;       // CTRL+C
            case 4:     return LOGOUT;      // CTRL+D
            case 5:     return KEY_END;     // CTRL+E
            case 8:     return BACKSPACE;   // (CTRL+H)
            case 9:     return TAB;
            case 10:    return ENTER;
            case 12:    return CLS;         // CTRL+L
            case 13:    return ENTER;
            case 14:    return NEXT;        // CTRL+N (additional character sequence)
            case 16:    return PREVIOUS;    // CTRL+P (additional character sequence)
            case 25:    return REDO;        // CTRL+Y
            case 26:    return UNDO;        // CTRL+Z
            case 27:    return ESCAPE;
            //case 33:      => EXCLAMATION
            //case 34:      => DOUBLE_QUOTE
            //case 35:      => SHARP
            //case 36:      => DOLLAR
            //case 37:      => PERCENT
            //case 38:      => AMPERCENT
            //case 39:      => QUOTE
            //case 40:      => OPENING_BRACE
            //case 41:      => CLOSING_BRACE
            //case 42:      => STAR
            //case 43:      => PLUS
            //case 44:      => COMA
            //case 45:      => MINUS
            //case 46:      => DOT
            //case 47:      => SLASH
            //case 48-57:   => KEY_0 - KEY_9
            //case 58:      => COLUMN
            //case 59:      => SEMI_COLUMN
            //case 60:      => LOWER_THAN
            //case 61:      => EQUAL
            //case 62:      => GREATER_THAN
            //case 63:      => QUESTION
            //case 64:      => AROBASE
            //case 65-90:   => KEY_A - KEY_Z
            //case 91:      => OPENING_BRACKET
            //case 92:      => BACKSLASH
            //case 93:      => CLOSING_BRACKET
            //case 95:      => UNDERSCORE
            case 96:    break;              // Additional character returned after '`'
            //case 97-122:  => KEY_a - KEY_z
            //case 123:     => OPENING_CURLY_BRACE
            //case 124:     => PIPE
            //case 125:     => CLOSING_CURLY_BRACE
            case 126:   break;              // Additional character returned after '~'
            case 129:   return KEY_uuml;
            case 130:   return KEY_eacute;
            case 131:   return KEY_acirc;
            case 132:   return KEY_auml;
            case 133:   return KEY_agrave;
            case 135:   return KEY_ccedil;
            case 136:   return KEY_ecirc;
            case 137:   return KEY_euml;
            case 138:   return KEY_egrave;
            case 139:   return KEY_iuml;
            case 140:   return KEY_icirc;
            case 141:   return KEY_igrave;
            case 147:   return KEY_ocirc;
            case 148:   return KEY_ouml;
            case 149:   return KEY_ograve;
            case 150:   return KEY_ucirc;
            case 151:   return KEY_ugrave;
            case 156:   return POUND;
            case 160:   return KEY_aacute;
            case 161:   return KEY_iacute;
            case 162:   return KEY_oacute;
            case 163:   return KEY_uacute;
            case 184:   return COPYRIGHT;   // Does not work actually. To be checked...
            case 213:   return EURO;
            case 224: // Escaped characters.
            {
                const int i_Char2 = _getch(); // [contrib: Oleg Smolsky, 2010, based on CLI 2.5]
                GetTraces().SafeTrace(CLI_WIN_CONSOLE_IN, *this) << "i_Char2 = " << i_Char2 << endl;
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
                    case 82:    return INSERT;
                    case 83:    return DELETE;
                    case 115:   return PAGE_LEFT;   // CTRL+LEFT
                    case 116:   return PAGE_RIGHT;  // CTRL+RIGHT
                    case 133:   return F11;
                    case 134:   return F12;
                    case 141:   return PAGE_UP;     // CTRL+UP
                    case 145:   return PAGE_DOWN;   // CTRL+DOWN
                    case 146:   return COPY;        // CTRL+INS
                    case 147:   return CUT;         // CTRL+SHIFT+DELETE
                }
                break;
            }
            case 230:   return MICRO;
            case 245:   return PARAGRAPH;
            case 248:   return DEGREE;
            case 253:   return SQUARE;
            default:
            {
                const KEY e_Char = IODevice::Char2Key(i_Char);
                if (e_Char == FEED_MORE)
                {
                    // Let's loop again.
                    //return GetKey();
                }
                else if (e_Char != NULL_KEY)
                {
                    return e_Char;
                }
            }
        }
    }
}

void Console::PutString(const char* const STR_Out) const
{
    if (STR_Out != NULL)
    {
        // Windows console works better when unicode string is output.
        // See https://msdn.microsoft.com/en-us/library/windows/desktop/dd319072(v=vs.85).aspx
        int i_RequiredWideCharLen = MultiByteToWideChar(
            CP_UTF8, 0, // CodePage, dwFlags
            STR_Out, -1, // lpMultiByteStr
            NULL, 0 // lpWideCharStr
        );
        if (i_RequiredWideCharLen > 0) {
            LPWSTR str_Unicode = new WCHAR[i_RequiredWideCharLen];
            MultiByteToWideChar(
                CP_UTF8, 0, // CodePage, dwFlags
                STR_Out, -1, // lpMultiByteStr
                str_Unicode, i_RequiredWideCharLen // lpWideCharStr
            );
            _cputws(str_Unicode);
            delete [] str_Unicode;
        }

        // Insure the cursor is visible (reposition in the same place).
        do
        {
            HANDLE h_Console = ::GetWinConsoleHandle();
            CONSOLE_SCREEN_BUFFER_INFO t_ScreenBufferInfo;
            if (! ::GetConsoleScreenBufferInfo(h_Console, & t_ScreenBufferInfo))
            {
                GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "GetConsoleScreenBufferInfo(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
                break;
            }
            if (! ::SetConsoleCursorPosition(h_Console, t_ScreenBufferInfo.dwCursorPosition))
            {
                GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "GetConsoleScreenBufferInfo(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
                break;
            }
        } while(0);
    }
}

void Console::Beep(void) const
{
    IODevice::Beep();
}

void Console::CleanScreen(void) const
{
    do
    {
        // Retrieve the handle of the console (use stdout).
        HANDLE h_Console = ::GetWinConsoleHandle();

        // Get the number of character cells in the current buffer.
        CONSOLE_SCREEN_BUFFER_INFO t_ScreenBufferInfo;
        if (! ::GetConsoleScreenBufferInfo(h_Console, & t_ScreenBufferInfo))
        {
            GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "GetConsoleScreenBufferInfo(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
            break;
        }
        const DWORD dw_ConsoleSize = t_ScreenBufferInfo.dwSize.X * t_ScreenBufferInfo.dwSize.Y;

        // Fill the entire screen with blanks.
        const COORD t_ZeroCoord = { 0, 0 };
        DWORD dw_CharsWritten = 0;
        if (! ::FillConsoleOutputCharacter(h_Console, (TCHAR) ' ', dw_ConsoleSize, t_ZeroCoord, & dw_CharsWritten))
        {
            GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "FillConsoleOutputCharacter(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
            break;
        }

// This part of code seems a little bit strange to me,
// Whatever, it is advised by MSDN. Nevermind then.
        // Get the current text attribute.
        if (! ::GetConsoleScreenBufferInfo(h_Console, & t_ScreenBufferInfo))
        {
            GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "GetConsoleScreenBufferInfo(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
            break;
        }

        // Now set the buffer's attributes accordingly.
        if (! ::FillConsoleOutputAttribute(h_Console, t_ScreenBufferInfo.wAttributes, dw_ConsoleSize, t_ZeroCoord, & dw_CharsWritten))
        {
            GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "FillConsoleOutputAttribute(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
            break;
        }
// End of strange code

        // Put the cursor at (0, 0).
        if (! ::SetConsoleCursorPosition(h_Console, t_ZeroCoord))
        {
            GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "SetConsoleCursorPosition(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
            break;
        }

        // Succesful return.
        return;
    } while(0);

    // Fallback to default behaviour.
    IODevice::CleanScreen();
}

const OutputDevice::ScreenInfo Console::GetScreenInfo(void) const
{
    do
    {
        // Retrieve the handle of the console (use stdout).
        HANDLE h_Console = ::GetWinConsoleHandle();

        // Get the number of character cells in the current buffer.
        CONSOLE_SCREEN_BUFFER_INFO t_ScreenBufferInfo;
        if (! ::GetConsoleScreenBufferInfo(h_Console, & t_ScreenBufferInfo))
        {
            GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "GetConsoleScreenBufferInfo(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
            break;
        }

        DWORD dw_Mode = 0;
        if (! ::GetConsoleMode(h_Console, & dw_Mode))
        {
            GetTraces().SafeTrace(CLI_WIN_CONSOLE_OUT, *this) << "GetConsoleMode(line=" << __LINE__ << ") failed (" << (uint64_t) ::GetLastError() << ")" << endl;
            break;
        }

        // Succesful return.
        return ScreenInfo(
            t_ScreenBufferInfo.srWindow.Right - t_ScreenBufferInfo.srWindow.Left + 1,
            t_ScreenBufferInfo.srWindow.Bottom - t_ScreenBufferInfo.srWindow.Top + 1,
            true,                                       // True Cls (if this line is reached, it means CleanScreen would succeed)
            (dw_Mode & ENABLE_WRAP_AT_EOL_OUTPUT) != 0  // Line wrapping
        );
    } while(0);

    // Fallback to default behaviour.
    return IODevice::GetScreenInfo();
}
