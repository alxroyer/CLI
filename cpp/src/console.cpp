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


#include "cli/console.h"

//! @warning Include ncurses after cli.
#include <curses.h>

using namespace cli;


Console::Console(const bool B_AutoDelete)
  : IODevice("console", "\n", B_AutoDelete),
    m_pWindow(NULL)
{
}

Console::~Console(void)
{
    Console::CloseDevice();
}

const bool Console::OpenDevice(void)
{
    if (m_pWindow == NULL)
    {
        // Configure ncurses
        WINDOW* const p_Window = initscr();
        raw();
        noecho();
        scrollok(p_Window, TRUE);
        keypad(p_Window, TRUE);

        m_pWindow = p_Window;
    }

    return true;
}

const bool Console::CloseDevice(void)
{
    if (m_pWindow != NULL)
    {
        m_pWindow = NULL;

        //sleep(1);
        endwin();
    }

    return true;
}

const KEY Console::GetKey(void) const
{
    while (1)
    {
        const int i_Char = getch();
        switch (i_Char)
        {
        // ncurses KEY_UP
        case KEY_UP:
            #undef KEY_UP
            return cli::KEY_UP;
        // ncurses KEY_DOWN
        case KEY_DOWN:
            #undef KEY_DOWN
            return cli::KEY_DOWN;
        // ncurses KEY_LEFT
        case KEY_LEFT:
            #undef KEY_LEFT
            return cli::KEY_LEFT;
        // ncurses KEY_RIGHT
        case KEY_RIGHT:
            #undef KEY_RIGHT
            return cli::KEY_RIGHT;
        case KEY_BACKSPACE:
            return cli::BACKSPACE;
        default:
            const KEY e_Char = IODevice::GetKey(i_Char);
            if (e_Char != NULL_KEY)
            {
                return e_Char;
            }
        }
    }
}

void Console::PutString(const std::string& STR_Out) const
{
    addstr(STR_Out.c_str());
    refresh();
}

void Console::Beep(void) const
{
    beep();
}
