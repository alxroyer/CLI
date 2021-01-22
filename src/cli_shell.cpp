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


#include <stdio.h>
#include <assert.h>

#include "cli_shell.h"
#include "cli_io_device.h"
#include "cli_console.h"
#include "cli_telnet.h"
#include "cli_cli.h"
#include "cli_menu.h"
#include "cli_command_line.h"
#include "cli_endl.h"
#include "cli_param.h"


static const int CLI_HELP_MARGIN = 2;
static const int CLI_HELP_OFFSET = 15;
static const int CLI_HISTORY_PAGE = 5;
static const int CLI_HISTORY_STACK_SIZE = 100;


CcliShell::CcliShell(const CcliCli& CLI_Cli)
  : m_pcliCli(& CLI_Cli),
    m_eLang(CcliHelp::CLI_LANG_EN), m_bBeep(true),
    m_iHistoryIndex(0)
{
    m_vpcliMenus.push_back(& CLI_Cli);
    CLI_Cli.SetShell(*this);

    for (int i=0; i<CLI_STREAM_TYPES_COUNT; i++)
    {
        m_arpcliStream[i] = NULL;
    }

    m_vstrHistory.insert(m_vstrHistory.begin(), m_strLine);
}

CcliShell::~CcliShell(void)
{
}

const CcliCli& CcliShell::GetCli(void) const
{
    assert(m_pcliCli != NULL);
    return *m_pcliCli;
}

const CcliOutputDevice& CcliShell::GetWelcomeStream(void) const
{
    if (m_arpcliStream[CLI_WELCOME_STREAM] != NULL)
    {
        return *m_arpcliStream[CLI_WELCOME_STREAM];
    }
    else
    {
        return CcliOutputDevice::GetStdOut();
    }
}

void CcliShell::SetWelcomeStream(const CcliOutputDevice& CLI_WelcomeStream)
{
    m_arpcliStream[CLI_WELCOME_STREAM] = & CLI_WelcomeStream;
}

void CcliShell::SetWelcomeMessage(const std::string& STR_WelcomeMessage)
{
    m_strWelcomeMessage = STR_WelcomeMessage;
}

void CcliShell::SetByeMessage(const std::string& STR_ByeMessage)
{
    m_strByeMessage = STR_ByeMessage;
}

const CcliOutputDevice& CcliShell::GetPromptStream(void) const
{
    if (m_arpcliStream[CLI_PROMPT_STREAM] != NULL)
    {
        return *m_arpcliStream[CLI_PROMPT_STREAM];
    }
    else
    {
        return CcliOutputDevice::GetStdOut();
    }
}

void CcliShell::SetPromptStream(const CcliOutputDevice& CLI_PromptStream)
{
    m_arpcliStream[CLI_PROMPT_STREAM] = & CLI_PromptStream;
}

void CcliShell::SetPrompt(const std::string& STR_Prompt)
{
    m_strNoDefaultPrompt = STR_Prompt;
}

const CcliOutputDevice& CcliShell::GetEchoStream(void) const
{
    if (m_arpcliStream[CLI_ECHO_STREAM] != NULL)
    {
        return *m_arpcliStream[CLI_ECHO_STREAM];
    }
    else
    {
        return CcliOutputDevice::GetStdOut();
    }
}

void CcliShell::SetEchoStream(const CcliOutputDevice& CLI_EchoStream)
{
    m_arpcliStream[CLI_ECHO_STREAM] = & CLI_EchoStream;
}

const CcliOutputDevice& CcliShell::GetOutputStream(void) const
{
    if (m_arpcliStream[CLI_OUTPUT_STREAM] != NULL)
    {
        return *m_arpcliStream[CLI_OUTPUT_STREAM];
    }
    else
    {
        return CcliOutputDevice::GetStdOut();
    }
}

void CcliShell::SetOutputStream(const CcliOutputDevice& CLI_OutputStream)
{
    m_arpcliStream[CLI_OUTPUT_STREAM] = & CLI_OutputStream;
}

const CcliOutputDevice& CcliShell::GetErrorStream(void) const
{
    if (m_arpcliStream[CLI_ERROR_STREAM] != NULL)
    {
        return *m_arpcliStream[CLI_ERROR_STREAM];
    }
    else
    {
        return CcliOutputDevice::GetStdErr();
    }
}

void CcliShell::SetErrorStream(const CcliOutputDevice& CLI_ErrorStream)
{
    m_arpcliStream[CLI_ERROR_STREAM] = & CLI_ErrorStream;
}

void CcliShell::SetLang(const CcliHelp::CLI_LANG E_Lang)
{
    m_eLang = E_Lang;
}

