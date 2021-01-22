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


#include "cli_syntax_tag.h"
#include "cli_help.h"


CcliSyntaxTag::CcliSyntaxTag(void)
  : CcliSyntaxNode("", CcliHelp())
{
}

CcliSyntaxTag::~CcliSyntaxTag(void)
{
}


CcliSyntaxRef::CcliSyntaxRef(const CcliSyntaxTag& CLI_Tag)
  : CcliElement("", CcliHelp()),
    m_pcliTag(& CLI_Tag)
{
}

CcliSyntaxRef::~CcliSyntaxRef(void)
{
}

const CcliSyntaxTag& CcliSyntaxRef::GetTag(void) const
{
    assert(m_pcliTag != NULL);
    return *m_pcliTag;
}

