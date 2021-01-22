/*
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006-2007 Alexis Royer.

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


#include "cli/help.h"

using namespace cli;


Help::Help(void)
{
}

Help::Help(const Help& CLI_Help)
  : m_mstrHelps(CLI_Help.m_mstrHelps)
{
}

Help::~Help(void)
{
}

Help& Help::AddHelp(const Help::LANG E_Lang, const std::string& STR_Help)
{
    m_mstrHelps[E_Lang] = STR_Help;
    return *this;
}

const bool Help::HasHelp(const Help::LANG E_Lang) const
{
    HelpMap::const_iterator it = m_mstrHelps.find(E_Lang);
    return (it != m_mstrHelps.end());
}

const std::string Help::GetHelp(const Help::LANG E_Lang) const
{
    HelpMap::const_iterator it = m_mstrHelps.find(E_Lang);
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