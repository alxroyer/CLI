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

#include "cli_param.h"


CcliParam::CcliParam(const std::string& STR_Keyword, const CcliHelp& CLI_Help)
  : CcliSyntaxNode(STR_Keyword, CLI_Help)
{
    m_pcliCloned = NULL;
}

CcliParam::~CcliParam(void)
{
}

std::string CcliParam::GetKeyword(void) const
{
    if (! m_strValue.empty())
    {
        return GetstrValue();
    }
    else
    {
        return CcliSyntaxNode::GetKeyword();
    }
}

bool CcliParam::FindElements(CcliElementList& CLI_ExactList, CcliElementList& CLI_NearList, const std::string& STR_Keyword) const
{
    if (GetCloned() != NULL)
    {
        return GetCloned()->FindElements(CLI_ExactList, CLI_NearList, STR_Keyword);
    }
    else
    {
        return CcliSyntaxNode::FindElements(CLI_ExactList, CLI_NearList, STR_Keyword);
    }
}

std::string CcliParam::GetstrValue(void) const
{
    return m_strValue;
}

const CcliParam* CcliParam::GetCloned(void) const
{
    return m_pcliCloned;
}

void CcliParam::SetValue(const std::string& STR_Value) const
{
    m_strValue = STR_Value;
}

const CcliParam* CcliParam::InitClone(CcliParam* PCLI_Param) const
{
    PCLI_Param->SetCli(const_cast<CcliCli&>(GetCli()));
    PCLI_Param->SetstrValue(GetstrValue());
    PCLI_Param->SetCloned(this);
    return PCLI_Param;
}

void CcliParam::SetCloned(const CcliParam* PCLI_Cloned)
{
    m_pcliCloned = PCLI_Cloned;
}

