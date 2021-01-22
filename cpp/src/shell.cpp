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


#include <stdio.h>
#include <assert.h>
#include <algorithm>

#include "cli/shell.h"
#include "cli/io_device.h"
#include "cli/io_mux.h"
#include "cli/cli.h"
#include "cli/menu.h"
#include "cli/command_line.h"
#include "cli/endl.h"
#include "cli/param.h"
#include "cli/traces.h"
#include "consistency.h"

using namespace cli;


static const unsigned int HELP_MARGIN = 2;
static const unsigned int HELP_OFFSET = 15;
static const unsigned int HISTORY_PAGE = 5;
static const unsigned int HISTORY_STACK_SIZE = 100;

static const TraceClass TRACE_SHELL("SHELL", "Shell traces");


Shell::Shell(const Cli& CLI_Cli)
  : m_pcliCli(& CLI_Cli), m_pcliInput(NULL),
    m_eLang(Help::LANG_EN), m_bBeep(true),
    m_iHistoryIndex(0)
{
    EnsureCommonDevices();
    EnsureTraces();

    for (int i=0; i<STREAM_TYPES_COUNT; i++)
    {
        m_artStream[i].pcliStream = NULL;
        m_artStream[i].bEnable = true;
    }

    m_vstrHistory.insert(m_vstrHistory.begin(), m_strLine);

    m_vpcliMenus.push_back(& CLI_Cli);
    CLI_Cli.SetShell(*this);

    GetTraces().Trace(TRACE_SHELL) << "New shell for CLI '" << GetCli().GetKeyword() << "'." << cli::endl;
}

Shell::~Shell(void)
{
    GetTraces().Trace(TRACE_SHELL) << "Shell deleted for CLI '" << GetCli().GetKeyword() << "'." << cli::endl;

    if (m_pcliInput != NULL)
    {
        m_pcliInput->FreeInstance(__CALL_INFO__);
        m_pcliInput = NULL;
    }

    for (int i=0; i<STREAM_TYPES_COUNT; i++)
    {
        if (m_artStream[i].pcliStream != NULL)
        {
            m_artStream[i].pcliStream->FreeInstance(__CALL_INFO__);
            m_artStream[i].pcliStream = NULL;
        }
    }
}

const Cli& Shell::GetCli(void) const
{
    assert(m_pcliCli != NULL);
    return *m_pcliCli;
}

const OutputDevice& Shell::GetStream(const STREAM_TYPE E_StreamType) const
{
    if ((E_StreamType >= 0) && (E_StreamType < STREAM_TYPES_COUNT))
    {
        if (m_artStream[E_StreamType].bEnable)
        {
            if (const IOMux* const pcli_IOMux = dynamic_cast<const IOMux*>(m_artStream[E_StreamType].pcliStream))
            {
                // This is a hack to have IOMux multiplexing outputs correctly.
                return pcli_IOMux->GetOutput(E_StreamType);
            }
            else if (m_artStream[E_StreamType].pcliStream != NULL)
            {
                return *m_artStream[E_StreamType].pcliStream;
            }
        }
    }

    // Default to null device.
    return OutputDevice::GetNullDevice();
}

const bool Shell::SetStream(const STREAM_TYPE E_StreamType, OutputDevice& CLI_Stream)
{
    if ((E_StreamType < 0) || (E_StreamType >= STREAM_TYPES_COUNT))
    {
        return false;
    }
    else
    {
        // Free previous reference.
        if (OutputDevice* const pcli_Stream = m_artStream[E_StreamType].pcliStream)
        {
            bool b_Res = true;

            // Unreference the device right now.
            m_artStream[E_StreamType].pcliStream = NULL;
            if (IsRunning())
            {
                if (! pcli_Stream->CloseDown(__CALL_INFO__))
                {
                    b_Res = false;
                }
            }
            pcli_Stream->FreeInstance(__CALL_INFO__);

            if (! b_Res)
            {
                // Abort on error.
                return false;
            }
        }

        // Store next reference.
        {
            CLI_Stream.UseInstance(__CALL_INFO__);
            if (IsRunning())
            {
                if (! CLI_Stream.OpenUp(__CALL_INFO__))
                {
                    // Store nothing on error.
                    CLI_Stream.FreeInstance(__CALL_INFO__);
                    return false;
                }
            }
            // Do not store the reference until opening is done.
            m_artStream[E_StreamType].pcliStream = & CLI_Stream;
        }

        return true;
    }
}

