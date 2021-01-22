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


#include "cli/endl.h"
#include "cli/cli.h"
#include "cli/menu.h"

using namespace cli;


Endl::Endl(const Help& CLI_Help)
  : Element("\n", CLI_Help),
    m_pcliMenuRef(NULL)
{
}

Endl::~Endl(void)
{
    if (m_pcliMenuRef != NULL)
    {
        delete m_pcliMenuRef;
        m_pcliMenuRef = NULL;
    }
}

Menu& Endl::SetMenu(Menu* const PCLI_Menu)
{
    if (m_pcliMenuRef != NULL)
    {
        delete m_pcliMenuRef;
        m_pcliMenuRef = NULL;
    }

    GetCli().AddMenu(PCLI_Menu);
    PCLI_Menu->SetCli(GetCli());
    SetMenuRef(new MenuRef(*PCLI_Menu));
    return *PCLI_Menu;
}

MenuRef& Endl::SetMenuRef(MenuRef* const PCLI_MenuRef)
{
    if (m_pcliMenuRef != NULL)
    {
        delete m_pcliMenuRef;
        m_pcliMenuRef = NULL;
    }

    PCLI_MenuRef->SetCli(GetCli());
    m_pcliMenuRef = PCLI_MenuRef;
    return *PCLI_MenuRef;
}

const MenuRef* const Endl::GetMenuRef(void) const
{
    return m_pcliMenuRef;
}


