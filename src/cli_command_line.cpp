/*
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006 Alexis Royer.

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


#include "cli_command_line.h"
#include "cli_io_device.h"
#include "cli_menu.h"
#include "cli_syntax_node.h"
#include "cli_endl.h"
#include "cli_param.h"


CcliCommandLine::CcliCommandLine(void)
  : m_pcliMenu(NULL),
    m_iNumBackspacesForCompletion(0)
{
}

CcliCommandLine::~CcliCommandLine(void)
{
    while (! m_cliAutoDelete.empty())
    {
        const CcliElement* pcli_Front = m_cliAutoDelete.front();
	m_cliAutoDelete.pop_front();
	delete pcli_Front;
    }
}

const bool CcliCommandLine::Parse(
        const CcliMenu& CLI_Menu,
        const std::string& STR_Line,
        const bool B_Execution
        )
{
    m_pcliMenu = & CLI_Menu;

    // Reset the error message.
    m_strError.erase();

    // Split the line into words.
    const CcliElement* pcli_Node = & CLI_Menu;
    std::vector<std::string> vstr_Words;
    int i_LastWordPosition = -1;
    if (! Split(STR_Line, vstr_Words, i_LastWordPosition))
    {
        return false;
    }

    // Determine whether the last word should be parsed.
    int i_WordCount = vstr_Words.size();
    if ((! B_Execution)
        && (! vstr_Words.empty())
        && (i_LastWordPosition >= 0))
    {
        i_WordCount --;
        m_strLastWord = vstr_Words.back();
        m_iNumBackspacesForCompletion = (STR_Line.size() - i_LastWordPosition);
    }
    else
    {
        m_strLastWord.erase();
        m_iNumBackspacesForCompletion = 0;
    }

    // For each word, match the right element.
    for (int i=0; i<i_WordCount; i++)
    {
        CcliElementList cli_ExactList, cli_NearList;
        if (! pcli_Node->FindElements(cli_ExactList, cli_NearList, vstr_Words[i]))
	{
            m_strError =  "Internal error";
            return false;
	}

        if (cli_NearList.size() == 0)
        {
            if (vstr_Words[i] == "\n")
            {
                if (i == 0)
                {
                    return true;
                }
                else
                {
                    m_strError = "Uncomplete command";
                    return false;
                }
            }
            else
            {
                m_strError = "Syntax error next to " + vstr_Words[i];
                return false;
            }
        }
        else if ((cli_ExactList.size() > 1)
                || ((cli_ExactList.size() == 0) && (cli_NearList.size() > 1)))
        {
            m_strError = "Ambiguous syntax next to " + vstr_Words[i];
            return false;
        }
        else
        {
            pcli_Node = cli_NearList.front();
            AddElement(pcli_Node);
        }
    }

    if (B_Execution && (i_WordCount > 0)
        && (! dynamic_cast<const CcliEndl*>(& GetLastElement())))
    {
        m_strError = "Uncomplete command";
        return false;
    }

    return true;
}

const CcliElement& CcliCommandLine::operator[](const int I_Pos) const
{
    return *m_cliElements[I_Pos];
}

const CcliElement& CcliCommandLine::GetLastElement(void) const
{
    if (m_cliElements.empty())
    {
        return *m_pcliMenu;
    }
    else
    {
        return *m_cliElements.back();
    }
}

const int CcliCommandLine::GetElementCount(void) const
{
    return m_cliElements.size();
}

const std::string CcliCommandLine::GetLastWord(void) const
{
    return m_strLastWord;
}

const int CcliCommandLine::GetNumBackspacesForCompletion(void) const
{
    return m_iNumBackspacesForCompletion;
}

const std::string CcliCommandLine::GetLastError(void) const
{
    return m_strError;
}

const bool CcliCommandLine::Split(
        const std::string& STR_Line,
        std::vector<std::string>& VSTR_Words,
        int& I_LastWordPosition
        )
{
    I_LastWordPosition = -1;

    std::string str_Word;
    bool b_EscapeMode = false;
    bool b_QuotedWord = false;
    for (int i=0; i<STR_Line.size(); i++)
    {
        const char c = STR_Line[i];
        bool b_PushWord = false;

        // End of line.
        if (c == '\n')
        {
            if (b_EscapeMode)
            {
                m_strError = "Unterminated escape sequence";
                return false;
            }
            if (b_QuotedWord)
            {
                m_strError = "Unterminated quoted string";
                return false;
            }
            if (! str_Word.empty())
            {
                b_PushWord = true;
                I_LastWordPosition = -1;
            }
        }
        // Quote
        else if ((c == '"') && (! b_EscapeMode))
        {
            b_QuotedWord = (! b_QuotedWord);
            if (I_LastWordPosition < 0) { I_LastWordPosition = i; }
        }
        // Blank characters.
        else if (((c == ' ') || (c == '\t')) && (! b_EscapeMode) && (! b_QuotedWord))
        {
            if (! str_Word.empty())
            {
                b_PushWord = true;
                I_LastWordPosition = -1;
            }
        }
        // Escape character.
        else if ((c == '\\') && (! b_EscapeMode) && (i < STR_Line.size() - 1))
        {
            b_EscapeMode = true;
            if (I_LastWordPosition < 0) { I_LastWordPosition = i; }
        }
        // Non blank character.
        else
        {
            // Word expansion.
            str_Word += c;
            if (I_LastWordPosition < 0) { I_LastWordPosition = i; }

            // Escape mode reset.
            b_EscapeMode = false;
        }

        // End of line management.
        if (i >= STR_Line.size() - 1)
        {
            b_PushWord = true;
        }

        // Push the word if necessary.
        if (b_PushWord && (! str_Word.empty()))
        {
            VSTR_Words.push_back(str_Word);
            str_Word.erase();
        }
        if (c == '\n')
        {
            I_LastWordPosition = -1;
            VSTR_Words.push_back("\n");
            return true;
        }
    }

    if (b_EscapeMode)
    {
        m_strError = "Unterminated escape sequence";
        return false;
    }
    if (b_QuotedWord)
    {
        m_strError = "Unterminated quoted string";
        return false;
    }
    return true;
}

CcliCommandLine& CcliCommandLine::AddElement(const CcliElement* const PCLI_Element)
{
    if (const CcliParam* pcli_Param = dynamic_cast<const CcliParam*>(PCLI_Element))
    {
        const CcliParam* pcli_Clone = pcli_Param->Clone();
	m_cliElements.push_back(pcli_Clone);
	m_cliAutoDelete.push_back(pcli_Clone);
    }
    else
    {
        m_cliElements.push_back(PCLI_Element);
    }
    return *this;
}