const bool Shell::StreamEnabled(const STREAM_TYPE E_StreamType) const
{
    if ((E_StreamType < 0) || (E_StreamType >= STREAM_TYPES_COUNT))
    {
        return false;
    }
    else
    {
        return m_artStream[E_StreamType].bEnable;
    }
}

const bool Shell::EnableStream(const STREAM_TYPE E_StreamType, const bool B_Enable)
{
    if ((E_StreamType < 0) || (E_StreamType >= STREAM_TYPES_COUNT))
    {
        return false;
    }
    else
    {
        m_artStream[E_StreamType].bEnable = B_Enable;
        return true;
    }
}

void Shell::SetWelcomeMessage(const std::string& STR_WelcomeMessage)
{
    m_strWelcomeMessage = STR_WelcomeMessage;
}

void Shell::SetByeMessage(const std::string& STR_ByeMessage)
{
    m_strByeMessage = STR_ByeMessage;
}

void Shell::SetPrompt(const std::string& STR_Prompt)
{
    m_strNoDefaultPrompt = STR_Prompt;
}

void Shell::SetLang(const Help::LANG E_Lang)
{
    m_eLang = E_Lang;
}

const Help::LANG Shell::GetLang(void) const
{
    return m_eLang;
}

void Shell::SetBeep(const bool B_Enable)
{
    m_bBeep = B_Enable;
}

const bool Shell::GetBeep(void) const
{
    return m_bBeep;
}

void Shell::Run(IODevice& CLI_IODevice)
{
    GetTraces().Trace(TRACE_SHELL) << "Shell for CLI '" << GetCli().GetKeyword() << "' starts running." << cli::endl;
    if (OpenDevices(CLI_IODevice))
    {
        if (! m_vpcliMenus.empty())
        {
            // Remember the CLI reference.
            const Cli* const pcli_Cli = dynamic_cast<const Cli*>(m_vpcliMenus[0]);

            PromptWelcomeMessage();
            PromptMenu();
            while (! m_vpcliMenus.empty())
            {
                const KEY e_Key = CLI_IODevice.GetKey();
                if (e_Key != NULL_KEY)
                {
                    OnKey(e_Key);
                }
                else
                {
                    m_vpcliMenus.erase(m_vpcliMenus.begin(), m_vpcliMenus.end());
                }
            }
            PromptByeMessage();

            // Restore the CLI reference.
            m_vpcliMenus.push_back(pcli_Cli);
        }
        else
        {
            assert(false);
            GetStream(ERROR_STREAM) << "No current menu!" << endl;
        }
    }
    CloseDevices(CLI_IODevice);
    GetTraces().Trace(TRACE_SHELL) << "Shell for CLI '" << GetCli().GetKeyword() << "' end of run." << cli::endl;
}

const bool Shell::IsRunning(void) const
{
    return (m_pcliInput != NULL);
}

const unsigned int Shell::GetHelpMargin(void) const
{
    return HELP_MARGIN;
}

const unsigned int Shell::GetHelpOffset(void) const
{
    return HELP_OFFSET;
}

const bool Shell::OpenDevices(IODevice& CLI_IODevice)
{
    GetTraces().Trace(TRACE_SHELL) << "Shell for CLI '" << GetCli().GetKeyword() << "' opening devices." << cli::endl;

    bool b_Res = true;

    // First of all, set references.
    {
        // Input.
        if (m_pcliInput != NULL)
        {
            m_pcliInput->FreeInstance(__CALL_INFO__);
            m_pcliInput = NULL;
        }
        m_pcliInput = & CLI_IODevice;
        m_pcliInput->UseInstance(__CALL_INFO__);

        // Output.
        for (int i=0; i<STREAM_TYPES_COUNT; i++)
        {
            if (m_artStream[i].pcliStream == NULL)
            {
                m_artStream[i].pcliStream = & CLI_IODevice;
                m_artStream[i].pcliStream->UseInstance(__CALL_INFO__);
            }
            //! @note Enables all output streams by default.
            m_artStream[i].bEnable = true;
        }
    }

    // Then open up devices.
    {
        // Input.
        if ((m_pcliInput == NULL)
            || (! m_pcliInput->OpenUp(__CALL_INFO__)))
        {
            b_Res = false;
        }

        // Output.
        for (int i=0; i<STREAM_TYPES_COUNT; i++)
        {
            if ((m_artStream[i].pcliStream == NULL)
                || (! m_artStream[i].pcliStream->OpenUp(__CALL_INFO__)))
            {
                b_Res = false;
            }
        }
    }

    return b_Res;
}

