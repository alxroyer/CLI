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


#include "cli/command_line.h"
#include "cli/io_device.h"
#include "cli/menu.h"
#include "cli/syntax_node.h"
#include "cli/endl.h"
#include "cli/param.h"
#include "cli/traces.h"
#include "consistency.h"

using namespace cli;


static const TraceClass TRACE_CMD_LINE("CMD_LINE", "Command line parsing");
static const TraceClass TRACE_CMD_LINE_SPLIT("CMD_LINE_SPLIT", "Split of command lines");


CommandLine::CommandLine(void)
  : m_pcliMenu(NULL),
    m_bLastWordValid(false),
    m_iNumBackspacesForCompletion(0)
{
    EnsureTraces();
}

CommandLine::~CommandLine(void)
{
    while (! m_cliAutoDelete.empty())
    {
        const Element* pcli_Front = m_cliAutoDelete.front();
        m_cliAutoDelete.pop_front();
        delete pcli_Front;
    }
}

const bool CommandLine::Parse(
        const Menu& CLI_Menu,
        const std::string& STR_Line,
        const bool B_Execution
        )
{
    m_pcliMenu = & CLI_Menu;

    // Reset the error message.
    m_strError.erase();

    // Split the line into words.
    const Element* pcli_Node = & CLI_Menu;
    std::vector<std::string> vstr_Words;
    int i_LastWordPosition = -1;
    if (! Split(STR_Line, vstr_Words, i_LastWordPosition))
    {
        GetTraces().Trace(TRACE_CMD_LINE) << "Could not split '" << STR_Line << "' (" << m_strError << ")" << endl;
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
        m_bLastWordValid = true;
        m_iNumBackspacesForCompletion = (STR_Line.size() - i_LastWordPosition);
    }
    else
    {
        m_strLastWord.erase();
        m_bLastWordValid = false;
        m_iNumBackspacesForCompletion = 0;
    }

    // For each word, match the right element.
    for (int i=0; i<i_WordCount; i++)
    {
        GetTraces().Trace(TRACE_CMD_LINE) << "Word " << i << " '" << vstr_Words[i] << "'" << endl;
        ElementList cli_ExactList, cli_NearList;
        if (! pcli_Node->FindElements(cli_ExactList, cli_NearList, vstr_Words[i].c_str()))
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
                    GetTraces().Trace(TRACE_CMD_LINE) << "Uncomplete command" << endl;
                    m_strError = "Uncomplete command";
                    return false;
                }
            }
            else
            {
                GetTraces().Trace(TRACE_CMD_LINE) << "Syntax error next to " << vstr_Words[i] << endl;
                m_strError = "Syntax error next to " + vstr_Words[i];
                return false;
            }
        }
        else if ((cli_ExactList.size() > 1)
                || ((cli_ExactList.size() == 0) && (cli_NearList.size() > 1)))
        {
            GetTraces().Trace(TRACE_CMD_LINE) << "Ambiguous syntax next to " << vstr_Words[i] << endl;
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
        && (! dynamic_cast<const Endl*>(& GetLastElement())))
    {
        GetTraces().Trace(TRACE_CMD_LINE) << "Uncomplete command" << endl;
        m_strError = "Uncomplete command";
        return false;
    }

    if (GetLastWord() != NULL)
    {
        GetTraces().Trace(TRACE_CMD_LINE) << "Last word '" << GetLastWord() << "'..." << endl;
    }
    return true;
}

const Element& CommandLine::operator[](const int I_Pos) const
{
    return *m_cliElements[I_Pos];
}

const Element& CommandLine::GetLastElement(void) const
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

const int CommandLine::GetElementCount(void) const
{
    return m_cliElements.size();
}

const char* const CommandLine::GetLastWord(void) const
{
    if (m_bLastWordValid)
    {
        return m_strLastWord.c_str();
    }
    else
    {
        return NULL;
    }
}

const int CommandLine::GetNumBackspacesForCompletion(void) const
{
    return m_iNumBackspacesForCompletion;
}

const std::string CommandLine::GetLastError(void) const
{
    return m_strError;
}

