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


#include "cli/menu.h"
#include "cli/keyword.h"
#include "cli/endl.h"
#include "cli/shell.h"
#include "cli/io_device.h"
#include "cli/command_line.h"
#include "cli/help.h"

using namespace cli;


Menu::Menu(const std::string& STR_Name, const Help& CLI_Help)
  : SyntaxNode(STR_Name, CLI_Help),
    m_pcliHelp(NULL), m_pcliExit(NULL), m_pcliQuit(NULL), m_pcliPwm(NULL)
{
}

Menu::~Menu(void)
{
}

Keyword& Menu::GetHelpNode(void)
{
    return *m_pcliHelp;
}

const Keyword& Menu::GetHelpNode(void) const
{
    return *m_pcliHelp;
}

Keyword& Menu::GetExitNode(void)
{
    return *m_pcliExit;
}

const Keyword& Menu::GetExitNode(void) const
{
    return *m_pcliExit;
}

Keyword& Menu::GetQuitNode(void)
{
    return *m_pcliQuit;
}

const Keyword& Menu::GetQuitNode(void) const
{
    return *m_pcliQuit;
}

Keyword& Menu::GetPwmNode(void)
{
    return *m_pcliPwm;
}

const Keyword& Menu::GetPwmNode(void) const
{
    return *m_pcliPwm;
}

void Menu::SetCli(Cli& CLI_Cli)
{
    SyntaxNode::SetCli(CLI_Cli);
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Get help"));
        m_pcliHelp = dynamic_cast<Keyword*>(& AddElement(new Keyword("help", cli_Help)));
        m_pcliHelp->AddElement(new Endl(cli_Help)); }
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Exit menu " + GetKeyword()));
        m_pcliExit = dynamic_cast<Keyword*>(& AddElement(new Keyword("exit", cli_Help)));
        m_pcliExit->AddElement(new Endl(cli_Help)); }
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Quit"));
        m_pcliQuit = dynamic_cast<Keyword*>(& AddElement(new Keyword("quit", cli_Help)));
        m_pcliQuit->AddElement(new Endl(cli_Help)); }
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Print Working Menu"));
        m_pcliPwm = dynamic_cast<Keyword*>(& AddElement(new Keyword("pwm", cli_Help)));
        m_pcliPwm->AddElement(new Endl(cli_Help)); }
}

const bool Menu::ExecuteReserved(const CommandLine& CLI_CommandLine) const
{
    if (0) {}
    else if (& CLI_CommandLine[0] == & GetHelpNode())
    {
        if (dynamic_cast<const Endl*>(& CLI_CommandLine[1]))
        {
            GetShell().DisplayHelp();
            return true;
        }
    }
    else if (& CLI_CommandLine[0] == & GetExitNode())
    {
        if (dynamic_cast<const Endl*>(& CLI_CommandLine[1]))
        {
            GetShell().ExitMenu();
            return true;
        }
    }
    else if (& CLI_CommandLine[0] == & GetQuitNode())
    {
        if (dynamic_cast<const Endl*>(& CLI_CommandLine[1]))
        {
            GetShell().Quit();
            return true;
        }
    }
    else if (& CLI_CommandLine[0] == & GetPwmNode())
    {
        if (dynamic_cast<const Endl*>(& CLI_CommandLine[1]))
        {
            GetShell().PrintWorkingMenu();
            return true;
        }
    }

    return false;
}

const bool Menu::Execute(const CommandLine& CLI_CommandLine) const
{
    GetErrorStream() << "(no execution defined)" << endl;
    return true;
}

void Menu::OnExit(void) const
{
}


MenuRef::MenuRef(const Menu& CLI_Menu)
  : Element("", Help()),
    m_pcliMenu(& CLI_Menu)
{
}

MenuRef::~MenuRef(void)
{
}

const Menu& MenuRef::GetMenu(void) const
{
    return *m_pcliMenu;
}

