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


#include <assert.h>

#include "cli/cli.h"
#include "cli/shell.h"
#include "cli/element.h"

using namespace cli;


Element::Element(const std::string& STR_Keyword, const Help& CLI_Help)
  : m_strKeyword(STR_Keyword),
    m_cliHelp(CLI_Help),
    m_pcliCli(NULL)
{
}

Element::~Element(void)
{
}

const std::string Element::GetKeyword(void) const
{
    return m_strKeyword;
}

const Help& Element::GetHelp(void) const
{
    return m_cliHelp;
}

const bool Element::FindElements(
        ElementList& CLI_ExactList,
        ElementList& CLI_NearList,
        const char* const STR_Keyword
        ) const
{
    // Should be overridden.
    assert(false);
    return false;
}

void Element::SetCli(Cli& CLI_Cli)
{
    m_pcliCli = & CLI_Cli;
}

Cli& Element::GetCli(void)
{
    assert(m_pcliCli != NULL);
    return *m_pcliCli;
}

const Cli& Element::GetCli(void) const
{
    assert(m_pcliCli != NULL);
    return *m_pcliCli;
}

Shell& Element::GetShell(void) const
{
    return GetCli().GetShell();
}

const OutputDevice& Element::GetOutputStream(void) const
{
    return GetShell().GetStream(OUTPUT_STREAM);
}

const OutputDevice& Element::GetErrorStream(void) const
{
    return GetShell().GetStream(ERROR_STREAM);
}

