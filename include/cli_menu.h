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
//! @brief CcliMenu and CcliMenuRef classes definition.

#ifndef _CLI_MENU_H_
#define _CLI_MENU_H_

#include <string>
#include "cli_syntax_node.h"


// Forward declarations.
class CcliCli;
class CcliConsole;
class CcliCommandLine;
class CcliKeyword;
class CcliHelp;


//! @brief Menu definition.
class CcliMenu : public CcliSyntaxNode
{
public:
    //! @brief Constructor.
    CcliMenu(
        const std::string& STR_Name,    //!< Menu name.
        const CcliHelp& CLI_Help        //!< Corresponding help.
        );

    //! @brief Destructor.
    virtual ~CcliMenu(void);

public:
    //! @brief CLI reference setting.
    virtual void SetCli(CcliCli& CLI_Cli);
    //! @brief Reserved commands execution.
    virtual const bool ExecuteReserved(const CcliCommandLine& CLI_CommandLine) const;
    //! @brief User-defined commands execution.
    virtual const bool Execute(const CcliCommandLine& CLI_CommandLine) const;
    //! @brief Handler on menu exit.
    virtual void OnExit(void) const;

private:
    //! @brief Help node access.
    CcliKeyword& GetHelpNode(void);
    //! @brief Help node access (read-only).
    const CcliKeyword& GetHelpNode(void) const;
    //! @brief Exit node access.
    CcliKeyword& GetExitNode(void);
    //! @brief Exit node access (read-only).
    const CcliKeyword& GetExitNode(void) const;
    //! @brief Quit node access.
    CcliKeyword& GetQuitNode(void);
    //! @brief Quit node access (read-only).
    const CcliKeyword& GetQuitNode(void) const;
    //! @brief Print working menu node access.
    CcliKeyword& GetPwmNode(void);
    //! @brief Print working menu node access (read-only).
    const CcliKeyword& GetPwmNode(void) const;

private:
    //! Help node.
    CcliKeyword* m_pcliHelp;
    //! Exit node.
    CcliKeyword* m_pcliExit;
    //! Quit node.
    CcliKeyword* m_pcliQuit;
    //! Print working menu node.
    CcliKeyword* m_pcliPwm;
};


//! @brief Menu reference element.
class CcliMenuRef : public CcliElement
{
public:
    //! @brief Constructor.
    CcliMenuRef(
        const CcliMenu& CLI_Menu    //!< Referenced menu.
        );

    //! @brief Destructor.
    virtual ~CcliMenuRef(void);

public:
    //! @brief Referenced menu access.
    const CcliMenu& GetMenu(void) const;

protected:
    //! Referenced menu.
    const CcliMenu* const m_pcliMenu;
};


#endif // _CLI_MENU_H_
