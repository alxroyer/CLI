/*
    Copyright (c) 2006-2009, Alexis Royer, http://alexis.royer.free.fr/CLI

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


//! @file
//! @author Alexis Royer
//! @brief Shell class definition.

#ifndef _CLI_SHELL_H_
#define _CLI_SHELL_H_

#include "cli/namespace.h"
#include "cli/object.h"
#include "cli/help.h"
#include "cli/io_device.h"
#include "cli/tk.h"


CLI_NS_BEGIN(cli)

    // Forward declarations.
    class Cli;
    class Menu;
    class Element;
    class OutputDevice;
    class IODevice;
    class MonoThreadDevice;
    class CmdLineEdition;
    class CmdLineHistory;


    //! @brief Output stream enumeration.
    typedef enum _STREAM_TYPE
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
    class Shell : public Object
    {
    private:
        //! @brief No default constructor.
        Shell(void);
        //! @brief No copy constructor.
        Shell(const Shell&);

    public:
        //! @brief Constructor.
        Shell(
            const Cli& CLI_Cli  //!< CLI reference.
            );

        //! @brief Destructor.
        virtual ~Shell(void);

    private:
        //! @brief No assignment operator.
        Shell& operator=(const Shell&);

    public:
        //! @brief CLI access.
        const Cli& GetCli(void) const;

        //! @brief Input stream access.
        const IODevice& GetInput(void) const;
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
        void SetWelcomeMessage(
            const ResourceString& CLI_WelcomeMessage    //!< Welcome message.
                                                        //!< When an empty string is given, the default
                                                        //!< welcome message is restored.
            );
        //! @brief Bye message setting.
        void SetByeMessage(
            const ResourceString& CLI_ByeMessage        //!< Bye message.
                                                        //!< When an empty string is given, the default
                                                        //!< bye message is restored.
            );
        //! @brief Prompt message positionning.
        void SetPrompt(
            const ResourceString& CLI_Prompt            //!< Prompt string.
                                                        //!< When an empty string is given, the default
                                                        //!< prompt (depending on the current menu)
                                                        //!< is restored.
            );
        //! @brief Error formatting.
        void SetErrorFormatting(
            const ResourceString& CLI_LocationPrefix,   //!< Very first error prefix string, before the location.
                                                        //!< No error prefixing when the string given is empty.
            const ResourceString& CLI_ErrorPrefix,      //!< Error prefix string.
                                                        //!< No error prefixing when the string given is empty.
            const ResourceString& CLI_ErrorSuffix       //!< Error suffix string.
                                                        //!< No error suffixing when the string given is empty.
            );

        //! @brief Language setting.
        void SetLang(
            const ResourceString::LANG E_Lang   //!< New value.
            );
        //! @brief Language access.
        //! @return The language currently set.
        const ResourceString::LANG GetLang(void) const;

        //! @brief Beep configuration setting.
        void SetBeep(
            const bool B_Enable         //!< New value.
            );
        //! @brief Beep configuration access.
        //! @return The current beep configuration. true if enabled, false otherwise.
        const bool GetBeep(void) const;

    public:
        //! @brief Runs the shell onto the corresponding input/output device.
        void Run(
            IODevice& CLI_IODevice      //!< Input/output device to run onto.
            );

        //! @brief Tells whether this shell is running or not.
        //! @return The running status.
        const bool IsRunning(void) const;

    public:
        //! @brief Help margin accessor.
        //! @return Number of spaces for the help margin.
        const unsigned int GetHelpMargin(void) const;

        //! @brief Help offset accessor.
        //! @return Number of spaces for the help offset.
        const unsigned int GetHelpOffset(void) const;

    private:
        //! @brief Assumes input and output devices to be ready for execution.
        const bool OpenDevices(
            IODevice& CLI_IODevice      //!< Input/output device to run onto.
            );
        //! @brief Release input and output devices after execution.
        const bool CloseDevices(void);

        //! @brief Prints the welcome message.
        void PromptWelcomeMessage(void) const;
        //! @brief Prints the bye message.
        void PromptByeMessage(void) const;
        //! @brief Prints the prompt message, indicating the current menu.
        void PromptMenu(void) const;
        //! @brief Error display.
        void PrintError(
            const ResourceString& CLI_Location,     //!< Error location
            const ResourceString& CLI_ErrorMessage  //!< Error message
            ) const;

    private:
        //! @brief Enters a menu.
        void EnterMenu(
            const Menu& CLI_Menu    //!< Menu to enter.
            );

    public:
        //! @brief Exits the current menu.
        void ExitMenu(void);

        //! @brief Terminates the shell.
        //! @warning Not thread safe. Call QuitThreadSafe() to do so.
        void Quit(void);

        //! @brief Terminates the shell.
        //! @warning The shell will actually quit as soon as the input device return a character.
        void QuitThreadSafe(void);

        //! @brief Displays help depending on the context of the current line.
        void DisplayHelp(void);

        //! @brief Prints the working menu.
        void PrintWorkingMenu(void);

    private:
        //! @brief Beginning of execution.
        const bool StartExecution(
            IODevice& CLI_IODevice              //!< Input/output device to run onto.
            );
        //! @brief Main loop executed when the input device is not a mono-thread device.
        void MainLoop(void);
        //! @brief Hook called by mono-thread devices on character input.
        void OnMonoThreadKey(
            MonoThreadDevice& CLI_Source,       //!< Input mono-thread device.
            const KEY E_KeyCode                 //!< Input key.
            );
        //! @brief Shell termination.
        const bool FinishExecution(void);

    private:
        //! @brief Called when a character comes up from the input device.
        void OnKey(const KEY E_KeyCode);
        //! @brief Called when a printable character comes up from the input device.
        void OnPrintableChar(const char C_KeyCode);
        //! @brief Called when using the 'home'/'begin' key.
        void OnKeyBegin(void);
        //! @brief Called when using the 'end' key.
        void OnKeyEnd(void);
        //! @brief Called when using the left arrow.
        void OnKeyLeft(void);
        //! @brief Called when using the right arrow.
        void OnKeyRight(void);
        //! @brief Clean screen routine.
        void OnCleanScreen(void);
        //! @brief Called when a backspace comes up from the input device.
        void OnBackspace(void);
        //! @brief Called when a suppr comes up from the input device.
        void OnSuppr(void);
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
            const int I_Navigation          //!< Number of steps in history navigation.
                                            //!< Positive values mean navigating to older command lines.
                                            //!< Negative values mean navigating to newer command lines.
            );

    private:
        //! @brief Prints an help line for a given element.
        void PrintHelp(
            const Element& CLI_Element      //!< Element to print the help for.
            );
        //! @brief Sends a beep signal.
        void Beep(void);

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
        ResourceString m_cliWelcomeMessage;
        //! Bye message.
        ResourceString m_cliByeMessage;
        //! Non-default Prompt.
        ResourceString m_cliNoDefaultPrompt;
        //! Error formatting: prefix and suffix.
        ResourceString m_cliErrorFormatting[3];
        //! Current language.
        ResourceString::LANG m_eLang;
        //! Beep enabled.
        bool m_bBeep;
        //! Menu stack.
        tk::Queue<const Menu*> m_qMenus;
        //! Current line.
        CmdLineEdition& m_cliCmdLine;
        //! History.
        CmdLineHistory& m_cliHistory;
        //! Thread safe command.
        enum {
            THREAD_SAFE_NONE,   //!< No current thread safe command.
            THREAD_SAFE_QUIT    //!< Have the shell quit as soon as possible.
        } m_eThreadSafeCmd;
        //! Final traces restoration.
        bool m_bRestoreTracesOnFinish;

    private:
        // In order to allow access to private method OnMonoThreadKey().
        friend class MonoThreadDevice;
    };

CLI_NS_END(cli)

#endif // _CLI_SHELL_H_

