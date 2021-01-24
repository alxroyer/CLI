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


#include "cli/pch.h"

#include <stdio.h>
#include <string.h>

#include "cli/shell.h"
#include "cli/io_device.h"
#include "cli/io_mux.h"
#include "cli/cli.h"
#include "cli/menu.h"
#include "cli/command_line.h"
#include "cli/endl.h"
#include "cli/param.h"
#include "cli/traces.h"
#include "cli/assert.h"
#include "consistency.h"
#include "constraints.h"

CLI_NS_USE(cli)


static const unsigned int HELP_MARGIN = 2;
static const unsigned int HELP_OFFSET = 15;
static const unsigned int HISTORY_PAGE = 5;
static const unsigned int HISTORY_STACK_SIZE = 100;

static const TraceClass TRACE_SHELL("CLI_SHELL", Help()
    .AddHelp(Help::LANG_EN, "Shell traces")
    .AddHelp(Help::LANG_FR, "Traces du shell"));


Shell::Shell(const Cli& CLI_Cli)
  : m_pcliCli(& CLI_Cli), m_pcliInput(NULL),
    m_eLang(Help::LANG_EN), m_bBeep(true),
    m_qMenus(MAX_MENU_PER_CLI),
    m_strLine(MAX_CMD_LINE_LENGTH),
    m_qHistory(HISTORY_STACK_SIZE),
    m_iHistoryIndex(0)
{
    EnsureCommonDevices();
    EnsureTraces();

    for (int i=0; i<STREAM_TYPES_COUNT; i++)
    {
        m_artStream[i].pcliStream = NULL;
        m_artStream[i].bEnable = true;
    }

    if ((! m_qMenus.AddHead(& CLI_Cli))
        || (! m_qHistory.AddHead(m_strLine)))
    {
        GetTraces().Trace(INTERNAL_ERROR) << "Could not set initial shell status." << endl;
    }
    CLI_Cli.SetShell(*this);

    GetTraces().Trace(TRACE_SHELL) << "New shell for CLI '" << GetCli().GetKeyword() << "'." << endl;
}