const bool Shell::CloseDevices(IODevice& CLI_IODevice)
{
    GetTraces().Trace(TRACE_SHELL) << "Shell for CLI '" << GetCli().GetKeyword() << "' closing devices." << cli::endl;

    bool b_Res = true;

    // Close down devices.
    {
        // Output.
        for (int i=0; i<STREAM_TYPES_COUNT; i++)
        {
            if ((m_artStream[i].pcliStream != NULL)
                && (! m_artStream[i].pcliStream->CloseDown(__CALL_INFO__)))
            {
                b_Res = false;
            }
        }

        // Input device.
        if ((m_pcliInput != NULL)
            && (! m_pcliInput->CloseDown(__CALL_INFO__)))
        {
            b_Res = false;
        }
    }

    // Unset references.
    {
        // Output.
        for (int i=0; i<STREAM_TYPES_COUNT; i++)
        {
            if (m_artStream[i].pcliStream == & CLI_IODevice)
            {
                m_artStream[i].pcliStream->FreeInstance(__CALL_INFO__);
                m_artStream[i].pcliStream = NULL;
            }
        }

        // Input.
        if (m_pcliInput != NULL)
        {
            m_pcliInput->FreeInstance(__CALL_INFO__);
            m_pcliInput = NULL;
        }
    }

    return b_Res;
}

void Shell::PromptWelcomeMessage(void) const
{
    if (! m_strWelcomeMessage.empty())
    {
        GetStream(WELCOME_STREAM) << m_strWelcomeMessage;
    }
    else
    {
        GetStream(WELCOME_STREAM)
        << "---------------------------------------------------" << endl
        << " Welcome to " << GetCli().GetKeyword() << "!" << endl
        << endl
        << " " << GetCli().GetKeyword() << " is a command line interface" << endl
        << " using the CLI library" << endl
        << "   (c) Alexis Royer http://alexis.royer.free.fr/CLI/" << endl
        << " Type 'help' at any time" << endl
        << " or press '?' or TAB to get completion or help." << endl
        << "---------------------------------------------------" << endl;
    }
}

void Shell::PromptByeMessage(void) const
{
    if (! m_strByeMessage.empty())
    {
        GetStream(WELCOME_STREAM) << m_strByeMessage;
    }
    else
    {
        GetStream(WELCOME_STREAM) << "Bye!" << endl;
    }
}

void Shell::PromptMenu(void) const
{
    if (! m_strNoDefaultPrompt.empty())
    {
        GetStream(PROMPT_STREAM) << m_strNoDefaultPrompt << m_strLine;
    }
    else
    {
        if (! m_vpcliMenus.empty())
        {
            GetStream(PROMPT_STREAM) << m_vpcliMenus.back()->GetKeyword() << ">";
            GetStream(ECHO_STREAM) << m_strLine;
        }
    }
}

void Shell::ExitMenu(void)
{
    std::string str_Mem = m_strLine;
    m_strLine.erase();
    OnExit(false);
    m_strLine = str_Mem;
}

void Shell::Quit(void)
{
    while (! m_vpcliMenus.empty())
    {
        ExitMenu();
    }
}

void Shell::DisplayHelp(void)
{
    if (! m_vpcliMenus.empty())
    {
        const std::string str_Mem = m_strLine;
        m_strLine.erase();
        OnHelp(false, true);
        m_strLine = str_Mem;
    }
    else
    {
        assert(false);
        GetStream(ERROR_STREAM) << "No current menu!" << endl;
    }
}

