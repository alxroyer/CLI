/*
    Copyright (c) 2006-2007, Alexis Royer

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
        * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

