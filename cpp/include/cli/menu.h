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
//! @brief Menu and MenuRef classes definition.

#ifndef _CLI_MENU_H_
#define _CLI_MENU_H_

#include <string>
#include <cli/syntax_node.h>


namespace cli {

    // Forward declarations.
    class Cli;
    class Console;
    class CommandLine;
    class Keyword;
    class Help;


    //! @brief Menu definition.
    class Menu : public SyntaxNode
    {
    public:
        //! @brief Constructor.
        Menu(
            const std::string& STR_Name,    //!< Menu name.
            const Help& CLI_Help            //!< Corresponding help.
            );

        //! @brief Destructor.
        virtual ~Menu(void);

    public:
        //! @brief CLI reference setting.
        virtual void SetCli(Cli& CLI_Cli);
        //! @brief Reserved commands execution.
        virtual const bool ExecuteReserved(const CommandLine& CLI_CommandLine) const;
        //! @brief User-defined commands execution.
        virtual const bool Execute(const CommandLine& CLI_CommandLine) const;
        //! @brief Handler on menu exit.
        virtual void OnExit(void) const;

    private:
        //! @brief Help node access.
        Keyword& GetHelpNode(void);
        //! @brief Help node access (read-only).
        const Keyword& GetHelpNode(void) const;
        //! @brief Exit node access.
        Keyword& GetExitNode(void);
        //! @brief Exit node access (read-only).
        const Keyword& GetExitNode(void) const;
        //! @brief Quit node access.
        Keyword& GetQuitNode(void);
        //! @brief Quit node access (read-only).
        const Keyword& GetQuitNode(void) const;
        //! @brief Print working menu node access.
        Keyword& GetPwmNode(void);
        //! @brief Print working menu node access (read-only).
        const Keyword& GetPwmNode(void) const;

    private:
        //! Help node.
        Keyword* m_pcliHelp;
        //! Exit node.
        Keyword* m_pcliExit;
        //! Quit node.
        Keyword* m_pcliQuit;
        //! Print working menu node.
        Keyword* m_pcliPwm;
    };


    //! @brief Menu reference element.
    class MenuRef : public Element
    {
    public:
        //! @brief Constructor.
        MenuRef(
            const Menu& CLI_Menu        //!< Referenced menu.
            );

        //! @brief Destructor.
        virtual ~MenuRef(void);

    public:
        //! @brief Referenced menu access.
        const Menu& GetMenu(void) const;

    protected:
        //! Referenced menu.
        const Menu* const m_pcliMenu;
    };

};

#endif // _CLI_MENU_H_
