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


#ifndef _CLI_COMMAND_LINE_H_
#define _CLI_COMMAND_LINE_H_

#include <string>
#include <vector>

#include "cli_element.h"


class CcliMenu;


class CcliCommandLine
{
public:
    CcliCommandLine(void);
    virtual ~CcliCommandLine(void);

public:
    bool Parse(const CcliMenu& CLI_Menu, const std::string& STR_Line, bool B_Execution);

    const CcliElement& operator[](int I_Pos) const;
    const CcliElement& GetLastElement(void) const;
    int GetElementCount(void) const;

    std::string GetLastWord(void) const;

private:
    std::vector<std::string> Split(const std::string& STR_Line);
    CcliCommandLine& AddElement(const CcliElement* PCLI_Element);

private:
    CcliElementList m_cliElements;
    CcliElementList m_cliAutoDelete;
    const CcliElement* m_pcliMenu;
    std::string m_strLastWord;
};

#endif // _CLI_COMMAND_LINE_H_
