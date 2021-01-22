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


#ifndef _CLI_ELEMENT_H_
#define _CLI_ELEMENT_H_

#include <string>
#include <deque>
#include <map>

#include "cli_help.h"


class CcliElement;
typedef std::deque<const CcliElement*> CcliElementList;
typedef std::map<std::string, const CcliElement*> CcliElementMap;

class CcliCli;
class CcliShell;
class CcliOutputDevice;
class CcliHelp;


class CcliElement
{
public:
    CcliElement(const std::string& STR_Keyword, const CcliHelp& STR_Help);
    virtual ~CcliElement(void);

public:
    virtual std::string GetKeyword(void) const;
    virtual const CcliHelp& GetHelp(void) const;

    virtual bool FindElements(CcliElementList& CLI_OutList, const std::string& STR_Keyword) const;
    virtual bool FindElements(CcliElementList& CLI_ExactList, CcliElementList& CLI_NearList, const std::string& STR_Keyword) const;

    virtual void SetCli(CcliCli& CLI_Cli);
    CcliCli& GetCli(void);
    const CcliCli& GetCli(void) const;
    CcliShell& GetShell(void) const;
    const CcliOutputDevice& GetOutputStream(void) const;
    const CcliOutputDevice& GetErrorStream(void) const;

private:
    const std::string m_strKeyword;
    const CcliHelp m_cliHelp;
    CcliCli* m_pcliCli;
};

#endif // _CLI_ELEMENT_H_
