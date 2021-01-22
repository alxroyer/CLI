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

#include "cli/param.h"

using namespace cli;


Param::Param(const std::string& STR_Keyword, const Help& CLI_Help)
  : SyntaxNode(STR_Keyword, CLI_Help),
    m_pcliCloned(NULL)
{
}

Param::~Param(void)
{
}

const std::string Param::GetKeyword(void) const
{
    if (! m_strValue.empty())
    {
        return GetstrValue();
    }
    else
    {
        return SyntaxNode::GetKeyword();
    }
}

const bool Param::FindElements(
        ElementList& CLI_ExactList,
        ElementList& CLI_NearList,
        const char* const STR_Keyword
        ) const
{
    if (GetCloned() != NULL)
    {
        return GetCloned()->FindElements(CLI_ExactList, CLI_NearList, STR_Keyword);
    }
    else
    {
        return SyntaxNode::FindElements(CLI_ExactList, CLI_NearList, STR_Keyword);
    }
}

const std::string Param::GetstrValue(void) const
{
    return m_strValue;
}

const Param* const Param::GetCloned(void) const
{
    return m_pcliCloned;
}

void Param::SetValue(const std::string& STR_Value) const
{
    m_strValue = STR_Value;
}

const Param* const Param::InitClone(Param& CLI_Param) const
{
    CLI_Param.SetCli(const_cast<Cli&>(GetCli()));
    CLI_Param.SetstrValue(GetstrValue());
    CLI_Param.SetCloned(*this);
    return & CLI_Param;
}

void Param::SetCloned(const Param& CLI_Cloned)
{
    m_pcliCloned = & CLI_Cloned;
}

