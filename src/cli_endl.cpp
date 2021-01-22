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


#include "cli_endl.h"
#include "cli_cli.h"
#include "cli_menu.h"


CcliEndl::CcliEndl(const CcliHelp& CLI_Help)
  : CcliElement("\n", CLI_Help)
{
    m_pcliMenuRef = NULL;
}

CcliEndl::~CcliEndl(void)
{
    if (m_pcliMenuRef != NULL)
    {
        delete m_pcliMenuRef;
        m_pcliMenuRef = NULL;
    }
}

CcliMenu& CcliEndl::SetMenu(CcliMenu* PCLI_Menu)
{
    if (m_pcliMenuRef != NULL)
    {
        delete m_pcliMenuRef;
        m_pcliMenuRef = NULL;
    }

    GetCli().AddMenu(PCLI_Menu);
    PCLI_Menu->SetCli(GetCli());
    SetMenuRef(new CcliMenuRef(*PCLI_Menu));
    return *PCLI_Menu;
}

CcliMenuRef& CcliEndl::SetMenuRef(CcliMenuRef* PCLI_MenuRef)
{
    if (m_pcliMenuRef != NULL)
    {
        delete m_pcliMenuRef;
        m_pcliMenuRef = NULL;
    }

    m_pcliMenuRef = PCLI_MenuRef;
    m_pcliMenuRef->SetCli(GetCli());
    return *m_pcliMenuRef;
}

const CcliMenuRef* CcliEndl::GetMenuRef(void) const
{
    return m_pcliMenuRef;
}


