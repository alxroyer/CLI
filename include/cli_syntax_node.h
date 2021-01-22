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


#ifndef _CLI_SYNTAX_NODE_H_
#define _CLI_SYNTAX_NODE_H_

#include "cli_element.h"


class CcliHelp;


class CcliSyntaxNode : public CcliElement
{
public:
    CcliSyntaxNode(const std::string& STR_Keyword, const CcliHelp& CLI_Help);
    virtual ~CcliSyntaxNode(void);

public:
    CcliElement& AddElement(CcliElement* PCLI_Element);
    virtual bool FindElements(
        CcliElementList& CLI_OutList,
        const std::string& STR_Keyword) const;
    virtual bool FindElements(
        CcliElementList& CLI_ExactList,
        CcliElementList& CLI_NearList,
        const std::string& STR_Keyword) const;

private:
    CcliElementMap m_cliElements;
};

#endif // _CLI_SYNTAX_NODE_H_
