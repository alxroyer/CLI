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


#include "cli/syntax_tag.h"
#include "cli/help.h"

using namespace cli;


SyntaxTag::SyntaxTag(const bool B_Hollow)
  : SyntaxNode("", Help()),
    m_bHollow(B_Hollow)
{
}

SyntaxTag::~SyntaxTag(void)
{
}

const std::string SyntaxTag::GetKeyword(void) const
{
    static char str_Buffer[256];
    memset(str_Buffer, '\0', sizeof(str_Buffer));
    snprintf(str_Buffer, sizeof(str_Buffer) - 1, "tag[0x%08x]", (unsigned int) this);
    return str_Buffer;
}

const bool SyntaxTag::GetbHollow(void) const
{
    return m_bHollow;
}


SyntaxRef::SyntaxRef(const SyntaxTag& CLI_Tag)
  : Element("", Help()),
    m_pcliTag(& CLI_Tag)
{
}

SyntaxRef::~SyntaxRef(void)
{
}

const std::string SyntaxRef::GetKeyword(void) const
{
    static char str_Buffer[256];
    memset(str_Buffer, '\0', sizeof(str_Buffer));
    snprintf(str_Buffer, sizeof(str_Buffer) - 1, "ref[0x%08x]", (unsigned int) this);

    assert(m_pcliTag != NULL);
    return (std::string(str_Buffer) + std::string(" -> ") + m_pcliTag->GetKeyword());
}

const SyntaxTag& SyntaxRef::GetTag(void) const
{
    assert(m_pcliTag != NULL);
    return *m_pcliTag;
}

