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


#include "config_menu.h"
#include "cli/keyword.h"
#include "cli/endl.h"
#include "cli/command_line.h"
#include "cli/shell.h"

using namespace cli;


ConfigMenu::ConfigMenu(void)
  : Menu("cli-config", Help()
        .AddHelp(Help::LANG_EN, "CLI settings menu")),
    m_pcliEcho(NULL), m_pcliEchoOn(NULL), m_pcliEchoOff(NULL),
    m_pcliBeep(NULL), m_pcliBeepOn(NULL), m_pcliBeepOff(NULL)
{
}

ConfigMenu::~ConfigMenu(void)
{
}

void ConfigMenu::SetCli(Cli& CLI_Cli)
{
    Menu::SetCli(CLI_Cli);
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Modify echo behavior"));
        m_pcliEcho = dynamic_cast<Keyword*>(& AddElement(new Keyword("echo", cli_Help)));
        {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Set echo on"));
            m_pcliEchoOn = dynamic_cast<Keyword*>(& m_pcliEcho->AddElement(new Keyword("on", cli_Help)));
            m_pcliEchoOn->AddElement(new Endl(cli_Help)); }
        {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Set echo off"));
            m_pcliEchoOff = dynamic_cast<Keyword*>(& m_pcliEcho->AddElement(new Keyword("off", cli_Help)));
            m_pcliEchoOff->AddElement(new Endl(cli_Help)); }}
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Modify beep behavior"));
        m_pcliBeep = dynamic_cast<Keyword*>(& AddElement(new Keyword("beep", cli_Help)));
        {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Set beep on"));
            m_pcliBeepOn = dynamic_cast<Keyword*>(& m_pcliBeep->AddElement(new Keyword("on", cli_Help)));
            m_pcliBeepOn->AddElement(new Endl(cli_Help)); }
        {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Set beep off"));
            m_pcliBeepOff = dynamic_cast<Keyword*>(& m_pcliBeep->AddElement(new Keyword("off", cli_Help)));
            m_pcliBeepOff->AddElement(new Endl(cli_Help)); }}
}

const bool ConfigMenu::ExecuteReserved(const CommandLine& CLI_CmdLine) const
{
    int i_CmdLineIndex = 0;
    const cli::Element* pcli_Element = NULL;
    #define CLI_STEP_IT() do { if (i_CmdLineIndex >= CLI_CmdLine.GetElementCount()) return false; pcli_Element = & CLI_CmdLine[i_CmdLineIndex++]; } while(0)

    CLI_STEP_IT();
    if (0) {}
    else if (pcli_Element == & GetEchoNode())
    {
        CLI_STEP_IT();
        if (0) {}
        else if (pcli_Element == & GetEchoOnNode())
        {
            CLI_STEP_IT();
            if (dynamic_cast<const Endl*>(pcli_Element))
            {
                EchoOn();
                return true;
            }
        }
        else if (pcli_Element == & GetEchoOffNode())
        {
            CLI_STEP_IT();
            if (dynamic_cast<const Endl*>(pcli_Element))
            {
                EchoOff();
                return true;
            }
        }
    }
    else if (pcli_Element == & GetBeepNode())
    {
        CLI_STEP_IT();
        if (0) {}
        else if (pcli_Element == & GetBeepOnNode())
        {
            CLI_STEP_IT();
            if (dynamic_cast<const Endl*>(pcli_Element))
            {
                BeepOn();
                return true;
            }
        }
        else if (pcli_Element == & GetBeepOffNode())
        {
            CLI_STEP_IT();
            if (dynamic_cast<const Endl*>(pcli_Element))
            {
                BeepOff();
                return true;
            }
        }
    }

    return Menu::ExecuteReserved(CLI_CmdLine);
}

const bool ConfigMenu::Execute(const CommandLine& CLI_CmdLine) const
{
    return false;
}

void ConfigMenu::EchoOn(void) const
{
    GetShell().EnableStream(ECHO_STREAM, true);
    GetOutputStream() << "echo is on" << endl;
}

void ConfigMenu::EchoOff(void) const
{
    GetShell().EnableStream(ECHO_STREAM, false);
    GetOutputStream() << "echo is off" << endl;
}

void ConfigMenu::BeepOn(void) const
{
    GetShell().SetBeep(true);
    GetOutputStream() << "beep is on" << endl;
}

void ConfigMenu::BeepOff(void) const
{
    GetShell().SetBeep(false);
    GetOutputStream() << "beep is off" << endl;
}

Keyword& ConfigMenu::GetEchoNode(void)
{
    return *m_pcliEcho;
}

const Keyword& ConfigMenu::GetEchoNode(void) const
{
    return *m_pcliEcho;
}

Keyword& ConfigMenu::GetEchoOnNode(void)
{
    return *m_pcliEchoOn;
}

const Keyword& ConfigMenu::GetEchoOnNode(void) const
{
    return *m_pcliEchoOn;
}

Keyword& ConfigMenu::GetEchoOffNode(void)
{
    return *m_pcliEchoOff;
}

const Keyword& ConfigMenu::GetEchoOffNode(void) const
{
    return *m_pcliEchoOff;
}

Keyword& ConfigMenu::GetBeepNode(void)
{
    return *m_pcliBeep;
}

const Keyword& ConfigMenu::GetBeepNode(void) const
{
    return *m_pcliBeep;
}

Keyword& ConfigMenu::GetBeepOnNode(void)
{
    return *m_pcliBeepOn;
}

const Keyword& ConfigMenu::GetBeepOnNode(void) const
{
    return *m_pcliBeepOn;
}

Keyword& ConfigMenu::GetBeepOffNode(void)
{
    return *m_pcliBeepOff;
}

const Keyword& ConfigMenu::GetBeepOffNode(void) const
{
    return *m_pcliBeepOff;
}
