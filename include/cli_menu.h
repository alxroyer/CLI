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


#ifndef _CLI_MENU_H_
#define _CLI_MENU_H_

#include <string>
#include "cli_syntax_node.h"


class CcliCli;
class CcliConsole;
class CcliCommandLine;
class CcliKeyword;
class CcliHelp;


class CcliMenu : public CcliSyntaxNode
{
public:
    CcliMenu(const std::string& STR_Name, const CcliHelp& CLI_Help);
    virtual ~CcliMenu(void);

public:
    CcliKeyword& GetHelpNode(void);
    const CcliKeyword& GetHelpNode(void) const;
    CcliKeyword& GetExitNode(void);
    const CcliKeyword& GetExitNode(void) const;
    CcliKeyword& GetQuitNode(void);
    const CcliKeyword& GetQuitNode(void) const;
    CcliKeyword& GetPwmNode(void);
    const CcliKeyword& GetPwmNode(void) const;

    virtual void SetCli(CcliCli& CLI_Cli);
    virtual bool ExecuteReserved(const CcliCommandLine& CLI_CommandLine) const;
    virtual bool Execute(const CcliCommandLine& CLI_CommandLine) const;
    virtual void OnExit(void) const;

private:
    const CcliCli* m_pcliCli;
    CcliKeyword* m_pcliHelp;
    CcliKeyword* m_pcliExit;
    CcliKeyword* m_pcliQuit;
    CcliKeyword* m_pcliPwm;
};


class CcliMenuRef : public CcliElement
{
public:
    CcliMenuRef(const CcliMenu& CLI_Menu);
    virtual ~CcliMenuRef(void);

public:
    const CcliMenu& GetMenu(void) const;

protected:
    const CcliMenu* m_pcliMenu;
};


#endif // _CLI_MENU_H_
