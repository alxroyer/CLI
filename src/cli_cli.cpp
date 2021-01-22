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


#include <regex.h>

#include "cli_cli.h"
#include "cli_menu.h"
#include "cli_shell.h"
#include "cli_config_menu.h"
#include "cli_keyword.h"
#include "cli_endl.h"
#include "cli_command_line.h"


CcliCliList CcliCli::M_CLIList;

CcliCli::CcliCli(const std::string& STR_Name, const CcliHelp& CLI_Help)
  : CcliMenu(STR_Name, CLI_Help),
    m_pcliShell(NULL),
    m_pcliConfigMenu(NULL), m_pcliConfigMenuNode(NULL)
{
    // Set the CLI reference to oneself.
    SetCli(*this);

    // Register this new CLI in the global CLI list.
    M_CLIList.push_back(this);
}

CcliCli::~CcliCli(void)
{
    // Remove this CLI from the global CLI list.
    for (   CcliCliList::iterator it = M_CLIList.begin();
            it != M_CLIList.end();
            it ++)
    {
        if (*it == this)
        {
            it = M_CLIList.erase(it);
            break;
        }
    }

    // Destroy menus.
    while (! m_vpcliMenus.empty())
    {
        delete m_vpcliMenus.back();
        m_vpcliMenus.pop_back();
    }
}

const int CcliCli::FindFromName(CcliCliList& CLI_CliList, const std::string& STR_RegExp)
{
    // Compile the regular expression.
    regex_t s_RegExp;
    memset(& s_RegExp, '\0', sizeof(regex_t));
    if (regcomp(& s_RegExp, STR_RegExp.c_str(), 0) != 0)
    {
        
        return -1;
    }

    // Check the name for each CLI instance.
    const int i_InitialCount = CLI_CliList.size();
    for (   CcliCliList::iterator it = M_CLIList.begin();
            it != M_CLIList.end();
            it ++)
    {
        if (regexec(& s_RegExp, (*it)->GetName().c_str(), 0, NULL, 0) == 0)
        {
            CLI_CliList.push_back(*it);
        }
    }

    // Free.
    regfree(& s_RegExp);

    return (CLI_CliList.size() - i_InitialCount);
}

const std::string CcliCli::GetName(void) const
{
    return GetKeyword();
}

CcliMenu& CcliCli::AddMenu(CcliMenu* const PCLI_Menu)
{
    PCLI_Menu->SetCli(*this);
    m_vpcliMenus.push_back(PCLI_Menu);
    return *PCLI_Menu;
}

void CcliCli::SetShell(CcliShell& CLI_Shell) const
{
    m_pcliShell = & CLI_Shell;
}

CcliShell& CcliCli::GetShell(void) const
{
    assert(m_pcliShell != NULL);
    return *m_pcliShell;
}

CcliConfigMenu& CcliCli::GetConfigMenu(void)
{
    assert(m_pcliConfigMenu != NULL);
    return *m_pcliConfigMenu;
}

const CcliConfigMenu& CcliCli::GetConfigMenu(void) const
{
    assert(m_pcliConfigMenu != NULL);
    return *m_pcliConfigMenu;
}

CcliKeyword& CcliCli::GetConfigMenuNode(void)
{
    assert(m_pcliConfigMenuNode != NULL);
    return *m_pcliConfigMenuNode;
}

const CcliKeyword& CcliCli::GetConfigMenuNode(void) const
{
    assert(m_pcliConfigMenuNode != NULL);
    return *m_pcliConfigMenuNode;
}

void CcliCli::SetCli(CcliCli& CLI_Cli)
{
    CcliMenu::SetCli(CLI_Cli);

    m_pcliConfigMenu = dynamic_cast<CcliConfigMenu*>(& AddMenu(new CcliConfigMenu()));
    {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "CLI configuration menu"));
        m_pcliConfigMenuNode = dynamic_cast<CcliKeyword*>(& AddElement(new CcliKeyword("cli-config", cli_Help)));
        CcliEndl* const pcli_Endl = dynamic_cast<CcliEndl*>(& m_pcliConfigMenuNode->AddElement(new CcliEndl(cli_Help)));
        pcli_Endl->SetMenuRef(new CcliMenuRef(*m_pcliConfigMenu)); }
}

const bool CcliCli::ExecuteReserved(const CcliCommandLine& CLI_CommandLine) const
{
    if (& CLI_CommandLine[0] == & GetConfigMenuNode())
    {
        if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[1]))
        {
            // Do nothing but return true so that an execution error is not detected.
            return true;
        }
    }

    return CcliMenu::ExecuteReserved(CLI_CommandLine);
}

void CcliCli::OnExit(void) const
{
    GetConfigMenu().RestoreInitialConfig();
}
