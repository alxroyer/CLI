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

#include "cli/param.h"

using namespace cli;


Param::Param(const std::string& STR_Keyword, const Help& CLI_Help)
  : SyntaxNode(STR_Keyword, CLI_Help),
    m_pcliCloned(NULL)
{
}

Param::~Param(void)
{
}

const std::string Param::GetKeyword(void) const
{
    if (! m_strValue.empty())
    {
        return GetstrValue();
    }
    else
    {
        return SyntaxNode::GetKeyword();
    }
}

const bool Param::FindElements(
        ElementList& CLI_ExactList,
        ElementList& CLI_NearList,
        const char* const STR_Keyword
        ) const
{
    if (GetCloned() != NULL)
    {
        return GetCloned()->FindElements(CLI_ExactList, CLI_NearList, STR_Keyword);
    }
    else
    {
        return SyntaxNode::FindElements(CLI_ExactList, CLI_NearList, STR_Keyword);
    }
}

const std::string Param::GetstrValue(void) const
{
    return m_strValue;
}

const Param* const Param::GetCloned(void) const
{
    return m_pcliCloned;
}

void Param::SetValue(const std::string& STR_Value) const
{
    m_strValue = STR_Value;
}

const Param* const Param::InitClone(Param& CLI_Param) const
{
    CLI_Param.SetCli(const_cast<Cli&>(GetCli()));
    CLI_Param.SetstrValue(GetstrValue());
    CLI_Param.SetCloned(*this);
    return & CLI_Param;
}

void Param::SetCloned(const Param& CLI_Cloned)
{
    m_pcliCloned = & CLI_Cloned;
}

