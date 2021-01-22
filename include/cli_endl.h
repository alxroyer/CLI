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


#ifndef _CLI_ENDL_H_
#define _CLI_ENDL_H_

#include "cli_element.h"


class CcliMenu;
class CcliMenuRef;
class CcliHelp;


class CcliEndl : public CcliElement
{
public:
    CcliEndl(const CcliHelp& CLI_Help);
    virtual ~CcliEndl(void);

public:
    CcliMenu& SetMenu(CcliMenu* PCLI_Menu);
    CcliMenuRef& SetMenuRef(CcliMenuRef* PCLI_MenuRef);
    const CcliMenuRef* GetMenuRef(void) const;

private:
    CcliMenuRef* m_pcliMenuRef;
};

#endif // _CLI_ENDL_H_
