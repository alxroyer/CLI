/*
    Copyright (c) 2006-2007, Alexis Royer

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

#include "cli/console.h"

//! @warning Include ncurses after cli.
#include <curses.h>

CLI_NS_USE(cli)


Console::Console(const bool B_AutoDelete)
  : IODevice("ncurses-console", "\n", B_AutoDelete),
    m_pData(NULL)
{
}

Console::~Console(void)
{
    Console::CloseDevice();
}

const bool Console::OpenDevice(void)
{
    if (m_pData == NULL)
    {
        // Configure ncurses
        WINDOW* const p_Window = initscr();
        raw();
        noecho();
        scrollok(p_Window, TRUE);
        keypad(p_Window, TRUE);

        m_pData = p_Window;
    }

    return true;
}

const bool Console::CloseDevice(void)
{
    if (m_pData != NULL)
    {
        m_pData = NULL;

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

void Console::PutString(const char* const STR_Out) const
{
    addstr(STR_Out);
    refresh();
}

void Console::Beep(void) const
{
    beep();
}