void Shell::PrintWorkingMenu(void)
{
    unsigned int i = 0;
    for (i=0; i<HELP_MARGIN; i++)
    {
        GetStream(OUTPUT_STREAM) << " ";
    }
    for (i=0; i<m_vpcliMenus.size(); i++)
    {
        GetStream(OUTPUT_STREAM) << "/" << m_vpcliMenus[i]->GetKeyword();
    }
    GetStream(OUTPUT_STREAM) << endl;
}

void Shell::EnterMenu(const Menu& CLI_Menu)
{
    m_vpcliMenus.push_back(& CLI_Menu);
    PromptMenu();
}

void Shell::OnKey(const KEY E_KeyCode)
{
    // In general, history navigation is lost.
    const int i_HistoryIndexBackup = m_iHistoryIndex;
    m_iHistoryIndex = 0;

    switch (E_KeyCode)
    {
    case KEY_UP:    m_iHistoryIndex = i_HistoryIndexBackup;
                    OnHistory(1);                   break;
    case KEY_DOWN:  m_iHistoryIndex = i_HistoryIndexBackup;
                    OnHistory(-1);                  break;
    case PAGE_UP:   m_iHistoryIndex = i_HistoryIndexBackup;
                    OnHistory(HISTORY_PAGE);        break;
    case PAGE_DOWN: m_iHistoryIndex = i_HistoryIndexBackup;
                    OnHistory(-HISTORY_PAGE);       break;
    case BACKSPACE: OnBackspace();                  break;
    case ENTER:     OnExecute();                    break;
    case BREAK:
    case ESCAPE:    OnEscape();                     break;
    case LOGOUT:    OnExit(true);                   break;
    case TAB:       OnHelp(true, false);            break;
    case QUESTION:
        if (m_strLine[m_strLine.size() - 1] == '\\')
            OnPrintableChar(E_KeyCode);
        else
            OnHelp(true, true);
        break;

    case KEY_a: case KEY_b: case KEY_c: case KEY_d: case KEY_e: case KEY_f:
    case KEY_g: case KEY_h: case KEY_i: case KEY_j: case KEY_k: case KEY_l:
    case KEY_m: case KEY_n: case KEY_o: case KEY_p: case KEY_q: case KEY_r:
    case KEY_s: case KEY_t: case KEY_u: case KEY_v: case KEY_w: case KEY_x:
    case KEY_y: case KEY_z:

    case KEY_A: case KEY_B: case KEY_C: case KEY_D: case KEY_E: case KEY_F:
    case KEY_G: case KEY_H: case KEY_I: case KEY_J: case KEY_K: case KEY_L:
    case KEY_M: case KEY_N: case KEY_O: case KEY_P: case KEY_Q: case KEY_R:
    case KEY_S: case KEY_T: case KEY_U: case KEY_V: case KEY_W: case KEY_X:
    case KEY_Y: case KEY_Z:

    case KEY_0: case KEY_1: case KEY_2: case KEY_3: case KEY_4: case KEY_5:
    case KEY_6: case KEY_7: case KEY_8: case KEY_9:

    case PLUS:
    case MINUS:
    case STAR:
    case SLASH:
    case LOWER_THAN:
    case GREATER_THAN:
    case EQUAL:
    case PERCENT:

    case SPACE:
    case UNDERSCORE:
    case AROBASE:
    case SHARP:
    case AMPERCENT:
    case DOLLAR:
    case BACKSLASH:
    case PIPE:

    case EXCLAMATION:
    case COLUMN:
    case DOT:
    case COMA:
    case SEMI_COLUMN:
    case QUOTE:
    case DOUBLE_QUOTE:

    case OPENING_BRACE:
    case CLOSING_BRACE:
    case OPENING_CURLY_BRACE:
    case CLOSING_CURLY_BRACE:
    case OPENING_BRACKET:
    case CLOSING_BRACKET:
        OnPrintableChar(E_KeyCode);
        break;

    default:
        break;
    }
}

void Shell::OnPrintableChar(const char C_KeyCode)
{
    PrintLine(C_KeyCode);
}

void Shell::OnBackspace(void)
{
    Backspace(1);
}

void Shell::OnEscape(void)
{
    m_strLine.erase();
    GetStream(ECHO_STREAM) << endl;
    if (! m_vpcliMenus.empty())
    {
        PromptMenu();
    }
    else
    {
        assert(false);
        GetStream(ERROR_STREAM) << "No current menu!" << endl;
    }
}

