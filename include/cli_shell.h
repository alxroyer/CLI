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


//! @file
//! @author Alexis Royer
//! @brief CcliShell class definition.

#ifndef _CLI_SHELL_H_
#define _CLI_SHELL_H_

#include <vector>

#include "cli_help.h"
#include "cli_io_device.h"


// Forward declarations.
class CcliCli;
class CcliMenu;
class CcliElement;
class CcliOutputDevice;
class CcliIODevice;


//! @brief Shell management.
class CcliShell
{
public:
    //! @brief Constructor.
    CcliShell(
        const CcliCli& CLI_Cli  //!< CLI reference.
        );

    //! @brief Destructor.
    virtual ~CcliShell(void);

public:
    //! @brief CLI access.
    const CcliCli& GetCli(void) const;

    //! @brief Welcome stream access.
    const CcliOutputDevice& GetWelcomeStream(void) const;
    //! @brief Welcome stream positionning.
    void SetWelcomeStream(const CcliOutputDevice& CLI_WelcomeStream);
    //! @brief Welcome message setting.
    void SetWelcomeMessage(const std::string& STR_WelcomeMessage);
    //! @brief Bye message setting.
    void SetByeMessage(const std::string& STR_ByeMessage);

    //! @brief Prompt stream access.
    const CcliOutputDevice& GetPromptStream(void) const;
    //! @brief Prompt sream positionning
    void SetPromptStream(const CcliOutputDevice& CLI_PromptStream);
    //! @brief Prompt message positionning.
    void SetPrompt(const std::string& STR_Prompt);

    //! @brief Echo stream access.
    const CcliOutputDevice& GetEchoStream(void) const;
    //! @brief Echo stream positionning.
    void SetEchoStream(const CcliOutputDevice& CLI_EchoStream);

    //! @brief Output stream access.
    const CcliOutputDevice& GetOutputStream(void) const;
    //! @brief Output stream positionning.
    void SetOutputStream(const CcliOutputDevice& CLI_OutputStream);

    //! @brief Error stream access.
    const CcliOutputDevice& GetErrorStream(void) const;
    //! @brief Error stream positionning.
    void SetErrorStream(const CcliOutputDevice& CLI_ErrorStream);

    //! @brief Language setting.
    void SetLang(const CcliHelp::CLI_LANG E_Lang);
    //! @brief Language access.
    const CcliHelp::CLI_LANG GetLang(void) const;

    //! @brief Beep configuration setting.
    void SetBeep(const bool B_Enable);
    //! @brief Beep configuration access.
    const bool GetBeep(void) const;

public:
    //! @brief Runs the shell onto the corresponding input/output device.
    void Run(
        CcliIODevice& CLI_IODevice  //!< Input/output device to run onto.
        );

private:
    //! @brief Prints the welcome message.
    void PromptWelcomeMessage(void) const;
    //! @brief Prints the bye message.
    void PromptByeMessage(void) const;
    //! @brief Prints the prompt message, indicating the current menu.
    void PromptMenu(void) const;

private:
    friend class CcliMenu;
    //! @brief Enters a menu.
    void EnterMenu(
        const CcliMenu& CLI_Menu    //!< Menu to enter.
        );
    //! @brief Exits the current menu.
    void ExitMenu(void);
    //! @brief Terminates the shell.
    void Quit(void);
    //! @brief Displays help depending on the context of the current line.
    void DisplayHelp(void);
    //! @brief Prints the working menu.
    void PrintWorkingMenu(void);

private:
    //! @brief Called when a character comes up from the input device.
    void OnKey(const CLI_CHAR E_KeyCode);
    //! @brief Called when a printable character comes up from the input device.
    void OnPrintableChar(const char C_KeyCode);
    //! @brief Called when a backspace comes up from the input device.
    void OnBackspace(void);
    //! @brief Called when an escape character comes up from the input device.
    void OnEscape(void);
    //! @brief Called when an exit character (CTRL+C) comes up from the input device.
    void OnExit(const bool B_Edition);
    //! @brief Called when an help character ('?' or TAB) comes up from the input device.
    //!
    //! This method manages both help and completion.
    void OnHelp(
        const bool B_Edition,   //!< true when this method is called in the context
                                //!< of the command line edition.
        const bool B_HelpOnly   //!< true means no completion.
        );
    //! @brief Called when an execute character (ENTER) comes up from the input device.
    void OnExecute(void);
    //! @brief Moves in history.
    void OnHistory(
        const int I_Translation         //!< Number of steps in translation.
        );

private:
    //! @brief Appends the current line.
    void PrintLine(
        const std::string& STR_Append   //!< String to append.
        );
    //! @brief Appends the current line.
    void PrintLine(
        const char C_Append             //!< Character to append.
        );
    //! @brief Deletes last characters of the current line.
    void Backspace(
        const int I_BackspaceCount      //!< Number of characters to delete.
        );
    //! @brief Prints an help line for a given element.
    void PrintHelp(
        const CcliElement& CLI_Element  //!< Element to print the help for.
        );
    //! @brief Sends a beep signal.
    void Beep(void);
    //! @brief Pushes a command line in the history stack.
    void PushHistory(
        const std::string& STR_Line     //!< Command line to push.
        );

private:
    //! @brief Streams enumeration.
    typedef enum
    {
        CLI_WELCOME_STREAM,     //!< Welcome stream. Useful for bye too.
        CLI_PROMPT_STREAM,      //!< Prompt stream.
        CLI_ECHO_STREAM,        //!< Echo stream.
        CLI_OUTPUT_STREAM,      //!< Output stream.
	CLI_ERROR_STREAM,       //!< Error stream.
	CLI_STREAM_TYPES_COUNT  //!< Number of treams.
    } CLI_STREAM_TYPE;

private:
    //! CLI reference.
    const CcliCli* const m_pcliCli;
    //! Streams references.
    const CcliOutputDevice* m_arpcliStream[CLI_STREAM_TYPES_COUNT];
    //! Welcome message.
    std::string m_strWelcomeMessage;
    //! Bye message.
    std::string m_strByeMessage;
    //! Non-default Prompt.
    std::string m_strNoDefaultPrompt;
    //! Current language.
    CcliHelp::CLI_LANG m_eLang;
    //! Beep enabled.
    bool m_bBeep;
    //! Menu stack.
    std::vector<const CcliMenu*> m_vpcliMenus;
    //! Current line.
    std::string m_strLine;
    //! History.
    std::vector<std::string> m_vstrHistory;
    //! History index.
    int m_iHistoryIndex;
};

#endif // _CLI_SHELL_H_

