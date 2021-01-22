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


const int CLI_HELP_MARGIN = 2;
const int CLI_HELP_OFFSET = 15;


CcliShell::CcliShell(CcliCli& CLI_Cli)
{
    m_vpcliMenus.push_back(& CLI_Cli);
    CLI_Cli.SetShell(*this);
    m_pcliCli = & CLI_Cli;
    m_arpcliStream[CLI_WELCOME_STREAM] = & CcliOutputDevice::GetNullDevice();
    m_arpcliStream[CLI_PROMPT_STREAM] = & CcliOutputDevice::GetNullDevice();
    m_arpcliStream[CLI_ECHO_STREAM] = & CcliOutputDevice::GetNullDevice();
    m_arpcliStream[CLI_OUTPUT_STREAM] = & CcliOutputDevice::GetStdOut();
    m_arpcliStream[CLI_ERROR_STREAM] = & CcliOutputDevice::GetStdErr();
    m_eLang = CcliHelp::CLI_LANG_EN;
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
    return *m_arpcliStream[CLI_WELCOME_STREAM];
}

const CcliOutputDevice& CcliShell::GetPromptStream(void) const
{
    return *m_arpcliStream[CLI_PROMPT_STREAM];
}

const CcliOutputDevice& CcliShell::GetEchoStream(void) const
{
    return *m_arpcliStream[CLI_ECHO_STREAM];
}

const CcliOutputDevice& CcliShell::GetOutputStream(void) const
{
    return *m_arpcliStream[CLI_OUTPUT_STREAM];
}

const CcliOutputDevice& CcliShell::GetErrorStream(void) const
{
    return *m_arpcliStream[CLI_ERROR_STREAM];
}

void CcliShell::SetLang(CcliHelp::CLI_LANG E_Lang)
{
    m_eLang = E_Lang;
}

CcliHelp::CLI_LANG CcliShell::GetLang(void) const
{
    return m_eLang;
}

void CcliShell::RunConsole(void)
{
    CcliConsole cli_Console;
    Run(cli_Console);
}

void CcliShell::RunTelnet(unsigned long UL_TcpPort)
{
    CcliTelnetServer cli_Server(*this, UL_TcpPort);
    cli_Server.StartServer();
}

void CcliShell::Run(CcliIODevice& CLI_IODevice)
{
    if (CLI_IODevice.OpenDevice())
    {
        m_arpcliStream[CLI_WELCOME_STREAM] = & CLI_IODevice;
        m_arpcliStream[CLI_PROMPT_STREAM] = & CLI_IODevice;
        m_arpcliStream[CLI_ECHO_STREAM] = & CLI_IODevice;
        m_arpcliStream[CLI_OUTPUT_STREAM] = & CLI_IODevice;
        m_arpcliStream[CLI_ERROR_STREAM] = & CLI_IODevice;

        if (! m_vpcliMenus.empty())
        {
            PromptWelcomeMessage();
            PromptMenu();
            while (! m_vpcliMenus.empty())
            {
                int i_Key = CLI_IODevice.GetChar();
                OnKey(i_Key);
            }
            PromptByeMessage();
        }
        else
        {
            assert(false);
            CLI_IODevice << "No current menu!" << CLI_ENDL;
        }
    }
    CLI_IODevice.CloseDevice();
}

void CcliShell::PromptWelcomeMessage(void) const
{
    GetWelcomeStream()
    << "---------------------------------------------------" << CLI_ENDL
    << " Welcome to " << GetCli().GetKeyword() << "!" << CLI_ENDL
    << CLI_ENDL
    << " " << GetCli().GetKeyword() << " is a command line interface" << CLI_ENDL
    << " using the LGPL CLI library" << CLI_ENDL
    << "   (c) Alexis Royer http://alexis.royer.free.fr/cli/" << CLI_ENDL
    << " Type 'help' at any time" << CLI_ENDL
    << " or press '?' or TAB to get completion or help." << CLI_ENDL
    << "---------------------------------------------------" << CLI_ENDL;
}

void CcliShell::PromptByeMessage(void) const
{
    GetWelcomeStream() << "Bye!" << CLI_ENDL;
}

