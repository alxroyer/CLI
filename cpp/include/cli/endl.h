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
//! @brief Endl class definition.

#ifndef _CLI_ENDL_H_
#define _CLI_ENDL_H_

#include <cli/element.h>


namespace cli {

    // Forward declarations.
    class Menu;
    class MenuRef;
    class Help;


    //! @brief End of command line element.
    //!
    //! CLI element used to terminate a command line.
    //! A menu reference can be optionally attached.
    class Endl : public Element
    {
    public:
        //! @brief Constructor.
        Endl(
            const Help& CLI_Help    //!< Help.
            );

        //! @brief Destructor.
        virtual ~Endl(void);

    public:
        //! @brief Attaches the optional menu.
        //! @return The menu object reference.
        //!
        //! Stores the menu object reference
        //! and gives it to the corresponding CLI object for ownership.
        Menu& SetMenu(
            Menu* const PCLI_Menu           //!< Newly created menu object.
            );

        //! @brief Attaches the optional menu.
        //! @return The menu object reference.
        MenuRef& SetMenuRef(
            MenuRef* const PCLI_MenuRef     //!< Menu reference.
            );

        //! @brief Access to the optional menu reference.
        const MenuRef* const GetMenuRef(void) const;

    private:
        //! Optional menu reference.
        const MenuRef* m_pcliMenuRef;
    };

};

#endif // _CLI_ENDL_H_