const bool CommandLine::Split(
        const std::string& STR_Line,
        std::vector<std::string>& VSTR_Words,
        int& I_LastWordPosition
        )
{
    I_LastWordPosition = -1;

    class Do { public:
        static void PushWord(
            const Element& CLI_Element, std::vector<std::string>& VSTR_Words,
            const int I_Position, std::string& STR_Word)
        {
            GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Word '" << STR_Word << "' pushed at " << I_Position << endl;
            VSTR_Words.push_back(STR_Word);
            STR_Word.erase();
        }
    };

    std::string str_Word;
    bool b_EscapeMode = false;
    bool b_QuotedWord = false;
    for (unsigned int i=0; i<STR_Line.size(); i++)
    {
        const char c = STR_Line[i];

        // End of line.
        if (c == '\n')
        {
            if (b_EscapeMode)
            {
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Unterminated escape sequence on \\n" << endl;
                m_strError = "Unterminated escape sequence";
                return false;
            }
            if (b_QuotedWord)
            {
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Unterminated quoted string on \\n" << endl;
                m_strError = "Unterminated quoted string";
                return false;
            }
            if (! str_Word.empty())
            {
                Do::PushWord(*m_pcliMenu, VSTR_Words, i, str_Word);
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "LastWordPosition set to -1 on \\n" << endl;
                I_LastWordPosition = -1;
            }
        }
        // Quote
        else if ((c == '"') && (! b_EscapeMode))
        {
            if (! b_QuotedWord)
            {
                // Beginning of quoted string.
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Quoted string starting at " << i << endl;
                b_QuotedWord = true;
                // Push previous word if any.
                if (! str_Word.empty())
                {
                    Do::PushWord(*m_pcliMenu, VSTR_Words, i, str_Word);
                }
                // Completion management.
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "LastWordPosition set to " << i << " on \"" << endl;
                I_LastWordPosition = i;
            }
            else
            {
                // End of quoted string.
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Quoted string ending at " << i << endl;
                b_QuotedWord = false;
                // Push the word whatever.
                Do::PushWord(*m_pcliMenu, VSTR_Words, i, str_Word);
                // No completion on quoted strings.
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "LastWordPosition set to -1 on \"" << endl;
                I_LastWordPosition = -1;
            }
        }
        // Blank characters.
        else if (((c == ' ') || (c == '\t')) && (! b_EscapeMode) && (! b_QuotedWord))
        {
            if (! str_Word.empty())
            {
                Do::PushWord(*m_pcliMenu, VSTR_Words, i, str_Word);
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "LastWordPosition set to -1 on blank character" << endl;
                I_LastWordPosition = -1;
            }
        }
        // Escape character.
        else if ((c == '\\') && (! b_EscapeMode) && (i < STR_Line.size() - 1))
        {
            GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Escape mode starting at " << i << endl;
            b_EscapeMode = true;
            if (I_LastWordPosition < 0)
            {
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "LastWordPosition set to " << i << " on \\" << endl;
                I_LastWordPosition = i;
            }
        }
        // Non blank character.
        else
        {
            // Word expansion.
            str_Word += c;
            if (I_LastWordPosition < 0)
            {
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "LastWordPosition set to " << i << " on non-blank character" << endl;
                I_LastWordPosition = i;
            }

            // Escape mode reset.
            if (b_EscapeMode)
            {
                GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Escape mode ending at " << i << endl;
                b_EscapeMode = false;
            }
        }

        // End of line management.
        if (i >= STR_Line.size() - 1)
        {
            if (! str_Word.empty())
            {
                Do::PushWord(*m_pcliMenu, VSTR_Words, i, str_Word);
            }
        }

        if (c == '\n')
        {
            GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "\\n pushed at " << i << endl;
            VSTR_Words.push_back("\n");
            GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "LastWordPosition set to -1 on \\n" << endl;
            I_LastWordPosition = -1;
            return true;
        }
    }

    if (b_EscapeMode)
    {
        GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Unterminated escape sequence on end of line" << endl;
        m_strError = "Unterminated escape sequence";
        return false;
    }
    if (b_QuotedWord)
    {
        GetTraces().Trace(TRACE_CMD_LINE_SPLIT) << "Unterminated quoted string on end of line" << endl;
        m_strError = "Unterminated quoted string";
        return false;
    }
    return true;
}

CommandLine& CommandLine::AddElement(const Element* const PCLI_Element)
{
    if (const Param* pcli_Param = dynamic_cast<const Param*>(PCLI_Element))
    {
        const Param* pcli_Clone = pcli_Param->Clone();
        m_cliElements.push_back(pcli_Clone);
        m_cliAutoDelete.push_back(pcli_Clone);
    }
    else
    {
        m_cliElements.push_back(PCLI_Element);
    }
    return *this;
}

