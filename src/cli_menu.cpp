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


#include "cli_menu.h"
#include "cli_keyword.h"
#include "cli_endl.h"
#include "cli_shell.h"
#include "cli_io_device.h"
#include "cli_command_line.h"
#include "cli_help.h"


CcliMenu::CcliMenu(const std::string& STR_Name, const CcliHelp& CLI_Help)
  : CcliSyntaxNode(STR_Name, CLI_Help),
    m_pcliHelp(NULL), m_pcliExit(NULL), m_pcliQuit(NULL), m_pcliPwm(NULL)
{
}

CcliMenu::~CcliMenu(void)
{
}

CcliKeyword& CcliMenu::GetHelpNode(void)
{
    return *m_pcliHelp;
}

const CcliKeyword& CcliMenu::GetHelpNode(void) const
{
    return *m_pcliHelp;
}

CcliKeyword& CcliMenu::GetExitNode(void)
{
    return *m_pcliExit;
}

const CcliKeyword& CcliMenu::GetExitNode(void) const
{
    return *m_pcliExit;
}

CcliKeyword& CcliMenu::GetQuitNode(void)
{
    return *m_pcliQuit;
}

const CcliKeyword& CcliMenu::GetQuitNode(void) const
{
    return *m_pcliQuit;
}

CcliKeyword& CcliMenu::GetPwmNode(void)
{
    return *m_pcliPwm;
}

const CcliKeyword& CcliMenu::GetPwmNode(void) const
{
    return *m_pcliPwm;
}

void CcliMenu::SetCli(CcliCli& CLI_Cli)
{
    CcliSyntaxNode::SetCli(CLI_Cli);
    {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Get help"));
        m_pcliHelp = dynamic_cast<CcliKeyword*>(& AddElement(new CcliKeyword("help", cli_Help)));
        m_pcliHelp->AddElement(new CcliEndl(cli_Help)); }
    {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Exit menu " + GetKeyword()));
        m_pcliExit = dynamic_cast<CcliKeyword*>(& AddElement(new CcliKeyword("exit", cli_Help)));
        m_pcliExit->AddElement(new CcliEndl(cli_Help)); }
    {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Quit"));
        m_pcliQuit = dynamic_cast<CcliKeyword*>(& AddElement(new CcliKeyword("quit", cli_Help)));
        m_pcliQuit->AddElement(new CcliEndl(cli_Help)); }
    {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Print Working Menu"));
        m_pcliPwm = dynamic_cast<CcliKeyword*>(& AddElement(new CcliKeyword("pwm", cli_Help)));
        m_pcliPwm->AddElement(new CcliEndl(cli_Help)); }
}

const bool CcliMenu::ExecuteReserved(const CcliCommandLine& CLI_CommandLine) const
{
    if (0) {}
    else if (& CLI_CommandLine[0] == & GetHelpNode())
    {
        if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[1]))
        {
            GetShell().DisplayHelp();
            return true;
        }
    }
    else if (& CLI_CommandLine[0] == & GetExitNode())
    {
        if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[1]))
        {
            GetShell().ExitMenu();
            return true;
        }
    }
    else if (& CLI_CommandLine[0] == & GetQuitNode())
    {
        if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[1]))
        {
            GetShell().Quit();
            return true;
        }
    }
    else if (& CLI_CommandLine[0] == & GetPwmNode())
    {
        if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[1]))
        {
            GetShell().PrintWorkingMenu();
            return true;
        }
    }

    return false;
}

const bool CcliMenu::Execute(const CcliCommandLine& CLI_CommandLine) const
{
    GetErrorStream() << "(no execution defined)" << CLI_ENDL;
    return true;
}

void CcliMenu::OnExit(void) const
{
}


CcliMenuRef::CcliMenuRef(const CcliMenu& CLI_Menu)
  : CcliElement("", CcliHelp()),
    m_pcliMenu(& CLI_Menu)
{
}

CcliMenuRef::~CcliMenuRef(void)
{
}

const CcliMenu& CcliMenuRef::GetMenu(void) const
{
    return *m_pcliMenu;
}

