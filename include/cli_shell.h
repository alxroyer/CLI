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


#ifndef _CLI_SHELL_H_
#define _CLI_SHELL_H_

#include <vector>

#include "cli_shell.h"
#include "cli_help.h"


class CcliCli;
class CcliMenu;
class CcliElement;
class CcliOutputDevice;
class CcliIODevice;


class CcliShell
{
public:
    CcliShell(CcliCli& CLI_Cli);
    virtual ~CcliShell(void);

public:
    const CcliCli& GetCli(void) const;
    const CcliOutputDevice& GetWelcomeStream(void) const;
    const CcliOutputDevice& GetPromptStream(void) const;
    const CcliOutputDevice& GetEchoStream(void) const;
    const CcliOutputDevice& GetOutputStream(void) const;
    const CcliOutputDevice& GetErrorStream(void) const;
    void SetLang(CcliHelp::CLI_LANG E_Lang);
    CcliHelp::CLI_LANG GetLang(void) const;

public:
    void RunConsole(void);
    void RunTelnet(unsigned long UL_TcpPort);

private:
    friend class CcliTelnetServer;
    void Run(CcliIODevice& CLI_IODevice);
    void PromptWelcomeMessage(void) const;
    void PromptByeMessage(void) const;
    void PromptMenu(void) const;

private:
    friend class CcliMenu;
    void EnterMenu(const CcliMenu& CLI_Menu);
    void ExitMenu(void);
    void Quit(void);
    void DisplayHelp(void);
    void PrintWorkingMenu(void);

private:
    void OnKey(char C_KeyCode);
    void OnPrintableChar(char C_KeyCode);
    void OnBackspace(void);
    void OnEscape(void);
    void OnExit(bool B_Edition);
    void OnHelp(bool B_Edition);
    void OnExecute(void);

private:
    void PrintLine(const std::string& STR_Append);
    void PrintLine(char C_Append);
    void Backspace(int I_BackspaceCount);
    void PrintHelp(const CcliElement& CLI_Element);
    void Beep(void);

private:
    typedef enum
    {
        CLI_WELCOME_STREAM,
        CLI_PROMPT_STREAM,
        CLI_ECHO_STREAM,
        CLI_OUTPUT_STREAM,
	CLI_ERROR_STREAM,
	CLI_STREAM_TYPES_COUNT
    } CLI_STREAM_TYPE;

private:
    const CcliCli* m_pcliCli;
    const CcliOutputDevice* m_arpcliStream[CLI_STREAM_TYPES_COUNT];
    CcliHelp::CLI_LANG m_eLang;
    std::vector<const CcliMenu*> m_vpcliMenus;
    std::string m_strLine;
};

#endif // _CLI_SHELL_H_

