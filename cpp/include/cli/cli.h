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
//! @brief Cli class definition.

//! @mainpage
//! The CLI library allows you to easily define Command Line Interfaces.
//!
//! Within a single XML file, you define the tree of menus, keywords and parameters, and match them directly to some target language code.
//!
//! Different input/output devices are available:
//!     - console:                      cli::Console
//!     - TCP:                          cli::TelnetServer and cli::TelnetConnection
//!     - files:                        cli::InputFileDevice and cli::OutputFileDevice
//!     - single command:               cli::SingleCommand
//!     - RS232 (todo)
//!     - user defined input/output:    cli::IODevice
//!
//! This documentation gives information about the implementation of this library for C++. Here are pieces of advice to quickly get the essential of it:
//!     - First of all, you might be interested in the cli::Cli class, which defines a CLI structure.
//!     - Then have a look at the cli::Shell and cli::CommandLine classes. These classes implement CLI executions.
//!     - Get back to the cli::Menu class and find out how user code is executed through the handlers. It is also interesting at this point to have a look at the XSL transformation.
//!     - Eventually go to the cli::IODevice class and its child classes.
//!       You may also implement this generic input/output interface for special needs.

#ifndef _CLI_CLI_H_
#define _CLI_CLI_H_

#include <string>
#include <vector>

#include <cli/menu.h>


//! @brief Main namespace of the CLI library.
namespace cli {

    // Forward declarations
    class Cli;
    class Console;
    class Help;
    class ConfigMenu;
    class TracesMenu;


    // Typedefs.
    //! @brief CLI list type.
    typedef std::deque<const Cli*> CliList;


    //! @brief CLI definition class.
    //!
    //! An object of this class defines a CLI structure.
    //! Since a Cli object defines the main menu, it inherits from Menu.
    //! On the other hand, this class owns all sub-menus through the member m_vpcliMenus.
    //! Finally, since Menu inherits from SyntaxNode,
    //! it also owns a collection of possible Element objects
    //! that define the commands you can type in the first place.
    //!
    //! Since you should override the execution handler, this class should be inherited.
    //! That's the reason why the destructor is pure virtual.
    class Cli : public Menu
    {
    public:
        //! @brief Constructor.
        Cli(
            const std::string& STR_Name,    //!< Name of the CLI.
            const Help& CLI_Help            //!< Help object.
            );

        //! @brief Destructor.
        //!
        //! Pure virtual. Cli class is not final.
        virtual ~Cli(void) = 0;

    public:
        //! @brief Retrieve CLI references from their names.
        //! @return The number of Cli instances found.
        static const int FindFromName(
            CliList& CLI_CliList,           //!< Output list.
            const std::string& STR_RegExp   //!< Regular expression matching the searched CLI names.
            );

    public:
        //! @brief CLI name access.
        const std::string GetName(void) const;

    public:
        //! @brief Menu addition.
        Menu& AddMenu(
            Menu* const PCLI_Menu       //!< New menu.
            );

    public:
        //! @brief Set the shell reference when executing.
        void SetShell(
            Shell& CLI_Shell    //!< Shell reference.
            ) const;
        //! @brief Returns the shell reference on execution.
        Shell& GetShell(void) const;

        //! @brief CLI reference setting.
        virtual void SetCli(Cli& CLI_Cli);
        //! @brief Reserved commands execution.
        virtual const bool ExecuteReserved(const CommandLine& CLI_CommandLine) const;
        //! @brief Handler on menu exit.
        virtual void OnExit(void) const;

    private:
        //! @brief Configuration menu access.
        ConfigMenu& GetConfigMenu(void);
        //! @brief Configuration menu access (read-only).
        const ConfigMenu& GetConfigMenu(void) const;
        //! @brief Configuration menu node access (read-only).
        const Keyword& GetConfigMenuNode(void) const;

        #ifdef _DEBUG
        //! @brief Traces menu access.
        TracesMenu& GetTracesMenu(void);
        //! @brief Traces menu access (read-only).
        const TracesMenu& GetTracesMenu(void) const;
        //! @brief Configuration menu node access (read-only).
        const Keyword& GetTracesMenuNode(void) const;
        #endif

    private:
        //! Shell reference.
        mutable Shell* m_pcliShell;
        //! Owned menu collection.
        std::vector<const Menu*> m_vpcliMenus;
        //! Configuration menu.
        ConfigMenu* m_pcliConfigMenu;
        //! Configuration menu node.
        Keyword* m_pcliConfigMenuNode;
        #ifdef _DEBUG
        //! Traces menu.
        TracesMenu* m_pcliTracesMenu;
        //! Traces menu node.
        Keyword* m_pcliTracesMenuNode;
        #endif
    };

};

#endif // _CLI_CLI_H_

