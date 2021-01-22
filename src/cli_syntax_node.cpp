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


#include "cli_syntax_node.h"
#include "cli_syntax_tag.h"
#include "cli_param.h"


CcliSyntaxNode::CcliSyntaxNode(const std::string& STR_Keyword, const CcliHelp& CLI_Help)
  : CcliElement(STR_Keyword, CLI_Help)
{
}

CcliSyntaxNode::~CcliSyntaxNode(void)
{
    for (   CcliElementMap::const_iterator it = m_cliElements.begin();
            it != m_cliElements.end();
            it ++)
    {
        delete it->second;
    }
    m_cliElements.clear();
}

CcliElement& CcliSyntaxNode::AddElement(CcliElement* const PCLI_Element)
{
    CcliElementMap::const_iterator it = m_cliElements.find(PCLI_Element->GetKeyword());
    if (it != m_cliElements.end())
    {
        delete PCLI_Element;
	return *const_cast<CcliElement*>(it->second);
    }
    else
    {
        PCLI_Element->SetCli(GetCli());
        m_cliElements[PCLI_Element->GetKeyword()] = PCLI_Element;
        return *PCLI_Element;
    }
}

const bool CcliSyntaxNode::FindElements(
        CcliElementList& CLI_ExactList,
        CcliElementList& CLI_NearList,
        const std::string& STR_Keyword
        ) const
{

    for (   CcliElementMap::const_iterator it = m_cliElements.begin();
            it != m_cliElements.end();
            it ++)
    {
        const CcliElement* const pcli_Element = it->second;

        if (0) {}
        else if (const CcliSyntaxTag* const pcli_Tag = dynamic_cast<const CcliSyntaxTag*>(pcli_Element))
        {
            if (! pcli_Tag->GetbHollow())
            {
                if (! pcli_Tag->FindElements(CLI_ExactList, CLI_NearList, STR_Keyword))
                {
                    return false;
                }
            }
        }
        else if (const CcliSyntaxRef* const pcli_Ref = dynamic_cast<const CcliSyntaxRef*>(pcli_Element))
        {
            if (! pcli_Ref->GetTag().FindElements(CLI_ExactList, CLI_NearList, STR_Keyword))
            {
                return false;
            }
        }
        else if (const CcliParam* const pcli_Param = dynamic_cast<const CcliParam*>(pcli_Element))
        {
            if (STR_Keyword != "\n")
            {
                if (pcli_Param->SetstrValue(STR_Keyword))
                {
                    CLI_ExactList.push_back(pcli_Param);
		    CLI_NearList.push_back(pcli_Param);
                }
            }
        }
        else if (pcli_Element->GetKeyword().compare(0, STR_Keyword.size(), STR_Keyword) == 0)
        {
            CLI_NearList.push_back(pcli_Element);
            if (STR_Keyword.size() == pcli_Element->GetKeyword().size())
            {
                CLI_ExactList.push_back(pcli_Element);
            }
        }
    }

    return true;
}

