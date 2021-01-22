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


#ifndef _CLI_CLI_H_
#define _CLI_CLI_H_

#include <string>
#include <vector>

#include "cli_menu.h"

class CcliConsole;
class CcliHelp;


class CcliCli : public CcliMenu
{
public:
    CcliCli(const std::string& STR_Name, const CcliHelp& CLI_Help);
    virtual ~CcliCli(void);

public:
    CcliMenu& AddMenu(CcliMenu* PCLI_Menu);

public:
    void SetShell(CcliShell& CLI_Shell);
    CcliShell& GetShell(void) const;

private:
    CcliShell* m_pcliShell;
    std::vector<CcliMenu*> m_vpcliMenus;
};

#endif // _CLI_CLI_H_
