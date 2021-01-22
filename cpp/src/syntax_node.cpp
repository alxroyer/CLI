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


#include "cli/syntax_node.h"
#include "cli/syntax_tag.h"
#include "cli/param.h"

using namespace cli;


SyntaxNode::SyntaxNode(const std::string& STR_Keyword, const Help& CLI_Help)
  : Element(STR_Keyword, CLI_Help)
{
}

SyntaxNode::~SyntaxNode(void)
{
    for (   ElementMap::const_iterator it = m_cliElements.begin();
            it != m_cliElements.end();
            it ++)
    {
        delete it->second;
    }
    m_cliElements.clear();
}

Element& SyntaxNode::AddElement(Element* const PCLI_Element)
{
    ElementMap::const_iterator it = m_cliElements.find(PCLI_Element->GetKeyword());
    if (it != m_cliElements.end())
    {
        // Conflicting names.
        // Caution!
        // The behaviour is the following: deletion of the new element,
        // and retrieval of the element with the same name.
        // This could be convenient if both elements are keywords for instance,
        // but there is absolutely no guarantee for any other conditions.
        delete PCLI_Element;
        return *const_cast<Element*>(it->second);
    }
    else
    {
        PCLI_Element->SetCli(GetCli());
        m_cliElements[PCLI_Element->GetKeyword()] = PCLI_Element;
        return *PCLI_Element;
    }
}

const bool SyntaxNode::FindElements(
        ElementList& CLI_ExactList,
        ElementList& CLI_NearList,
        const char* const STR_Keyword
        ) const
{
    for (   ElementMap::const_iterator it = m_cliElements.begin();
            it != m_cliElements.end();
            it ++)
    {
        const Element* const pcli_Element = it->second;

        if (0) {}
        else if (const SyntaxTag* const pcli_Tag = dynamic_cast<const SyntaxTag*>(pcli_Element))
        {
            if (! pcli_Tag->GetbHollow())
            {
                if (! pcli_Tag->FindElements(CLI_ExactList, CLI_NearList, STR_Keyword))
                {
                    return false;
                }
            }
        }
        else if (const SyntaxRef* const pcli_Ref = dynamic_cast<const SyntaxRef*>(pcli_Element))
        {
            if (! pcli_Ref->GetTag().FindElements(CLI_ExactList, CLI_NearList, STR_Keyword))
            {
                return false;
            }
        }
        else if (STR_Keyword == NULL)
        {
            // No keyword begun.
            // Retrieve all sub-elements.
            CLI_NearList.push_back(pcli_Element);
        }
        else
        {
            // A beginning of word has been given.
            const std::string str_Keyword(STR_Keyword);

            if (const Param* const pcli_Param = dynamic_cast<const Param*>(pcli_Element))
            {
                // Just check SetstrValue() works for parameters.
                if (str_Keyword != "\n")
                {
                    if (pcli_Param->SetstrValue(str_Keyword))
                    {
                        CLI_ExactList.push_back(pcli_Param);
                        CLI_NearList.push_back(pcli_Param);
                    }
                }
            }
            else if (pcli_Element->GetKeyword().compare(0, str_Keyword.size(), str_Keyword) == 0)
            {
                // Check the beginning of the word for other elements.
                CLI_NearList.push_back(pcli_Element);
                if (str_Keyword.size() == pcli_Element->GetKeyword().size())
                {
                    CLI_ExactList.push_back(pcli_Element);
                }
            }
        }
    }

    return true;
}

