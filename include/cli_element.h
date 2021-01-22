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
//! @brief CcliElement class definition.

#ifndef _CLI_ELEMENT_H_
#define _CLI_ELEMENT_H_

#include <string>
#include <deque>
#include <map>

#include "cli_help.h"


// Forward declarations.
class CcliCli;
class CcliShell;
class CcliOutputDevice;
class CcliElement;
class CcliHelp;


// Typedefs.
//! @brief CLI element list type.
typedef std::deque<const CcliElement*> CcliElementList;
//! @brief CLI element map type.
typedef std::map<const std::string, const CcliElement*> CcliElementMap;


//! @brief Generic CLI element.
//!
//! This class describes a generic CLI element (keywords, parameters, menus...).
//! It basically contains a keyword i.e. an string identifier, and a help object.
class CcliElement
{
public:
    //! @brief Constructor.
    CcliElement(
        const std::string& STR_Keyword, //!< String identifier.
        const CcliHelp& STR_Help        //!< Help.
        );

    //! @brief Destructor.
    //!
    //! Pure virtual. CcliElement is not final.
    virtual ~CcliElement(void) = 0;

public:
    //! @brief Access to the string identifier.
    virtual const std::string GetKeyword(void) const;

    //! @brief Access to the help object.
    virtual const CcliHelp& GetHelp(void) const;

    //! @brief Sub-elements search.
    virtual const bool FindElements(
        CcliElementList& CLI_ExactList, //!< Output list of elements exactly matching STR_Keyword.
        CcliElementList& CLI_NearList,  //!< Output list of elements matching STR_Keyword exactly or not.
        const std::string& STR_Keyword  //!< Research pattern.
                                        //!< Can be the whole word, or just the beginning, or something possibly matching (for parameters).
					//!< Put an empty string to retrieve all sub-elements.
        ) const;

    //! @brief CcliCli reference setting.
    //!
    //! The m_pcliCli member should be set for each CcliElement object.
    //! However, generally, this method is automaticaly called, as in CcliSyntaxNode::AddElement() for instance.
    virtual void SetCli(
        CcliCli& CLI_Cli    //!< CcliCli reference.
	);

    //! @brief Access to the CLI reference.
    CcliCli& GetCli(void);
    //! @brief Access to the CLI reference.
    const CcliCli& GetCli(void) const;

    //! @brief Access to the shell.
    CcliShell& GetShell(void) const;

    //! @brief Access to the output stream.
    const CcliOutputDevice& GetOutputStream(void) const;

    //! @brief Access to the error stream.
    const CcliOutputDevice& GetErrorStream(void) const;

private:
    //! String identifier.
    const std::string m_strKeyword;
    //! Help object.
    const CcliHelp m_cliHelp;
    //! CLI reference.
    CcliCli* m_pcliCli;
};

#endif // _CLI_ELEMENT_H_

