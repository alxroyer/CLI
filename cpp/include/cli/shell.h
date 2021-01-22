/*
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006-2007 Alexis Royer.

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
//! @brief Shell class definition.

#ifndef _CLI_SHELL_H_
#define _CLI_SHELL_H_

#include <vector>

#include <cli/help.h>
#include <cli/io_device.h>


namespace cli {

    // Forward declarations.
    class Cli;
    class Menu;
    class Element;
    class OutputDevice;
    class IODevice;


    //! @brief Output stream enumeration.
    typedef enum
    {
        ALL_STREAMS = -1,   //!< All streams.

        WELCOME_STREAM = 0, //!< Welcome stream. Useful for bye too.
        PROMPT_STREAM,      //!< Prompt stream.
        ECHO_STREAM,        //!< Echo stream.
        OUTPUT_STREAM,      //!< Output stream.
        ERROR_STREAM,       //!< Error stream.
        STREAM_TYPES_COUNT  //!< Number of streams.
    } STREAM_TYPE;


    //! @brief Shell management.
    class Shell
    {
    public:
        //! @brief Constructor.
        Shell(
            const Cli& CLI_Cli  //!< CLI reference.
            );

        //! @brief Destructor.
        virtual ~Shell(void);

    public:
        //! @brief CLI access.
        const Cli& GetCli(void) const;

        //! @brief Output stream access.
        const OutputDevice& GetStream(
            const STREAM_TYPE E_StreamType          //!< Output stream identifier.
            ) const;
        //! @brief Output stream positionning.
        //! @warning Please ensure one of the following conditions regarding the given device:
        //!     - Either the device is an auto-deleted device,
        //!     - or it will be destroyed after this shell object,
        //!     - or another call to this method with the null device is done on termination.
        //! Otherwise you could experience consistency troubles.
        //! The null device and standard devices are not subject to this remark.
        const bool SetStream(
            const STREAM_TYPE E_StreamType,         //!< Output stream identifier.
            OutputDevice& CLI_Stream                //!< Stream reference.
            );
        //! @brief Enabled/disabled stream accessor.
        const bool StreamEnabled(
            const STREAM_TYPE E_StreamType          //!< Output stream identifier.
            ) const;
        //! @brief Enable/disable stream.
        const bool EnableStream(
            const STREAM_TYPE E_StreamType,         //!< Output stream identifier.
            const bool B_Enable                     //!< Enable flag.
            );

        //! @brief Welcome message setting.
        void SetWelcomeMessage(const std::string& STR_WelcomeMessage);
        //! @brief Bye message setting.
        void SetByeMessage(const std::string& STR_ByeMessage);
        //! @brief Prompt message positionning.
        void SetPrompt(const std::string& STR_Prompt);

        //! @brief Language setting.
        void SetLang(const Help::LANG E_Lang);
        //! @brief Language access.
        const Help::LANG GetLang(void) const;

        //! @brief Beep configuration setting.
        void SetBeep(const bool B_Enable);
        //! @brief Beep configuration access.
        const bool GetBeep(void) const;

    public:
        //! @brief Runs the shell onto the corresponding input/output device.
        void Run(
            IODevice& CLI_IODevice      //!< Input/output device to run onto.
            );

        //! @brief Tells whether this shell is running or not.
        const bool IsRunning(void) const;

    public:
        //! @brief Help margin accessor.
        const unsigned int GetHelpMargin(void) const;

        //! @brief Help offset accessor.
        const unsigned int GetHelpOffset(void) const;

    private:
        //! @brief Assumes input and output devices to be ready for execution.
        const bool OpenDevices(
            IODevice& CLI_IODevice      //!< Input/output device to run onto.
            );
        //! @brief Release input and output devices after execution.
        const bool CloseDevices(
            IODevice& CLI_IODevice      //!< Input/output device to run onto.
            );

        //! @brief Prints the welcome message.
        void PromptWelcomeMessage(void) const;
        //! @brief Prints the bye message.
        void PromptByeMessage(void) const;
        //! @brief Prints the prompt message, indicating the current menu.
        void PromptMenu(void) const;

    private:
        friend class Menu;
        //! @brief Enters a menu.
        void EnterMenu(
            const Menu& CLI_Menu    //!< Menu to enter.
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
        void OnKey(const KEY E_KeyCode);
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
            const Element& CLI_Element      //!< Element to print the help for.
            );
        //! @brief Sends a beep signal.
        void Beep(void);
        //! @brief Pushes a command line in the history stack.
        void PushHistory(
            const std::string& STR_Line     //!< Command line to push.
            );

    private:
        //! CLI reference.
        const Cli* const m_pcliCli;
        //! Input device.
        IODevice* m_pcliInput;
        //! Output streams.
        struct {
            OutputDevice* pcliStream;
            bool bEnable;
        } m_artStream[STREAM_TYPES_COUNT];
        //! Welcome message.
        std::string m_strWelcomeMessage;
        //! Bye message.
        std::string m_strByeMessage;
        //! Non-default Prompt.
        std::string m_strNoDefaultPrompt;
        //! Current language.
        Help::LANG m_eLang;
        //! Beep enabled.
        bool m_bBeep;
        //! Menu stack.
        std::vector<const Menu*> m_vpcliMenus;
        //! Current line.
        std::string m_strLine;
        //! History.
        std::vector<std::string> m_vstrHistory;
        //! History index.
        int m_iHistoryIndex;
    };

};

#endif // _CLI_SHELL_H_

