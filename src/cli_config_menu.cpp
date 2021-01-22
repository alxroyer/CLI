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


#include "cli_config_menu.h"
#include "cli_keyword.h"
#include "cli_endl.h"
#include "cli_command_line.h"
#include "cli_shell.h"


CcliConfigMenu::CcliConfigMenu(void)
  : CcliMenu("cli-config", CcliHelp()
        .AddHelp(CcliHelp::CLI_LANG_EN, "CLI settings menu")),
    m_pcliEcho(NULL), m_pcliEchoOn(NULL), m_pcliEchoOff(NULL), m_pcliEchoDevice(NULL),
    m_pcliBeep(NULL), m_pcliBeepOn(NULL), m_pcliBeepOff(NULL), m_iInitialBeepConfiguration(-1)
{
}

CcliConfigMenu::~CcliConfigMenu(void)
{
}

void CcliConfigMenu::SetCli(CcliCli& CLI_Cli)
{
    CcliMenu::SetCli(CLI_Cli);
    {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Modify echo behavior"));
        m_pcliEcho = dynamic_cast<CcliKeyword*>(& AddElement(new CcliKeyword("echo", cli_Help)));
        {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Set echo on"));
            m_pcliEchoOn = dynamic_cast<CcliKeyword*>(& m_pcliEcho->AddElement(new CcliKeyword("on", cli_Help)));
            m_pcliEchoOn->AddElement(new CcliEndl(cli_Help)); }
        {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Set echo off"));
            m_pcliEchoOff = dynamic_cast<CcliKeyword*>(& m_pcliEcho->AddElement(new CcliKeyword("off", cli_Help)));
            m_pcliEchoOff->AddElement(new CcliEndl(cli_Help)); }}
    {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Modify beep behavior"));
        m_pcliBeep = dynamic_cast<CcliKeyword*>(& AddElement(new CcliKeyword("beep", cli_Help)));
        {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Set beep on"));
            m_pcliBeepOn = dynamic_cast<CcliKeyword*>(& m_pcliBeep->AddElement(new CcliKeyword("on", cli_Help)));
            m_pcliBeepOn->AddElement(new CcliEndl(cli_Help)); }
        {   CcliHelp cli_Help(CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, "Set beep off"));
            m_pcliBeepOff = dynamic_cast<CcliKeyword*>(& m_pcliBeep->AddElement(new CcliKeyword("off", cli_Help)));
            m_pcliBeepOff->AddElement(new CcliEndl(cli_Help)); }}
}

const bool CcliConfigMenu::ExecuteReserved(const CcliCommandLine& CLI_CommandLine) const
{
    if (0) {}
    else if (& CLI_CommandLine[0] == & GetEchoNode())
    {
        if (0) {}
        else if (& CLI_CommandLine[1] == & GetEchoOnNode())
        {
            if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[2]))
            {
                EchoOn();
                return true;
            }
        }
        else if (& CLI_CommandLine[1] == & GetEchoOffNode())
        {
            if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[2]))
            {
                EchoOff();
                return true;
            }
        }
    }
    else if (& CLI_CommandLine[0] == & GetBeepNode())
    {
        if (0) {}
        else if (& CLI_CommandLine[1] == & GetBeepOnNode())
        {
            if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[2]))
            {
                BeepOn();
                return true;
            }
        }
        else if (& CLI_CommandLine[1] == & GetBeepOffNode())
        {
            if (dynamic_cast<const CcliEndl*>(& CLI_CommandLine[2]))
            {
                BeepOff();
                return true;
            }
        }
    }

    return CcliMenu::ExecuteReserved(CLI_CommandLine);
}

const bool CcliConfigMenu::Execute(const CcliCommandLine& CLI_CommandLine) const
{
    return false;
}

void CcliConfigMenu::EchoOn(void) const
{
    if (m_pcliEchoDevice != NULL)
    {
        GetShell().SetEchoStream(*m_pcliEchoDevice);
        GetOutputStream() << "echo is on" << CLI_ENDL;
        m_pcliEchoDevice = NULL;
    }
}

void CcliConfigMenu::EchoOff(void) const
{
    if (m_pcliEchoDevice == NULL)
    {
        m_pcliEchoDevice = & GetShell().GetEchoStream();
        GetShell().SetEchoStream(CcliOutputDevice::GetNullDevice());
        GetOutputStream() << "echo is off" << CLI_ENDL;
    }
}

void CcliConfigMenu::BeepOn(void) const
{
    if (m_iInitialBeepConfiguration < 0)
    {
        m_iInitialBeepConfiguration = GetShell().GetBeep();
    }

    if (! GetShell().GetBeep())
    {
        GetShell().SetBeep(true);
        GetOutputStream() << "beep is on" << CLI_ENDL;
    }
}

void CcliConfigMenu::BeepOff(void) const
{
    if (m_iInitialBeepConfiguration < 0)
    {
        m_iInitialBeepConfiguration = GetShell().GetBeep();
    }

    if (GetShell().GetBeep())
    {
        GetShell().SetBeep(false);
        GetOutputStream() << "beep is off" << CLI_ENDL;
    }
}

void CcliConfigMenu::RestoreInitialConfig(void) const
{
    //! Echo device restoration.
    if (m_pcliEchoDevice != NULL)
    {
        GetShell().SetEchoStream(*m_pcliEchoDevice);
        m_pcliEchoDevice = NULL;
    }

    //! Beep configuration restoration.
    if (m_iInitialBeepConfiguration >= 0)
    {
        GetShell().SetBeep(m_iInitialBeepConfiguration ? true : false);
    }
}

CcliKeyword& CcliConfigMenu::GetEchoNode(void)
{
    return *m_pcliEcho;
}

const CcliKeyword& CcliConfigMenu::GetEchoNode(void) const
{
    return *m_pcliEcho;
}

CcliKeyword& CcliConfigMenu::GetEchoOnNode(void)
{
    return *m_pcliEchoOn;
}

const CcliKeyword& CcliConfigMenu::GetEchoOnNode(void) const
{
    return *m_pcliEchoOn;
}

CcliKeyword& CcliConfigMenu::GetEchoOffNode(void)
{
    return *m_pcliEchoOff;
}

const CcliKeyword& CcliConfigMenu::GetEchoOffNode(void) const
{
    return *m_pcliEchoOff;
}

CcliKeyword& CcliConfigMenu::GetBeepNode(void)
{
    return *m_pcliBeep;
}

const CcliKeyword& CcliConfigMenu::GetBeepNode(void) const
{
    return *m_pcliBeep;
}

CcliKeyword& CcliConfigMenu::GetBeepOnNode(void)
{
    return *m_pcliBeepOn;
}

const CcliKeyword& CcliConfigMenu::GetBeepOnNode(void) const
{
    return *m_pcliBeepOn;
}

CcliKeyword& CcliConfigMenu::GetBeepOffNode(void)
{
    return *m_pcliBeepOff;
}

const CcliKeyword& CcliConfigMenu::GetBeepOffNode(void) const
{
    return *m_pcliBeepOff;
}
