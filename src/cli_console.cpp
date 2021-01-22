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


#include <curses.h>

#include "cli_console.h"


CcliConsole::CcliConsole(void)
  : CcliIODevice("\n")
{
}

CcliConsole::~CcliConsole(void)
{
}

const bool CcliConsole::OpenDevice(void)
{
    // Configure ncurses
    WINDOW* const p_Window = initscr();
    raw();
    noecho();
    scrollok(p_Window, TRUE);
    keypad(p_Window, TRUE);

    return true;
}

const bool CcliConsole::CloseDevice(void)
{
    //sleep(1);
    endwin();

    return true;
}

const CLI_CHAR CcliConsole::GetChar(void) const
{
    while (1)
    {
        const int i_Char = getch();
        switch (i_Char)
        {
        case KEY_UP:            return CLI_KEY_UP;
        case KEY_DOWN:          return CLI_KEY_DOWN;
        case KEY_LEFT:          return CLI_KEY_LEFT;
        case KEY_RIGHT:         return CLI_KEY_RIGHT;
        case KEY_BACKSPACE:     return CLI_BACKSPACE;
        default:
            const CLI_CHAR e_Char = CcliIODevice::GetChar(i_Char);
            if (e_Char != CLI_NULL_KEY)
            {
                return e_Char;
            }
            //  else
            //  {
            //      *this << "Unknown char [" << i_Char << "]" << CLI_ENDL;
            //  }
        }
    }
}

void CcliConsole::PutString(const std::string& STR_Out) const
{
    addstr(STR_Out.c_str());
    refresh();
}

void CcliConsole::Beep(void) const
{
    beep();
}
