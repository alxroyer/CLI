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


#include "cli_command_line.h"
#include "cli_io_device.h"
#include "cli_menu.h"
#include "cli_syntax_node.h"
#include "cli_endl.h"
#include "cli_param.h"


CcliCommandLine::CcliCommandLine(void)
{
    m_pcliMenu = NULL;
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

bool CcliCommandLine::Parse(const CcliMenu& CLI_Menu, const std::string& STR_Line, bool B_Execution)
{
    m_pcliMenu = & CLI_Menu;

    // Split the line into words.
    const CcliElement* pcli_Node = & CLI_Menu;
    std::vector<std::string> vstr_Words = Split(STR_Line);

    // Determine whether the last word should be parsed.
    int i_WordCount = vstr_Words.size();
    if ((! B_Execution)
        && (! vstr_Words.empty())
        && (STR_Line.compare(
            STR_Line.size() - vstr_Words.back().size(),
            vstr_Words.back().size(),
            vstr_Words.back()) == 0))
    {
        i_WordCount --;
        m_strLastWord = vstr_Words.back();
    }
    else
    {
        m_strLastWord.erase();
    }

    // For each word, match the right element.
    for (int i=0; i<i_WordCount; i++)
    {
        CcliElementList cli_ExactList, cli_NearList;
        if (! pcli_Node->FindElements(cli_ExactList, cli_NearList, vstr_Words[i]))
	{
            CLI_Menu.GetErrorStream() << "Internal error" << CLI_ENDL;
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
                    CLI_Menu.GetErrorStream() << "Uncomplete command" << CLI_ENDL;
                    return false;
                }
            }
            else
            {
                CLI_Menu.GetErrorStream() << "Syntax error next to " << vstr_Words[i] << CLI_ENDL;
                return false;
            }
        }
        else if ((cli_ExactList.size() > 1)
                || ((cli_ExactList.size() == 0) && (cli_NearList.size() > 1)))
        {
            CLI_Menu.GetErrorStream() << "Ambiguous syntax next to " << vstr_Words[i] << CLI_ENDL;
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
        CLI_Menu.GetErrorStream() << "Uncomplete command" << CLI_ENDL;
        return false;
    }

    return true;
}

const CcliElement& CcliCommandLine::operator[](int I_Pos) const
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

int CcliCommandLine::GetElementCount(void) const
{
    return m_cliElements.size();
}

std::string CcliCommandLine::GetLastWord(void) const
{
    return m_strLastWord;
}

std::vector<std::string> CcliCommandLine::Split(const std::string& STR_Line)
{
    std::vector<std::string> vstr_Words;

    std::string str_Word;
    for (int i=0; i<STR_Line.size(); i++)
    {
        if ((STR_Line[i] != ' ') && (STR_Line[i] != '\n') && (STR_Line[i] != '\t'))
        {
            str_Word += STR_Line[i];
        }

        if ((STR_Line[i] == ' ') || (STR_Line[i] == '\n') && (STR_Line[i] == '\t')
            || (i == STR_Line.size() - 1))
        {
            if (! str_Word.empty())
            {
                vstr_Words.push_back(str_Word);
                str_Word.erase();
            }
        }
        if (STR_Line[i] == '\n')
        {
            vstr_Words.push_back("\n");
            break;
        }
    }

    return vstr_Words;
}

CcliCommandLine& CcliCommandLine::AddElement(const CcliElement* PCLI_Element)
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