const CcliHelp::CLI_LANG CcliShell::GetLang(void) const
{
    return m_eLang;
}

void CcliShell::SetBeep(const bool B_Enable)
{
    m_bBeep = B_Enable;
}

const bool CcliShell::GetBeep(void) const
{
    return m_bBeep;
}

void CcliShell::Run(CcliIODevice& CLI_IODevice)
{
    if (CLI_IODevice.OpenDevice())
    {
        // Stream setting.
        for (int i=0; i<CLI_STREAM_TYPES_COUNT; i++)
        {
            if (m_arpcliStream[i] == NULL)
            {
                m_arpcliStream[i] = & CLI_IODevice;
            }
        }

        if (! m_vpcliMenus.empty())
        {
            // Remember the CLI reference.
            const CcliCli* const pcli_Cli = dynamic_cast<const CcliCli*>(m_vpcliMenus[0]);

            PromptWelcomeMessage();
            PromptMenu();
            while (! m_vpcliMenus.empty())
            {
                CLI_CHAR e_Key = CLI_IODevice.GetChar();
                OnKey(e_Key);
            }
            PromptByeMessage();

            // Unsetting streams.
            for (int i=0; i<CLI_STREAM_TYPES_COUNT; i++)
            {
                if (m_arpcliStream[i] == & CLI_IODevice)
                {
                    m_arpcliStream[i] = NULL;
                }
            }
            // Restore the CLI reference.
            m_vpcliMenus.push_back(pcli_Cli);
        }
        else
        {
            assert(false);
            GetErrorStream() << "No current menu!" << CLI_ENDL;
        }
    }
    CLI_IODevice.CloseDevice();
}

void CcliShell::PromptWelcomeMessage(void) const
{
    if (! m_strWelcomeMessage.empty())
    {
        GetWelcomeStream() << m_strWelcomeMessage;
    }
    else
    {
        GetWelcomeStream()
        << "---------------------------------------------------" << CLI_ENDL
        << " Welcome to " << GetCli().GetKeyword() << "!" << CLI_ENDL
        << CLI_ENDL
        << " " << GetCli().GetKeyword() << " is a command line interface" << CLI_ENDL
        << " using the LGPL CLI library" << CLI_ENDL
        << "   (c) Alexis Royer http://alexis.royer.free.fr/CLI/" << CLI_ENDL
        << " Type 'help' at any time" << CLI_ENDL
        << " or press '?' or TAB to get completion or help." << CLI_ENDL
        << "---------------------------------------------------" << CLI_ENDL;
    }
}

void CcliShell::PromptByeMessage(void) const
{
    if (! m_strByeMessage.empty())
    {
        GetWelcomeStream() << m_strByeMessage;
    }
    else
    {
        GetWelcomeStream() << "Bye!" << CLI_ENDL;
    }
}

void CcliShell::PromptMenu(void) const
{
    if (! m_strNoDefaultPrompt.empty())
    {
        GetPromptStream() << m_strNoDefaultPrompt << m_strLine;
    }
    else
    {
        if (! m_vpcliMenus.empty())
        {
            GetPromptStream() << m_vpcliMenus.back()->GetKeyword() << ">" << m_strLine;
        }
    }
}

void CcliShell::ExitMenu(void)
{
    std::string str_Mem = m_strLine;
    m_strLine.erase();
    OnExit(false);
    m_strLine = str_Mem;
}

void CcliShell::Quit(void)
{
    while (! m_vpcliMenus.empty())
    {
        ExitMenu();
    }
}

void CcliShell::DisplayHelp(void)
{
    if (! m_vpcliMenus.empty())
    {
        std::string str_Mem = m_strLine;
        m_strLine.erase();
        OnHelp(false, true);
        m_strLine = str_Mem;
    }
    else
    {
        assert(false);
        GetErrorStream() << "No current menu!" << CLI_ENDL;
    }
}

void CcliShell::PrintWorkingMenu(void)
{
    int i = 0;
    for (i=0; i<CLI_HELP_MARGIN; i++)
    {
        GetOutputStream() << " ";
    }
    for (i=0; i<m_vpcliMenus.size(); i++)
    {
        GetOutputStream() << "/" << m_vpcliMenus[i]->GetKeyword();
    }
    GetOutputStream() << CLI_ENDL;
}

void CcliShell::EnterMenu(const CcliMenu& CLI_Menu)
{
    m_vpcliMenus.push_back(& CLI_Menu);
    PromptMenu();
}

