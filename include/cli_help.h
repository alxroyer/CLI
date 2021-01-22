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


#ifndef _CLI_HELP_H_
#define _CLI_HELP_H_

#include <string>
#include <map>


class CcliHelp
{
public:
    CcliHelp(void);
    CcliHelp(const CcliHelp& CLI_Help);
    virtual ~CcliHelp(void);

public:
    typedef enum
    {
        CLI_LANG_EN,
        CLI_LANG_FR,
	CLI_LANG_COUNT
    } CLI_LANG;

public:
    CcliHelp& AddHelp(
        CLI_LANG E_Lang,            /**< Language identifier. */
        const std::string& STR_Help /**< Help string. */
        );

    bool HasHelp(CLI_LANG E_Lang) const;
    std::string GetHelp(CLI_LANG E_Lang) const;

private:
    typedef std::map<CLI_LANG, std::string> CcliHelpMap;
    CcliHelpMap m_mstrHelps;
};

#endif // _CLI_HELP_H_
