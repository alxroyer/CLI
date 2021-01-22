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
//! @brief CcliHelp class declaration.

#ifndef _CLI_HELP_H_
#define _CLI_HELP_H_

#include <string>
#include <map>


//! @brief Help container class.
//!
//! Objects of this class permits help definition of different languages.
//! An object of this class is owned by each CLI element.
class CcliHelp
{
public:
    //! @brief Default constructor.
    CcliHelp(void);

    //! @brief Copy constructor.
    CcliHelp(
        const CcliHelp& CLI_Help    //!< Source help object.
        );

    //! @brief Destructor.
    virtual ~CcliHelp(void);

public:
    //! @brief Languages identifiers.
    typedef enum
    {
        CLI_LANG_EN,    //!< English.
        CLI_LANG_FR,    //!< French.
	CLI_LANG_COUNT  //!< Languages count.
    } CLI_LANG;

public:
    //! @brief Adds help for a given language.
    //!
    //! If help already exists for the given language,
    //! then it is overwritten.
    CcliHelp& AddHelp(
        const CLI_LANG E_Lang,          //!< Language identifier.
        const std::string& STR_Help     //!< Help string.
        );

    //! @brief Determines whether help exists for a given language.
    const bool HasHelp(
        const CLI_LANG E_Lang           //!< Language identifier.
	) const;

    //! @brief Access to the help of a given language.
    const std::string GetHelp(
        const CLI_LANG E_Lang           //!< Language identifier.
	) const;

private:
    //! Help map type shortcut.
    typedef std::map<const CLI_LANG, std::string> CcliHelpMap;
    //! Help map.
    CcliHelpMap m_mstrHelps;
};

#endif // _CLI_HELP_H_