void CcliShell::OnKey(const CLI_CHAR E_KeyCode)
{
    // In general, history navigation is lost.
    const int i_HistoryIndexBackup = m_iHistoryIndex;
    m_iHistoryIndex = 0;

    switch (E_KeyCode)
    {
    case CLI_KEY_UP:    m_iHistoryIndex = i_HistoryIndexBackup;
                        OnHistory(1);                   break;
    case CLI_KEY_DOWN:  m_iHistoryIndex = i_HistoryIndexBackup;
                        OnHistory(-1);                  break;
    case CLI_PAGE_UP:   m_iHistoryIndex = i_HistoryIndexBackup;
                        OnHistory(CLI_HISTORY_PAGE);    break;
    case CLI_PAGE_DOWN: m_iHistoryIndex = i_HistoryIndexBackup;
                        OnHistory(-CLI_HISTORY_PAGE);   break;
    case CLI_BACKSPACE: OnBackspace();                  break;
    case CLI_ENTER:     OnExecute();                    break;
    case CLI_BREAK:
    case CLI_ESCAPE:    OnEscape();                     break;
    case CLI_LOGOUT:    OnExit(true);                   break;
    case CLI_TAB:       OnHelp(true, false);            break;
    case CLI_QUESTION:
        if (m_strLine[m_strLine.size() - 1] == '\\')
            OnPrintableChar(E_KeyCode);
        else
            OnHelp(true, true);
        break;

    case CLI_a: case CLI_b: case CLI_c: case CLI_d: case CLI_e: case CLI_f:
    case CLI_g: case CLI_h: case CLI_i: case CLI_j: case CLI_k: case CLI_l:
    case CLI_m: case CLI_n: case CLI_o: case CLI_p: case CLI_q: case CLI_r:
    case CLI_s: case CLI_t: case CLI_u: case CLI_v: case CLI_w: case CLI_x:
    case CLI_y: case CLI_z:

    case CLI_A: case CLI_B: case CLI_C: case CLI_D: case CLI_E: case CLI_F:
    case CLI_G: case CLI_H: case CLI_I: case CLI_J: case CLI_K: case CLI_L:
    case CLI_M: case CLI_N: case CLI_O: case CLI_P: case CLI_Q: case CLI_R:
    case CLI_S: case CLI_T: case CLI_U: case CLI_V: case CLI_W: case CLI_X:
    case CLI_Y: case CLI_Z:

    case CLI_0: case CLI_1: case CLI_2: case CLI_3: case CLI_4: case CLI_5:
    case CLI_6: case CLI_7: case CLI_8: case CLI_9:

    case CLI_PLUS:
    case CLI_MINUS:
    case CLI_STAR:
    case CLI_SLASH:
    case CLI_LOWER_THAN:
    case CLI_GREATER_THAN:
    case CLI_EQUAL:

    case CLI_SPACE:
    case CLI_UNDERSCORE:
    case CLI_AROBASE:
    case CLI_SHARP:
    case CLI_AMPERCENT:
    case CLI_DOLLAR:
    case CLI_BACKSLASH:

    case CLI_EXCLAMATION:
    case CLI_COLUMN:
    case CLI_DOT:
    case CLI_COMA:
    case CLI_SEMI_COLUMN:
    case CLI_QUOTE:
    case CLI_DOUBLE_QUOTE:

    case CLI_OPENING_BRACE:
    case CLI_CLOSING_BRACE:
    case CLI_OPENING_CURLY_BRACE:
    case CLI_CLOSING_CURLY_BRACE:
    case CLI_OPENING_BRACKET:
    case CLI_CLOSING_BRACKET:
        OnPrintableChar(E_KeyCode);
        break;

    default:
        break;
    }
}

void CcliShell::OnPrintableChar(const char C_KeyCode)
{
    PrintLine(C_KeyCode);
}

void CcliShell::OnBackspace(void)
{
    Backspace(1);
}

void CcliShell::OnEscape(void)
{
    m_strLine.erase();
    GetEchoStream() << CLI_ENDL;
    if (! m_vpcliMenus.empty())
    {
        PromptMenu();
    }
    else
    {
        assert(false);
        GetErrorStream() << "No current menu!" << CLI_ENDL;
    }
}

void CcliShell::OnExit(const bool B_Edition)
{
    if (m_strLine.empty())
    {
        if (B_Edition)
        {
            GetEchoStream() << CLI_ENDL;
        }
        if (! m_vpcliMenus.empty())
        {
            m_vpcliMenus.back()->OnExit();
            m_vpcliMenus.pop_back();
        }
        if (B_Edition)
        {
            PromptMenu();
        }
    }
    else
    {
        Beep();
    }
}

