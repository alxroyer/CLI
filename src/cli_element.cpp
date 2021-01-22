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


#include <assert.h>

#include "cli_cli.h"
#include "cli_shell.h"
#include "cli_element.h"


CcliElement::CcliElement(const std::string& STR_Keyword, const CcliHelp& CLI_Help)
  : m_strKeyword(STR_Keyword),
    m_cliHelp(CLI_Help)
{
    m_pcliCli = NULL;
}

CcliElement::~CcliElement(void)
{
}

std::string CcliElement::GetKeyword(void) const
{
    return m_strKeyword;
}

const CcliHelp& CcliElement::GetHelp(void) const
{
    return m_cliHelp;
}

bool CcliElement::FindElements(CcliElementList& CLI_OutList, const std::string& STR_Keyword) const
{
    assert(false);
    return false;
}

bool CcliElement::FindElements(CcliElementList& CLI_ExactList, CcliElementList& CLI_NearList, const std::string& STR_Keyword) const
{
    assert(false);
    return false;
}

void CcliElement::SetCli(CcliCli& CLI_Cli)
{
    m_pcliCli = & CLI_Cli;
}

CcliCli& CcliElement::GetCli(void)
{
    assert(m_pcliCli != NULL);
    return *m_pcliCli;
}

const CcliCli& CcliElement::GetCli(void) const
{
    assert(m_pcliCli != NULL);
    return *m_pcliCli;
}

CcliShell& CcliElement::GetShell(void) const
{
    return GetCli().GetShell();
}

const CcliOutputDevice& CcliElement::GetOutputStream(void) const
{
    return GetShell().GetOutputStream();
}

const CcliOutputDevice& CcliElement::GetErrorStream(void) const
{
    return GetShell().GetErrorStream();
}