void CcliShell::PromptMenu(void) const
{
    if (! m_vpcliMenus.empty())
    {
        GetPromptStream() << m_vpcliMenus.back()->GetKeyword() << ">" << m_strLine;
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
    m_vpcliMenus.clear();
}

void CcliShell::DisplayHelp(void)
{
    if (! m_vpcliMenus.empty())
    {
        std::string str_Mem = m_strLine;
        m_strLine.erase();
        OnHelp(false);
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

void CcliShell::OnKey(char C_KeyCode)
{
    switch (C_KeyCode)
    {
    //case KeyEvent.VK_UP:            break;
    //case KeyEvent.VK_KP_UP:         break;
    //case KeyEvent.VK_DOWN:          break;
    //case KeyEvent.VK_KP_DOWN:       break;
    //case KeyEvent.VK_RIGHT:         break;
    //case KeyEvent.VK_KP_RIGHT:      break;
    //case KeyEvent.VK_LEFT:          break;
    //case KeyEvent.VK_KP_LEFT:       break;

    //case KeyEvent.VK_BEGIN:         break;
    //case KeyEvent.VK_HOME:          break;
    //case KeyEvent.VK_END:           break;

    //case KeyEvent.VK_CLEAR:         break;
    //case KeyEvent.VK_INSERT:        break;
    //case KeyEvent.VK_BACK_SPACE:
    case '\b':
    case 127:
        OnBackspace();
        break;

    //case KeyEvent.VK_CONTROL:       break;
    //case KeyEvent.VK_CUT:           break;
    //case KeyEvent.VK_COPY:          break;
    //case KeyEvent.VK_PASTE:         break;
    //case KeyEvent.VK_UNDO:          break;

    //case KeyEvent.VK_ENTER:
    case '\r': case '\n':
        OnExecute();
        break;

    //case KeyEvent.VK_STOP:          break;
    //case KeyEvent.VK_CANCEL:
    //case KeyEvent.VK_ESCAPE:
    case 3:     // Ctrl+C
    case 27:    // ESC
        OnEscape();
        break;

    case 4:     // Ctrl+D
        OnExit(true);
        break;

    //case KeyEvent.VK_HELP:
    //case KeyEvent.VK_TAB:
    case '?':
    case '\t':
        OnHelp(true);
        break;

    //case KeyEvent.VK_A:
    //case KeyEvent.VK_B:
    //case KeyEvent.VK_C:
    //case KeyEvent.VK_D:
    //case KeyEvent.VK_E:
    //case KeyEvent.VK_F:
    //case KeyEvent.VK_G:
    //case KeyEvent.VK_H:
    //case KeyEvent.VK_I:
    //case KeyEvent.VK_J:
    //case KeyEvent.VK_K:
    //case KeyEvent.VK_L:
    //case KeyEvent.VK_M:
    //case KeyEvent.VK_N:
    //case KeyEvent.VK_O:
    //case KeyEvent.VK_P:
    //case KeyEvent.VK_Q:
    //case KeyEvent.VK_R:
    //case KeyEvent.VK_S:
    //case KeyEvent.VK_T:
    //case KeyEvent.VK_U:
    //case KeyEvent.VK_V:
    //case KeyEvent.VK_W:
    //case KeyEvent.VK_X:
    //case KeyEvent.VK_Y:
    //case KeyEvent.VK_Z:
    //case KeyEvent.VK_0:
    //case KeyEvent.VK_1:
    //case KeyEvent.VK_2:
    //case KeyEvent.VK_3:
    //case KeyEvent.VK_4:
    //case KeyEvent.VK_5:
    //case KeyEvent.VK_6:
    //case KeyEvent.VK_7:
    //case KeyEvent.VK_8:
    //case KeyEvent.VK_9:
    default:
        OnPrintableChar(C_KeyCode);
        break;
    }
}

void CcliShell::OnPrintableChar(char C_KeyCode)
{
    if (((C_KeyCode >= 'a') && (C_KeyCode <= 'z'))
        || ((C_KeyCode >= 'A') && (C_KeyCode <= 'Z'))
        || ((C_KeyCode >= '0') && (C_KeyCode <= '9'))
        || (C_KeyCode == ' ')
        || (C_KeyCode == '-')
        || (C_KeyCode == '_')
        || (C_KeyCode == '*')
        || (C_KeyCode == '+')
        || (C_KeyCode == '/')
        || (C_KeyCode == ':')
        || (C_KeyCode == '@')
        || (C_KeyCode == '#')
        || (C_KeyCode == '&')
        || (C_KeyCode == '!')
        || (C_KeyCode == '(')
        || (C_KeyCode == ')')
        || (C_KeyCode == '$')
        || (C_KeyCode == '.')
        || (C_KeyCode == ',')
        || (C_KeyCode == ';')
        || (C_KeyCode == '[')
        || (C_KeyCode == ']')
        || (C_KeyCode == '{')
        || (C_KeyCode == '}')
        || (C_KeyCode == '<')
        || (C_KeyCode == '>')
        || (C_KeyCode == '=')
        )
    {
        PrintLine(C_KeyCode);
    }
    else
    {
        GetErrorStream() << CLI_ENDL;
        GetErrorStream() << "unknown character [" << (int) C_KeyCode << "]" << CLI_ENDL;
        if (! m_vpcliMenus.empty())
        {
            PromptMenu();
        }
    }
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

void CcliShell::OnExit(bool B_Edition)
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

void CcliShell::OnHelp(bool B_Edition)
{
    if (! m_vpcliMenus.empty())
    {
        CcliCommandLine cli_CommandLine;
        if (cli_CommandLine.Parse(*m_vpcliMenus.back(), m_strLine, false))
        {
            CcliElementList cli_Elements;
            if (! cli_CommandLine.GetLastElement().FindElements(
                    cli_Elements, cli_CommandLine.GetLastWord()))
            {
                return;
            }

            bool b_PrintHelpList = false;
            if (! cli_CommandLine.GetLastWord().empty())
            {
                if (cli_Elements.size() == 0)
                {
                    Beep();
                    b_PrintHelpList = true;
                }
                else if (cli_Elements.size() == 1)
                {
                    Backspace(cli_CommandLine.GetLastWord().size());
                    PrintLine(cli_Elements.front()->GetKeyword() + " ");
                }
                else
                {
                    b_PrintHelpList = true;
                }
            }
            else
            {
                b_PrintHelpList = true;
            }

            if (b_PrintHelpList)
            {
                if (B_Edition)
                {
                    GetEchoStream() << CLI_ENDL;
                }
                for (int i=0; i<cli_Elements.size(); i++)
                {
                    PrintHelp(*cli_Elements[i]);
                }
                if (B_Edition)
                {
                    PromptMenu();
                }
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
        CcliCommandLine cli_CommandLine;
        if (cli_CommandLine.Parse(*m_vpcliMenus.back(), m_strLine + "\n", true))
        {
            bool b_Executed = false;
            if (cli_CommandLine.GetElementCount() > 1)
            {
                const CcliMenu* pcli_Menu = m_vpcliMenus.back();

                if (pcli_Menu->ExecuteReserved(cli_CommandLine))
                    b_Executed = true;
                if (pcli_Menu->Execute(cli_CommandLine))
                    b_Executed = true;
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
            m_strLine.erase();
        }
        PromptMenu();
    }
    else
    {
        assert(false);
        GetErrorStream() << "No current menu!" << CLI_ENDL;
    }
}

void CcliShell::PrintLine(const std::string& STR_Append)
{
    m_strLine += STR_Append;
    GetEchoStream() << STR_Append;
}

void CcliShell::PrintLine(char C_Append)
{
    PrintLine(std::string() += C_Append);
}

void CcliShell::Backspace(int I_BackspaceCount)
{
    if (I_BackspaceCount > m_strLine.size())
    {
        I_BackspaceCount = m_strLine.size();
    }

    m_strLine = m_strLine.substr(0, m_strLine.size() - I_BackspaceCount);
    for (int i=0; i<I_BackspaceCount; i++)
    {
        GetEchoStream() << "\b" << " " << "\b";
    }
}

void CcliShell::PrintHelp(const CcliElement& CLI_Element)
{
    int i = 0;
    std::string str_Keyword = CLI_Element.GetKeyword();
    std::string str_Help = CLI_Element.GetHelp().GetHelp(GetLang());

    if (dynamic_cast<const CcliEndl*>(& CLI_Element))
    {
        str_Keyword = "<cr>";
    }

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
}