void CcliShell::OnHelp(const bool B_Edition, const bool B_HelpOnly)
{
    if (! m_vpcliMenus.empty())
    {
        // Parse the command line.
        CcliCommandLine cli_CommandLine;
        if (cli_CommandLine.Parse(*m_vpcliMenus.back(), m_strLine, false))
        {
            // Research matching elements.
            CcliElementList cli_Exact, cli_Elements;
            if (! cli_CommandLine.GetLastElement().FindElements(
                    cli_Exact, cli_Elements, cli_CommandLine.GetLastWord()))
            {
                return;
            }

            // Determine whether help should be printed, and completion activated.
            bool b_PrintHelpList = false;
            if (B_HelpOnly)
            {
                // Just print help.
                b_PrintHelpList = true;
            }
            else
            {
                // Print help and do completion depending on the possible nodes.
                if (cli_Elements.empty())
                {
                    // No matching element.
                    Beep();
                }
                else if ((cli_Elements.size() == 1)
                        && (! dynamic_cast<const CcliEndl*>(cli_Elements.front()))
                        && (! dynamic_cast<const CcliParam*>(cli_Elements.front())))
                {
                    // Straight forward completion.
                    Backspace(cli_CommandLine.GetNumBackspacesForCompletion());
                    PrintLine(cli_Elements.front()->GetKeyword() + " ");
                }
                else
                {
                    // Several choices, print help.
                    b_PrintHelpList = true;
                }
            }

            // Help display.
            if (b_PrintHelpList)
            {
                // First of all, sort help elements on keywords.
                class _ { public:
                    static const bool cmp(
                            const CcliElement* const PCLI_1,
                            const CcliElement* const PCLI_2)
                    {
                        return (PCLI_1->GetKeyword() < PCLI_2->GetKeyword());
                    }
                };
                std::sort(cli_Elements.begin(), cli_Elements.end(), _::cmp);

                if (B_Edition)
                {
                    GetEchoStream() << CLI_ENDL;
                }

                // Initialize completion so far.
                std::string str_CompletionSoFar;
                bool b_InitCompletionSoFar = true;

                // For each matching keyword...
                for (   CcliElementList::const_iterator it = cli_Elements.begin();
                        it != cli_Elements.end();
                        it ++)
                {
                    if (dynamic_cast<const CcliParam*>(*it)
                        || dynamic_cast<const CcliEndl*>(*it))
                    {
                        // No completion for parameters and end of lines.
                        str_CompletionSoFar.erase();
                        b_InitCompletionSoFar = false;
                    }
                    else
                    {
                        if (b_InitCompletionSoFar)
                        {
                            // First initialization of str_CompletionSoFar.
                            str_CompletionSoFar = (*it)->GetKeyword();
                            b_InitCompletionSoFar = false;
                        }
                        else
                        {
                            // Remove last character of str_CompletionSoFar till it matches the current keyword.
                            while ( (! str_CompletionSoFar.empty())
                                    && ((*it)->GetKeyword().compare(0, str_CompletionSoFar.size(), str_CompletionSoFar) != 0))
                            {
                                str_CompletionSoFar = str_CompletionSoFar.substr(0, str_CompletionSoFar.size() - 1);
                            }
                        }
                    }

                    // Print the corresponding help.
                    PrintHelp(**it);
                }

                if (B_Edition)
                {
                    PromptMenu();

                    // Completion so far.
                    if ((! B_HelpOnly)
                        && (! str_CompletionSoFar.empty()))
                    {
                        Backspace(cli_CommandLine.GetNumBackspacesForCompletion());
                        PrintLine(str_CompletionSoFar);
                    }
                }
            }
        }
        else
        {
            if (B_Edition)
            {
                GetEchoStream() << CLI_ENDL;
            }
            GetErrorStream() << cli_CommandLine.GetLastError() << CLI_ENDL;
            if (B_Edition)
            {
                PromptMenu();
            }
        }
    }
    else
    {
        assert(false);
        GetErrorStream() << "No current menu!" << CLI_ENDL;
    }
}

