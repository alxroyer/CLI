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
//! @brief CcliCli class definition.

//! @mainpage
//! The CLI library allows you to easily define Command Line Interfaces.
//!
//! Within a single XML file, you define the tree of menus, keywords and parameters, and match them directly to some target language code.
//!
//! Different input/output devices are available:
//!     - console
//!     - TCP
//!     - RS232 (todo)
//!     - user defined input/output.
//!
//! This documentation gives information about the implementation of this library for C++. Here are pieces of advice to quickly get the essential of it:
//!     - First of all, you might be interested in the CcliCli class, which defines a CLI structure.
//!     - Then have a look at the CcliShell and CcliCommandLine classes. These classes implement CLI executions.
//!     - Get back to the CcliMenu class and find out how user code is executed through the handlers. It is also interesting at this point to have a look at the XSL transformation.
//!     - Eventually go to the CcliIODevice class and its child classes CcliConsole and CcliTelnet. CcliIODevice defines a generic input/output interface you may implement for special needs.

#ifndef _CLI_CLI_H_
#define _CLI_CLI_H_

#include <string>
#include <vector>

#include "cli_menu.h"

// Forward declarations
class CcliCli;
class CcliConsole;
class CcliHelp;
class CcliConfigMenu;


// Typedefs.
//! @brief CLI list type.
typedef std::deque<const CcliCli*> CcliCliList;


//! @brief CLI definition class.
//!
//! An object of this class defines a CLI structure.
//! Since a CcliCli object defines the main menu, it inherits from CcliMenu.
//! On the other hand, this class owns all sub-menus through the member m_vpcliMenus.
//! Finally, since CcliMenu inherits from CcliSyntaxNode,
//! it also owns a collection of possible CcliElement objects
//! that define the commands you can type in the first place.
//!
//! Since you should override the execution handler, this class should be inherited.
//! That's the reason why the destructor is pure virtual.
class CcliCli : public CcliMenu
{
public:
    //! @brief Constructor.
    CcliCli(
        const std::string& STR_Name,    //!< Name of the CLI.
        const CcliHelp& CLI_Help        //!< Help object.
        );

    //! @brief Destructor.
    //!
    //! Pure virtual. CcliCli class is not final.
    virtual ~CcliCli(void) = 0;

public:
    //! @brief Retrieve CLI references from their names.
    //! @return The number of CcliCli instances found.
    static const int FindFromName(
        CcliCliList& CLI_CliList,       //!< Output list.
        const std::string& STR_RegExp   //!< Regular expression matching the searched CLI names.
        );

public:
    //! @brief CLI name access.
    const std::string GetName(void) const;

public:
    //! @brief Menu addition.
    CcliMenu& AddMenu(
        CcliMenu* const PCLI_Menu       //!< New menu.
        );

public:
    //! @brief Set the shell reference when executing.
    void SetShell(
        CcliShell& CLI_Shell    //!< Shell reference.
	) const;
    //! @brief Returns the shell reference on execution.
    CcliShell& GetShell(void) const;

    //! @brief CLI reference setting.
    virtual void SetCli(CcliCli& CLI_Cli);
    //! @brief Reserved commands execution.
    virtual const bool ExecuteReserved(const CcliCommandLine& CLI_CommandLine) const;
    //! @brief Handler on menu exit.
    virtual void OnExit(void) const;

private:
    //! @brief Configuration menu access.
    CcliConfigMenu& GetConfigMenu(void);
    //! @brief Configuration menu access (read-only).
    const CcliConfigMenu& GetConfigMenu(void) const;

    //! @brief Configuration menu node access.
    CcliKeyword& GetConfigMenuNode(void);
    //! @brief Configuration menu node access (read-only).
    const CcliKeyword& GetConfigMenuNode(void) const;

private:
    static CcliCliList M_CLIList;

private:
    //! Shell reference.
    mutable CcliShell* m_pcliShell;
    //! Owned menu collection.
    std::vector<const CcliMenu*> m_vpcliMenus;
    //! Configuration menu.
    CcliConfigMenu* m_pcliConfigMenu;
    //! Configuration menu node.
    CcliKeyword* m_pcliConfigMenuNode;
};

#endif // _CLI_CLI_H_

