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


#include "cli_cli.h"
#include "cli_menu.h"
#include "cli_shell.h"


CcliCli::CcliCli(const std::string& STR_Name, const CcliHelp& CLI_Help)
  : CcliMenu(STR_Name, CLI_Help)
{
    m_pcliShell = NULL;
    SetCli(*this);

    AddMenu(new CcliMenu(STR_Name, CLI_Help));
}

CcliCli::~CcliCli(void)
{
    while (! m_vpcliMenus.empty())
    {
        delete m_vpcliMenus.back();
        m_vpcliMenus.pop_back();
    }
}

CcliMenu& CcliCli::AddMenu(CcliMenu* PCLI_Menu)
{
    PCLI_Menu->SetCli(*this);
    m_vpcliMenus.push_back(PCLI_Menu);
    return *PCLI_Menu;
}

void CcliCli::SetShell(CcliShell& CLI_Shell)
{
    m_pcliShell = & CLI_Shell;
}

CcliShell& CcliCli::GetShell(void) const
{
    assert(m_pcliShell != NULL);
    return *m_pcliShell;
}

