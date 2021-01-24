/*
    Copyright (c) 2006-2009, Alexis Royer, http://alexis.royer.free.fr/CLI

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

#ifdef WIN32
// ifdef defined for dependency computation
#include <conio.h>
#endif

#include "cli/console.h"
#include "cli/io_mux.h"
#include "cli/traces.h"

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



Console::Console(const bool B_AutoDelete)
  : IODevice("win-console", B_AutoDelete),
    m_pData(NULL)
{
    // Does nothing but registers the appropriate trace classes.
    GetTraces().Trace(CLI_WIN_CONSOLE_IN);
    GetTraces().Trace(CLI_WIN_CONSOLE_OUT);
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
        GetTraces().Trace(CLI_WIN_CONSOLE_IN) << "i_Char = " << i_Char << endl;
        switch (i_Char)
        {
            case 0: // Escaped characters
            {
                const int i_Char2 = getch();
                GetTraces().Trace(CLI_WIN_CONSOLE_IN) << "i_Char2 = " << i_Char2 << endl;
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
            case 12:    return CLS;         // CTRL+L
            case 14:    return NEXT;        // CTRL+N
            case 16:    return PREVIOUS;    // CTRL+P
            case 25:    return REDO;        // CTRL+Y
            case 26:    return UNDO;        // CTRL+Z
            case 27:    return ESCAPE;
            //case 63:    return EURO;        // CTRL+ALT+E, conflicts with '?'
            case 96:    break;              // character always returned after `
            case 126:   break;              // character always return after ~
            case 160:   return KEY_aacute;
            case 133:   return KEY_agrave;
            case 132:   return KEY_auml;
            case 131:   return KEY_acirc;
            case 135:   return KEY_ccedil;
            case 130:   return KEY_eacute;
            case 138:   return KEY_egrave;
            case 137:   return KEY_euml;
            case 136:   return KEY_ecirc;
            case 161:   return KEY_iacute;
            case 141:   return KEY_igrave;
            case 139:   return KEY_iuml;
            case 140:   return KEY_icirc;
            case 156:   return POUND;
            case 162:   return KEY_oacute;
            case 149:   return KEY_ograve;
            case 148:   return KEY_ouml;
            case 147:   return KEY_ocirc;
            case 163:   return KEY_uacute;
            case 151:   return KEY_ugrave;
            case 129:   return KEY_uuml;
            case 150:   return KEY_ucirc;
            case 224: // Escaped characters.
            {
                const int i_Char2 = getch();
                GetTraces().Trace(CLI_WIN_CONSOLE_IN) << "i_Char2 = " << i_Char2 << endl;
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
                }
                break;
            }
            case 230:   return MICRO;
            case 245:   return PARAGRAPH;
            case 248:   return DEGREE;
            case 184:   return COPYRIGHT;
            case 253:   return SQUARE;
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
    if (STR_Out != NULL)
    {
        const unsigned int ui_Len = (unsigned int) strlen(STR_Out);
        for (unsigned int ui=0; ui<ui_Len; ui++)
        {
            char str_Char[2] = { STR_Out[ui], '\0' };
            switch (str_Char[0])
            {
                case KEY_aacute:    str_Char[0] = (char) -96;   break;
                case KEY_agrave:    str_Char[0] = (char) -123;  break;
                case KEY_auml:      str_Char[0] = (char) -124;  break;
                case KEY_acirc:     str_Char[0] = (char) -125;  break;
                case KEY_ccedil:    str_Char[0] = (char) -121;  break;
                case KEY_eacute:    str_Char[0] = (char) -126;  break;
                case KEY_egrave:    str_Char[0] = (char) -118;  break;
                case KEY_euml:      str_Char[0] = (char) -119;  break;
                case KEY_ecirc:     str_Char[0] = (char) -120;  break;
                case KEY_iacute:    str_Char[0] = (char) -95;   break;
                case KEY_igrave:    str_Char[0] = (char) -115;  break;
                case KEY_iuml:      str_Char[0] = (char) -117;  break;
                case KEY_icirc:     str_Char[0] = (char) -116;  break;
                case KEY_oacute:    str_Char[0] = (char) -94;   break;
                case KEY_ograve:    str_Char[0] = (char) -107;  break;
                case KEY_ouml:      str_Char[0] = (char) -108;  break;
                case KEY_ocirc:     str_Char[0] = (char) -109;  break;
                case KEY_uacute:    str_Char[0] = (char) -93;   break;
                case KEY_ugrave:    str_Char[0] = (char) -105;  break;
                case KEY_uuml:      str_Char[0] = (char) -127;  break;
                case KEY_ucirc:     str_Char[0] = (char) -106;  break;
                case SQUARE:        str_Char[0] = (char) -3;    break;
                case EURO:          str_Char[0] = (char) -79;   break;
                case POUND:         str_Char[0] = (char) -100;  break;
                case MICRO:         str_Char[0] = (char) -26;   break;
                case PARAGRAPH:     str_Char[0] = (char) -11;   break;
                case DEGREE:        str_Char[0] = (char) -8;    break;
                case COPYRIGHT:     str_Char[0] = (char) -72;   break;
                case -61:
                {
                    // Ecaped characters (from Java for instance).
                    if (++ui<ui_Len)
                    {
                        switch (STR_Out[ui])
                        {
                            case -95:   str_Char[0] = (char) -96;   break;  // KEY_aacute
                            case -96:   str_Char[0] = (char) -123;  break;  // KEY_agrave
                            case -92:   str_Char[0] = (char) -124;  break;  // KEY_auml
                            case -94:   str_Char[0] = (char) -125;  break;  // KEY_acirc
                            case -89:   str_Char[0] = (char) -121;  break;  // KEY_ccedil
                            case -87:   str_Char[0] = (char) -126;  break;  // KEY_eacute
                            case -88:   str_Char[0] = (char) -118;  break;  // KEY_egrave
                            case -85:   str_Char[0] = (char) -119;  break;  // KEY_euml
                            case -86:   str_Char[0] = (char) -120;  break;  // KEY_ecirc
                            case -83:   str_Char[0] = (char) -95;   break;  // KEY_iacute
                            case -84:   str_Char[0] = (char) -115;  break;  // KEY_igrave
                            case -81:   str_Char[0] = (char) -117;  break;  // KEY_iuml
                            case -82:   str_Char[0] = (char) -116;  break;  // KEY_icirc
                            case -77:   str_Char[0] = (char) -94;   break;  // KEY_oacute
                            case -78:   str_Char[0] = (char) -107;  break;  // KEY_ograve
                            case -74:   str_Char[0] = (char) -108;  break;  // KEY_ouml
                            case -76:   str_Char[0] = (char) -109;  break;  // KEY_ocirc
                            case -70:   str_Char[0] = (char) -93;   break;  // KEY_uacute
                            case -71:   str_Char[0] = (char) -105;  break;  // KEY_ugrave
                            case -68:   str_Char[0] = (char) -127;  break;  // KEY_uuml
                            case -69:   str_Char[0] = (char) -106;  break;  // KEY_ucirc
                        }
                    }
                    break;
                }
            }

            _cputs(str_Char);

            // Additional trace.
            if (GetTraces().IsTraceOn(CLI_WIN_CONSOLE_OUT))
            {
                if (& GetTraces().GetStream().GetActualDevice() == this)
                {
                    GetTraces().SetStream(GetStdErr());
                    GetTraces().Trace(CLI_WIN_CONSOLE_OUT) << "Trace stream changed to stderr" << endl;
                }

                GetTraces().Trace(CLI_WIN_CONSOLE_OUT) << "(" << (int) STR_Out[ui] << ")";
            }
        }
    }
}

void Console::Beep(void) const
{
    IODevice::Beep();
}

void Console::CleanScreen(void) const
{
    IODevice::CleanScreen();
}