void CcliShell::OnExecute(void)
{
    GetEchoStream() << CLI_ENDL;

    if (! m_vpcliMenus.empty())
    {
        // First of all, store the line in history stack.
        PushHistory(m_strLine);

        CcliCommandLine cli_CommandLine;
        if (cli_CommandLine.Parse(*m_vpcliMenus.back(), m_strLine + "\n", true))
        {
            bool b_Executed = false;
            if (cli_CommandLine.GetElementCount() > 1)
            {
                if (const CcliMenu* const pcli_Menu = m_vpcliMenus.back())
                {
                    if (pcli_Menu->ExecuteReserved(cli_CommandLine))
                        b_Executed = true;
                    if (pcli_Menu->Execute(cli_CommandLine))
                        b_Executed = true;
                }

                if (b_Executed)
                {
                    if (const CcliEndl* pcli_Endl = dynamic_cast<const CcliEndl*>(& cli_CommandLine.GetLastElement()))
                    {
                        if (pcli_Endl->GetMenuRef() != NULL)
                        {
                            m_vpcliMenus.push_back(& pcli_Endl->GetMenuRef()->GetMenu());
                        }
                    }
                }
                else
                {
                    GetErrorStream() << "Execution error" << CLI_ENDL;
                }
            }
        }
        else
        {
            GetErrorStream() << cli_CommandLine.GetLastError() << CLI_ENDL;
        }
        m_strLine.erase();
        PromptMenu();
    }
    else
    {
        assert(false);
        GetErrorStream() << "No current menu!" << CLI_ENDL;
    }
}

void CcliShell::OnHistory(const int I_Translation)
{
    // First check the translation remains in bounds of the history stack.
    if ((m_iHistoryIndex + I_Translation >= 0)
        && (m_iHistoryIndex + I_Translation < (signed) m_vstrHistory.size()))
    {
        // If this is the current line, save it into the history stack.
        if (m_iHistoryIndex == 0)
        {
            m_vstrHistory[0] = m_strLine;
        }
        // Clean up the current line.
        Backspace(m_strLine.size());
        // Translate the history index.
        m_iHistoryIndex += I_Translation;
        // Print out this line.
        PrintLine(m_vstrHistory[m_iHistoryIndex]);
    }
    else
    {
        // Out of stack.
        Beep();
    }
}

void CcliShell::PrintLine(const std::string& STR_Append)
{
    m_strLine += STR_Append;
    GetEchoStream() << STR_Append;
}

void CcliShell::PrintLine(const char C_Append)
{
    PrintLine(std::string() += C_Append);
}

void CcliShell::Backspace(const int I_BackspaceCount)
{
    int i_BackspaceCount = I_BackspaceCount;
    if (i_BackspaceCount > m_strLine.size())
    {
        Beep();
        i_BackspaceCount = m_strLine.size();
    }

    m_strLine = m_strLine.substr(0, m_strLine.size() - i_BackspaceCount);
    for (int i=0; i<i_BackspaceCount; i++)
    {
        GetEchoStream() << "\b" << " " << "\b";
    }
}

void CcliShell::PrintHelp(const CcliElement& CLI_Element)
{
    // Retrieve basic information.
    int i = 0;
    std::string str_Keyword = CLI_Element.GetKeyword();
    std::string str_Help = CLI_Element.GetHelp().GetHelp(GetLang());

    // Optional corrections.
    if (dynamic_cast<const CcliEndl*>(& CLI_Element))
    {
        str_Keyword = "<cr>";
    }
    else if (const CcliParam* const pcli_Param = dynamic_cast<const CcliParam*>(& CLI_Element))
    {
        if (const CcliParam* const pcli_Clone = pcli_Param->Clone())
        {
            pcli_Clone->SetstrValue("");
            str_Keyword = pcli_Clone->GetKeyword();
            if (! pcli_Param->GetstrValue().empty())
            {
                str_Keyword += ("(" + pcli_Param->GetstrValue() + ")");
            }
            delete pcli_Clone;
        }
    }

    // Display.
    for (i = 0; i < CLI_HELP_MARGIN; i++)
    {
        GetOutputStream() << " ";
    }
    GetOutputStream() << str_Keyword << " ";
    for (i = str_Keyword.size(); i < CLI_HELP_OFFSET; i++)
    {
        GetOutputStream() << " ";
    }
    GetOutputStream() << str_Help;
    GetOutputStream() << CLI_ENDL;
}

void CcliShell::Beep(void)
{
    if (m_bBeep)
    {
        GetErrorStream().Beep();
    }
}

void CcliShell::PushHistory(const std::string& STR_Line)
{
    if (// Check the line is not empty.
        (! STR_Line.empty())
        // Check it is not the same as the previous one.
        && ((m_vstrHistory.size() < 2) || (STR_Line != m_vstrHistory[1])))
    {
        while (m_vstrHistory.size() > CLI_HISTORY_STACK_SIZE)
        {
            m_vstrHistory.pop_back();
        }
        m_vstrHistory[0] = STR_Line;
        m_vstrHistory.insert(m_vstrHistory.begin(), "");
    }
    m_iHistoryIndex = 0;
}