Shell::~Shell(void)
{
    GetTraces().Trace(TRACE_SHELL) << "Shell deleted for CLI '" << GetCli().GetKeyword() << "'." << endl;

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
    CLI_ASSERT(m_pcliCli != NULL);
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

void Shell::SetWelcomeMessage(const ResourceString& CLI_WelcomeMessage)
{
    m_cliWelcomeMessage = CLI_WelcomeMessage;
}

void Shell::SetByeMessage(const ResourceString& CLI_ByeMessage)
{
    m_cliByeMessage = CLI_ByeMessage;
}

void Shell::SetPrompt(const ResourceString& CLI_Prompt)
{
    m_cliNoDefaultPrompt = CLI_Prompt;
}

void Shell::SetLang(const ResourceString::LANG E_Lang)
{
    m_eLang = E_Lang;
}

const ResourceString::LANG Shell::GetLang(void) const
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
    GetTraces().Trace(TRACE_SHELL) << "Shell for CLI '" << GetCli().GetKeyword() << "' starts running." << endl;
    if (OpenDevices(CLI_IODevice))
    {
        // Check the CLI is the first menu, and remember the reference.
        if (const Cli* const pcli_Cli = dynamic_cast<const Cli*>(m_qMenus.GetHead()))
        {
            PromptWelcomeMessage();
            PromptMenu();
            while (! m_qMenus.IsEmpty())
            {
                const KEY e_Key = CLI_IODevice.GetKey();
                if (e_Key != NULL_KEY)
                {
                    OnKey(e_Key);
                }
                else
                {
                    while (! m_qMenus.IsEmpty())
                    {
                        m_qMenus.RemoveTail();
                    }
                }
            }
            PromptByeMessage();

            // Restore the CLI reference.
            if (! m_qMenus.AddTail(pcli_Cli))
            {
                GetTraces().Trace(INTERNAL_ERROR) << "Could not restore a valid status on shell exit." << endl;
            }
        }
    }
    CloseDevices(CLI_IODevice);
    GetTraces().Trace(TRACE_SHELL) << "Shell for CLI '" << GetCli().GetKeyword() << "' end of run." << endl;
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
    GetTraces().Trace(TRACE_SHELL) << "Shell for CLI '" << GetCli().GetKeyword() << "' opening devices." << endl;

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
    GetTraces().Trace(TRACE_SHELL) << "Shell for CLI '" << GetCli().GetKeyword() << "' closing devices." << endl;

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
    const tk::String str_WelcomeMessage = m_cliWelcomeMessage.GetString(GetLang());
    if (! str_WelcomeMessage.IsEmpty())
    {
        GetStream(WELCOME_STREAM) << str_WelcomeMessage;
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
    const tk::String str_ByeMessage = m_cliByeMessage.GetString(GetLang());
    if (! str_ByeMessage.IsEmpty())
    {
        GetStream(WELCOME_STREAM) << str_ByeMessage;
    }
    else
    {
        GetStream(WELCOME_STREAM) << "Bye!" << endl;
    }
}

void Shell::PromptMenu(void) const
{
    // Show the prompt.
    const tk::String str_NoDefaultPrompt = m_cliNoDefaultPrompt.GetString(GetLang());
    if (! str_NoDefaultPrompt.IsEmpty())
    {
        GetStream(PROMPT_STREAM) << str_NoDefaultPrompt;
    }
    else
    {
        if (! m_qMenus.IsEmpty())
        {
            if (const Menu* const pcli_Menu = m_qMenus.GetTail())
            {
                GetStream(PROMPT_STREAM) << pcli_Menu->GetKeyword() << ">";
            }
        }
    }

    // Eventually echo the current 
    GetStream(ECHO_STREAM) << m_strLine;
}

void Shell::ExitMenu(void)
{
    tk::String str_Mem = m_strLine;
    if (! m_strLine.Set(""))
        GetTraces().Trace(INTERNAL_ERROR) << "Shell::ExitMenu(): Could not reset m_strLine" << endl;
    OnExit(false);
    if (! m_strLine.Set(str_Mem))
        GetTraces().Trace(INTERNAL_ERROR) << "Shell::ExitMenu(): Could not restore str_Mem in m_strLine" << endl;
}

void Shell::Quit(void)
{
    while (! m_qMenus.IsEmpty())
    {
        ExitMenu();
    }
}

void Shell::DisplayHelp(void)
{
    if (! m_qMenus.IsEmpty())
    {
        const tk::String str_Mem = m_strLine;
        if (! m_strLine.Set(""))
            GetTraces().Trace(INTERNAL_ERROR) << "Shell::DisplayHelp(): Could not reset m_strLine" << endl;
        OnHelp(false, true);
        if (! m_strLine.Set(str_Mem))
            GetTraces().Trace(INTERNAL_ERROR) << "Shell::DisplayHelp(): Could not reset str_Mem in m_strLine" << endl;
    }
}

void Shell::PrintWorkingMenu(void)
{
    unsigned int i = 0;
    for (i=0; i<HELP_MARGIN; i++)
    {
        GetStream(OUTPUT_STREAM) << " ";
    }
    for (   tk::Queue<const Menu*>::Iterator it = m_qMenus.GetIterator();
            m_qMenus.IsValid(it);
            m_qMenus.MoveNext(it))
    {
        if (const Menu* const pcli_Menu = m_qMenus.GetAt(it))
        {
            GetStream(OUTPUT_STREAM) << "/" << pcli_Menu->GetKeyword();
        }
    }
    GetStream(OUTPUT_STREAM) << endl;
}

void Shell::EnterMenu(const Menu& CLI_Menu)
{
    if (! m_qMenus.AddTail(& CLI_Menu))
    {
        GetStream(ERROR_STREAM)
            << "Too many menus. "
            << "Cannot enter '" << CLI_Menu.GetName() << "'."
            << endl;
    }
    //  PromptMenu();
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
        if (m_strLine.GetChar(m_strLine.GetLength() - 1) == '\\')
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
    m_strLine.Set("");
    GetStream(ECHO_STREAM) << endl;
    if (! m_qMenus.IsEmpty())
    {
        PromptMenu();
    }
}

void Shell::OnExit(const bool B_Edition)
{
    if (m_strLine.IsEmpty())
    {
        if (B_Edition)
        {
            GetStream(ECHO_STREAM) << endl;
        }
        if (const Menu* const pcli_Menu = m_qMenus.RemoveTail())
        {
            pcli_Menu->OnExit();
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
    if (! m_qMenus.IsEmpty())
    {
        // Parse the command line.
        CommandLine cli_CommandLine;
        if (cli_CommandLine.Parse(*m_qMenus.GetTail(), m_strLine, false))
        {
            // Research matching elements.
            Element::List cli_Exact(MAX_WORDS_PER_NODE), cli_Elements(MAX_WORDS_PER_NODE);
            if (! cli_CommandLine.GetLastElement().FindElements(
                    cli_Exact, cli_Elements, cli_CommandLine.GetLastWord()))
            {
                return;
            }

            // Initialize completion so far.
            tk::String str_CompletionSoFar(MAX_WORD_LENGTH);
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
                if (cli_Elements.IsEmpty())
                {
                    // No matching element.
                    Beep();
                }
                else if ((cli_Elements.GetCount() == 1)
                        && (! dynamic_cast<const Endl*>(cli_Elements.GetHead()))
                        && (! dynamic_cast<const Param*>(cli_Elements.GetHead())))
                {
                    // Straight forward completion.
                    Backspace(cli_CommandLine.GetNumBackspacesForCompletion());
                    PrintLine(cli_Elements.GetHead()->GetKeyword());
                    PrintLine(" ");
                }
                else
                {
                    // Look for completion so far.
                    bool b_InitCompletionSoFar = true;
                    for (   Element::List::Iterator it = cli_Elements.GetIterator();
                            cli_Elements.IsValid(it);
                            cli_Elements.MoveNext(it))
                    {
                        if (dynamic_cast<const Param*>(cli_Elements.GetAt(it))
                            || dynamic_cast<const Endl*>(cli_Elements.GetAt(it)))
                        {
                            // No completion for parameters and end of lines.
                            str_CompletionSoFar.Set("");
                            break;
                        }
                        else if (const Element* const pcli_Element = dynamic_cast<const Element*>(cli_Elements.GetAt(it)))
                        {
                            if (b_InitCompletionSoFar)
                            {
                                // First initialization of str_CompletionSoFar.
                                if (! str_CompletionSoFar.Set(pcli_Element->GetKeyword()))
                                    GetTraces().Trace(INTERNAL_ERROR) << "Shell::OnHelp(): Could not store completion so far" << endl;
                                b_InitCompletionSoFar = false;
                            }
                            else
                            {
                                // Remove last character of str_CompletionSoFar till it matches the current keyword.
                                while ( (! str_CompletionSoFar.IsEmpty())
                                        && (pcli_Element->GetKeyword().SubString(0, str_CompletionSoFar.GetLength()) != str_CompletionSoFar))
                                {
                                    if (! str_CompletionSoFar.Set(str_CompletionSoFar.SubString(0, str_CompletionSoFar.GetLength() - 1)))
                                        GetTraces().Trace(INTERNAL_ERROR) << "Shell::OnHelp(): Could not store completion so far" << endl;
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
                    static const int cmp(const Element* const& PCLI_1, const Element* const& PCLI_2)
                    {
                        if (PCLI_1->GetKeyword() < PCLI_2->GetKeyword())
                            return 1;
                        if (PCLI_1->GetKeyword() > PCLI_2->GetKeyword())
                            return -1;
                        return 0;
                    }
                };
                cli_Elements.Sort(_::cmp);

                if (B_Edition)
                {
                    GetStream(ECHO_STREAM) << endl;
                }

                // For each matching keyword...
                for (   Element::List::Iterator it = cli_Elements.GetIterator();
                        cli_Elements.IsValid(it);
                        cli_Elements.MoveNext(it))
                {
                    // Print the corresponding help.
                    PrintHelp(*cli_Elements.GetAt(it));
                }

                if (B_Edition)
                {
                    PromptMenu();
                }
            }
            // Completion so far.
            else if ((! B_HelpOnly) && (! str_CompletionSoFar.IsEmpty()))
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
            GetStream(ERROR_STREAM) << cli_CommandLine.GetLastError().GetString(GetLang()) << endl;
            if (B_Edition)
            {
                PromptMenu();
            }
        }
    }
}

void Shell::OnExecute(void)
{
    GetStream(ECHO_STREAM) << endl;

    if (! m_qMenus.IsEmpty())
    {
        // First of all, store the line in history stack.
        PushHistory(m_strLine);

        // Append "\n" to the line and parse it.
        tk::String str_Line(MAX_CMD_LINE_LENGTH + 1);
        CommandLine cli_CommandLine;
        if (str_Line.Set(m_strLine) && str_Line.Append("\n")
            && cli_CommandLine.Parse(*m_qMenus.GetTail(), str_Line, true))
        {
            bool b_Executed = false;
            // No need to execute unless there are at least one word and one endl elements in the command line.
            CommandLineIterator it(cli_CommandLine);
            if (it.StepIt() && it.StepIt())
            {
                // Command line execution.
                if (const Menu* const pcli_Menu = m_qMenus.GetTail())
                {
                    if (pcli_Menu->ExecuteReserved(cli_CommandLine))
                        b_Executed = true;
                    if (pcli_Menu->Execute(cli_CommandLine))
                        b_Executed = true;
                }

                // Post-execution.
                if (b_Executed)
                {
                    // Entering a sub-menu.
                    if (const Endl* pcli_Endl = dynamic_cast<const Endl*>(& cli_CommandLine.GetLastElement()))
                    {
                        if (pcli_Endl->GetMenuRef() != NULL)
                        {
                            EnterMenu(pcli_Endl->GetMenuRef()->GetMenu());
                        }
                    }
                }
                else
                {
                    const ResourceString cli_ExecutionError = ResourceString()
                        .SetString(ResourceString::LANG_EN, "Execution error")
                        .SetString(ResourceString::LANG_FR, "Erreur d'exécution");
                    GetStream(ERROR_STREAM) << cli_ExecutionError.GetString(GetLang()) << endl;
                }
            }
        }
        else
        {
            GetStream(ERROR_STREAM) << cli_CommandLine.GetLastError().GetString(GetLang()) << endl;
        }
        m_strLine.Set("");
        PromptMenu();
    }
}

void Shell::OnHistory(const int I_Translation)
{
    // First check the translation remains in bounds of the history stack.
    if ((m_iHistoryIndex + I_Translation >= 0)
        && (m_iHistoryIndex + I_Translation < (signed) m_qHistory.GetCount()))
    {
        // If this is the current line, save it into the history stack.
        if (m_iHistoryIndex == 0)
        {
            m_qHistory.GetHead().Set(m_strLine);
        }
        // Clean up the current line.
        Backspace(m_strLine.GetLength());
        // Translate the history index.
        m_iHistoryIndex += I_Translation;
        // Print out this line.
        PrintLine(GetHistoryLine(m_iHistoryIndex));
    }
    else
    {
        // Out of stack.
        Beep();
    }
}

void Shell::PrintLine(const char* const STR_Append)
{
    if (m_strLine.Append(STR_Append))
    {
        GetStream(ECHO_STREAM) << STR_Append;
    }
    else
    {
        const ResourceString cli_LineError = ResourceString()
            .SetString(ResourceString::LANG_EN, "Line too long")
            .SetString(ResourceString::LANG_FR, "Ligne trop longue");
        GetStream(ERROR_STREAM) << endl << cli_LineError.GetString(GetLang()) << endl;
        PromptMenu();
    }
}

void Shell::PrintLine(const char C_Append)
{
    char arc_Buffer[] = { C_Append, '\0' };
    PrintLine(arc_Buffer);
}

void Shell::Backspace(const int I_BackspaceCount)
{
    unsigned int i_BackspaceCount = I_BackspaceCount;
    if (i_BackspaceCount > m_strLine.GetLength())
    {
        Beep();
        i_BackspaceCount = m_strLine.GetLength();
    }

    if (! m_strLine.Set(m_strLine.SubString(0, m_strLine.GetLength() - i_BackspaceCount)))
        GetTraces().Trace(INTERNAL_ERROR) << "Shell::Backspace(): Could not reduce m_strLine" << endl;
    for (unsigned int i=0; i<i_BackspaceCount; i++)
    {
        GetStream(ECHO_STREAM) << "\b" << " " << "\b";
    }
}

void Shell::PrintHelp(const Element& CLI_Element)
{
    // Retrieve basic information.
    tk::String str_Keyword = CLI_Element.GetKeyword();
    tk::String str_Help = CLI_Element.GetHelp().GetString(GetLang());

    // Optional corrections.
    if (dynamic_cast<const Endl*>(& CLI_Element))
    {
        str_Keyword.Set("<cr>");
    }
    else if (const Param* const pcli_Param = dynamic_cast<const Param*>(& CLI_Element))
    {
        if (const Param* const pcli_Clone = pcli_Param->Clone())
        {
            pcli_Clone->SetstrValue("");
            if (! str_Keyword.Set(pcli_Clone->GetKeyword()))
                GetTraces().Trace(INTERNAL_ERROR) << "Shell::PrintHelp(): Could not review str_Keyword for the parameter" << endl;
            if (! pcli_Param->GetstrValue().IsEmpty())
            {
                if (! str_Keyword.Append("("))
                    GetTraces().Trace(INTERNAL_ERROR) << "Shell::PrintHelp(): Could not review str_Keyword for the parameter" << endl;
                if (! str_Keyword.Append(pcli_Param->GetstrValue()))
                    GetTraces().Trace(INTERNAL_ERROR) << "Shell::PrintHelp(): Could not review str_Keyword for the parameter" << endl;
                if (! str_Keyword.Append(")"))
                    GetTraces().Trace(INTERNAL_ERROR) << "Shell::PrintHelp(): Could not review str_Keyword for the parameter" << endl;
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
    for (i = str_Keyword.GetLength(); i < HELP_OFFSET; i++)
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

void Shell::PushHistory(const char* const STR_Line)
{
    if (// Check the line is not empty.
        (STR_Line != NULL) && (strlen(STR_Line) > 0)
        // Check it is not the same as the previous one.
        && (STR_Line != GetHistoryLine(1)))
    {
        // Limit to HISTORY_STACK_SIZE.
        while (m_qHistory.GetCount() > HISTORY_STACK_SIZE)
        {
            m_qHistory.RemoveTail();
        }
        // Ensure there is at least one element.
        if (m_qHistory.IsEmpty())
        {
            m_qHistory.AddTail(tk::String(MAX_CMD_LINE_LENGTH));
        }
        m_qHistory.GetHead().Set(STR_Line);
        m_qHistory.AddHead(tk::String(MAX_CMD_LINE_LENGTH, ""));
    }
    m_iHistoryIndex = 0;
}

const tk::String Shell::GetHistoryLine(const unsigned int UI_BackwardIndex) const
{
    tk::Queue<tk::String>::Iterator it = m_qHistory.GetIterator();
    for (unsigned int ui=UI_BackwardIndex; ui>0; ui--)
    {
        if ((! m_qHistory.MoveNext(it))
            || (! m_qHistory.IsValid(it)))
        {
            return tk::String(MAX_CMD_LINE_LENGTH);
        }
    }

    return m_qHistory.GetAt(it);
}
