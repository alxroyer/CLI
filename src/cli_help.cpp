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


#include "cli_help.h"


CcliHelp::CcliHelp(void)
{
}

CcliHelp::CcliHelp(const CcliHelp& CLI_Help)
  : m_mstrHelps(CLI_Help.m_mstrHelps)
{
}

CcliHelp::~CcliHelp(void)
{
}

CcliHelp& CcliHelp::AddHelp(CcliHelp::CLI_LANG E_Lang, const std::string& STR_Help)
{
    m_mstrHelps[E_Lang] = STR_Help;
    return *this;
}

bool CcliHelp::HasHelp(CcliHelp::CLI_LANG E_Lang) const
{
    CcliHelpMap::const_iterator it = m_mstrHelps.find(E_Lang);
    return (it != m_mstrHelps.end());
}

std::string CcliHelp::GetHelp(CcliHelp::CLI_LANG E_Lang) const
{
    CcliHelpMap::const_iterator it = m_mstrHelps.find(E_Lang);
    if (it != m_mstrHelps.end())
    {
        return it->second;
    }
    else
    {
        it = m_mstrHelps.begin();
        if (it != m_mstrHelps.end())
        {
            return it->second;
        }
        else
        {
            return "";
        }
    }
}
