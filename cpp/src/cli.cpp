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


#include <regex.h>

#include "cli/cli.h"
#include "cli/menu.h"
#include "cli/shell.h"
#include "cli/keyword.h"
#include "cli/endl.h"
#include "cli/command_line.h"
#include "cli/traces.h"
#include "config_menu.h"
#ifdef _DEBUG
#include "traces_menu.h"
#endif
#include "consistency.h"

using namespace cli;


static const TraceClass TRACE_CLI("CLI", "CLI traces");

static CliList M_CLIList;

Cli::Cli(const std::string& STR_Name, const Help& CLI_Help)
  : Menu(STR_Name, CLI_Help),
    m_pcliShell(NULL),
    m_pcliConfigMenu(NULL), m_pcliConfigMenuNode(NULL)
{
    EnsureCommonDevices();
    EnsureTraces();

    // Set the CLI reference to oneself.
    SetCli(*this);

    // Register this new CLI in the global CLI list.
    M_CLIList.push_back(this);

    GetTraces().Trace(TRACE_CLI) << "New CLI '" << GetName() << "'." << cli::endl;
}

Cli::~Cli(void)
{
    GetTraces().Trace(TRACE_CLI) << "Deletion of CLI '" << GetName() << "'." << cli::endl;

    // Remove this CLI from the global CLI list.
    for (   CliList::iterator it = M_CLIList.begin();
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

const int Cli::FindFromName(CliList& CLI_CliList, const std::string& STR_RegExp)
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
    for (   CliList::iterator it = M_CLIList.begin();
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

const std::string Cli::GetName(void) const
{
    return GetKeyword();
}

Menu& Cli::AddMenu(Menu* const PCLI_Menu)
{
    PCLI_Menu->SetCli(*this);
    m_vpcliMenus.push_back(PCLI_Menu);
    return *PCLI_Menu;
}

void Cli::SetShell(Shell& CLI_Shell) const
{
    m_pcliShell = & CLI_Shell;
}

Shell& Cli::GetShell(void) const
{
    if (m_pcliShell != NULL)
    {
        return *m_pcliShell;
    }
    else
    {
        static Shell cli_Shell(*this);
        return cli_Shell;
    }
}

ConfigMenu& Cli::GetConfigMenu(void)
{
    assert(m_pcliConfigMenu != NULL);
    return *m_pcliConfigMenu;
}

const ConfigMenu& Cli::GetConfigMenu(void) const
{
    assert(m_pcliConfigMenu != NULL);
    return *m_pcliConfigMenu;
}

const Keyword& Cli::GetConfigMenuNode(void) const
{
    assert(m_pcliConfigMenuNode != NULL);
    return *m_pcliConfigMenuNode;
}

#ifdef _DEBUG
TracesMenu& Cli::GetTracesMenu(void)
{
    assert(m_pcliTracesMenu != NULL);
    return *m_pcliTracesMenu;
}

const TracesMenu& Cli::GetTracesMenu(void) const
{
    assert(m_pcliTracesMenu != NULL);
    return *m_pcliTracesMenu;
}

const Keyword& Cli::GetTracesMenuNode(void) const
{
    assert(m_pcliTracesMenuNode != NULL);
    return *m_pcliTracesMenuNode;
}
#endif

void Cli::SetCli(Cli& CLI_Cli)
{
    Menu::SetCli(CLI_Cli);

    m_pcliConfigMenu = dynamic_cast<ConfigMenu*>(& AddMenu(new ConfigMenu()));
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "CLI configuration menu"));
        m_pcliConfigMenuNode = dynamic_cast<Keyword*>(& AddElement(new Keyword("cli-config", cli_Help)));
        Endl* const pcli_Endl = dynamic_cast<Endl*>(& m_pcliConfigMenuNode->AddElement(new Endl(cli_Help)));
        pcli_Endl->SetMenuRef(new MenuRef(*m_pcliConfigMenu)); }
    #ifdef _DEBUG
    m_pcliTracesMenu = dynamic_cast<TracesMenu*>(& AddMenu(new TracesMenu()));
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Traces menu"));
        m_pcliTracesMenuNode = dynamic_cast<Keyword*>(& AddElement(new Keyword("traces", cli_Help)));
        Endl* const pcli_Endl = dynamic_cast<Endl*>(& m_pcliTracesMenuNode->AddElement(new Endl(cli_Help)));
        pcli_Endl->SetMenuRef(new MenuRef(*m_pcliTracesMenu)); }
    #endif
}

const bool Cli::ExecuteReserved(const CommandLine& CLI_CommandLine) const
{
    if (0) {}
    else if (& CLI_CommandLine[0] == & GetConfigMenuNode())
    {
        if (dynamic_cast<const Endl*>(& CLI_CommandLine[1]))
        {
            // Do nothing but return true so that an execution error is not detected.
            return true;
        }
    }
    #ifdef _DEBUG
    else if (& CLI_CommandLine[0] == & GetTracesMenuNode())
    {
        if (dynamic_cast<const Endl*>(& CLI_CommandLine[1]))
        {
            // Do nothing but return true so that an execution error is not detected.
            return true;
        }
    }
    #endif

    return Menu::ExecuteReserved(CLI_CommandLine);
}

void Cli::OnExit(void) const
{
}