void Shell::OnExit(const bool B_Edition)
{
    if (m_strLine.empty())
    {
        if (B_Edition)
        {
            GetStream(ECHO_STREAM) << endl;
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

void Shell::OnHelp(const bool B_Edition, const bool B_HelpOnly)
{
    if (! m_vpcliMenus.empty())
    {
        // Parse the command line.
        CommandLine cli_CommandLine;
        if (cli_CommandLine.Parse(*m_vpcliMenus.back(), m_strLine, false))
        {
            // Research matching elements.
            ElementList cli_Exact, cli_Elements;
            if (! cli_CommandLine.GetLastElement().FindElements(
                    cli_Exact, cli_Elements, cli_CommandLine.GetLastWord()))
            {
                return;
            }

            // Initialize completion so far.
            std::string str_CompletionSoFar;
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
                        && (! dynamic_cast<const Endl*>(cli_Elements.front()))
                        && (! dynamic_cast<const Param*>(cli_Elements.front())))
                {
                    // Straight forward completion.
                    Backspace(cli_CommandLine.GetNumBackspacesForCompletion());
                    PrintLine(cli_Elements.front()->GetKeyword() + " ");
                }
                else
                {
                    // Look for completion so far.
                    bool b_InitCompletionSoFar = true;
                    for (   ElementList::const_iterator it = cli_Elements.begin();
                            it != cli_Elements.end();
                            it ++)
                    {
                        if (dynamic_cast<const Param*>(*it)
                            || dynamic_cast<const Endl*>(*it))
                        {
                            // No completion for parameters and end of lines.
                            str_CompletionSoFar.erase();
                            break;
                        }
                        else if (const Element* const pcli_Element = dynamic_cast<const Element*>(*it))
                        {
                            if (b_InitCompletionSoFar)
                            {
                                // First initialization of str_CompletionSoFar.
                                str_CompletionSoFar = pcli_Element->GetKeyword();
                                b_InitCompletionSoFar = false;
                            }
                            else
                            {
                                // Remove last character of str_CompletionSoFar till it matches the current keyword.
                                while ( (! str_CompletionSoFar.empty())
                                        && (pcli_Element->GetKeyword().compare(0, str_CompletionSoFar.size(), str_CompletionSoFar) != 0))
                                {
                                    str_CompletionSoFar = str_CompletionSoFar.substr(0, str_CompletionSoFar.size() - 1);
                                }
                            }
                        }
                    }

                    // When no more completion so far available, print help.
                    if ((cli_CommandLine.GetLastWord() != NULL)
                        && (str_CompletionSoFar == cli_CommandLine.GetLastWord()))
                    {
                        b_PrintHelpList = true;
                    }
                }
            }

            // Help display.
            if (b_PrintHelpList)
            {
                // First of all, sort help elements on keywords.
                class _ { public:
                    static const bool cmp(
                            const Element* const PCLI_1,
                            const Element* const PCLI_2)
                    {
                        return (PCLI_1->GetKeyword() < PCLI_2->GetKeyword());
                    }
                };
                std::sort(cli_Elements.begin(), cli_Elements.end(), _::cmp);

                if (B_Edition)
                {
                    GetStream(ECHO_STREAM) << endl;
                }

                // For each matching keyword...
                for (   ElementList::const_iterator it = cli_Elements.begin();
                        it != cli_Elements.end();
                        it ++)
                {
                    // Print the corresponding help.
                    PrintHelp(**it);
                }

                if (B_Edition)
                {
                    PromptMenu();
                }
            }
            // Completion so far.
            else if ((! B_HelpOnly) && (! str_CompletionSoFar.empty()))
            {
                Backspace(cli_CommandLine.GetNumBackspacesForCompletion());
                PrintLine(str_CompletionSoFar);
            }
        }
        else
        {
            if (B_Edition)
            {
                GetStream(ECHO_STREAM) << endl;
            }
            GetStream(ERROR_STREAM) << cli_CommandLine.GetLastError() << endl;
            if (B_Edition)
            {
                PromptMenu();
            }
        }
    }
    else
    {
        assert(false);
        GetStream(ERROR_STREAM) << "No current menu!" << endl;
    }
}

void Shell::OnExecute(void)
{
    GetStream(ECHO_STREAM) << endl;

    if (! m_vpcliMenus.empty())
    {
        // First of all, store the line in history stack.
        PushHistory(m_strLine);

        CommandLine cli_CommandLine;
        if (cli_CommandLine.Parse(*m_vpcliMenus.back(), m_strLine + "\n", true))
        {
            bool b_Executed = false;
            if (cli_CommandLine.GetElementCount() > 1)
            {
                if (const Menu* const pcli_Menu = m_vpcliMenus.back())
                {
                    if (pcli_Menu->ExecuteReserved(cli_CommandLine))
                        b_Executed = true;
                    if (pcli_Menu->Execute(cli_CommandLine))
                        b_Executed = true;
                }

                if (b_Executed)
                {
                    if (const Endl* pendl = dynamic_cast<const Endl*>(& cli_CommandLine.GetLastElement()))
                    {
                        if (pendl->GetMenuRef() != NULL)
                        {
                            m_vpcliMenus.push_back(& pendl->GetMenuRef()->GetMenu());
                        }
                    }
                }
                else
                {
                    GetStream(ERROR_STREAM) << "Execution error" << endl;
                }
            }
        }
        else
        {
            GetStream(ERROR_STREAM) << cli_CommandLine.GetLastError() << endl;
        }
        m_strLine.erase();
        PromptMenu();
    }
    else
    {
        assert(false);
        GetStream(ERROR_STREAM) << "No current menu!" << endl;
    }
}

void Shell::OnHistory(const int I_Translation)
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

void Shell::PrintLine(const std::string& STR_Append)
{
    m_strLine += STR_Append;
    GetStream(ECHO_STREAM) << STR_Append;
}

void Shell::PrintLine(const char C_Append)
{
    PrintLine(std::string() += C_Append);
}

void Shell::Backspace(const int I_BackspaceCount)
{
    unsigned int i_BackspaceCount = I_BackspaceCount;
    if (i_BackspaceCount > m_strLine.size())
    {
        Beep();
        i_BackspaceCount = m_strLine.size();
    }

    m_strLine = m_strLine.substr(0, m_strLine.size() - i_BackspaceCount);
    for (unsigned int i=0; i<i_BackspaceCount; i++)
    {
        GetStream(ECHO_STREAM) << "\b" << " " << "\b";
    }
}

void Shell::PrintHelp(const Element& CLI_Element)
{
    // Retrieve basic information.
    std::string str_Keyword = CLI_Element.GetKeyword();
    std::string str_Help = CLI_Element.GetHelp().GetHelp(GetLang());

    // Optional corrections.
    if (dynamic_cast<const Endl*>(& CLI_Element))
    {
        str_Keyword = "<cr>";
    }
    else if (const Param* const pcli_Param = dynamic_cast<const Param*>(& CLI_Element))
    {
        if (const Param* const pcli_Clone = pcli_Param->Clone())
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
    unsigned int i = 0;
    for (i = 0; i < HELP_MARGIN; i++)
    {
        GetStream(OUTPUT_STREAM) << " ";
    }
    GetStream(OUTPUT_STREAM) << str_Keyword << " ";
    for (i = str_Keyword.size(); i < HELP_OFFSET; i++)
    {
        GetStream(OUTPUT_STREAM) << " ";
    }
    GetStream(OUTPUT_STREAM) << str_Help;
    GetStream(OUTPUT_STREAM) << endl;
}

void Shell::Beep(void)
{
    if (m_bBeep)
    {
        GetStream(ERROR_STREAM).Beep();
    }
}

void Shell::PushHistory(const std::string& STR_Line)
{
    if (// Check the line is not empty.
        (! STR_Line.empty())
        // Check it is not the same as the previous one.
        && ((m_vstrHistory.size() < 2) || (STR_Line != m_vstrHistory[1])))
    {
        while (m_vstrHistory.size() > HISTORY_STACK_SIZE)
        {
            m_vstrHistory.pop_back();
        }
        m_vstrHistory[0] = STR_Line;
        m_vstrHistory.insert(m_vstrHistory.begin(), "");
    }
    m_iHistoryIndex = 0;
}
