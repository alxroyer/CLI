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
//! @brief CcliCommandLine class definition.

#ifndef _CLI_COMMAND_LINE_H_
#define _CLI_COMMAND_LINE_H_

#include <string>
#include <vector>

#include "cli_element.h"


// Forward declarations.
class CcliMenu;


//! @brief Command line parsing.
//!
//! Parses a command line in its string form,
//! and returns the command line in its CcliElement collection form after analysis.
class CcliCommandLine
{
public:
    //! @brief Default constructor.
    CcliCommandLine(void);

    //! @brief Destructor.
    virtual ~CcliCommandLine(void);

public:
    //! @brief Parse and analysis invocation.
    //! @return true: Success.
    //! @return false: Failure.
    //!
    //! If this method succeeds,
    //! the result is stored in the object itself,
    //! and can be accessed through the following public methods.
    const bool Parse(
        const CcliMenu& CLI_Menu,	//!< Current menu.
        const std::string& STR_Line,    //!< Input command line in its string form.
        const bool B_Execution          //!< Flag set when parsing is done for execution.
                                        //!< Implicitely say completion otherwise.
        );

    //! @brief Element access.
    const CcliElement& operator[](
        const int I_Pos         //!< Element position.
	) const;

    //! @brief Last element access.
    const CcliElement& GetLastElement(void) const;

    //! @brief Element count.
    const int GetElementCount(void) const;

    //! @brief Last word (for completion).
    //!
    //! When used for completion, this object does not analyse the last word and just stores it.
    //! This method retrieves this last word.
    const std::string GetLastWord(void) const;

    //! @brief Number of backspaces for completion.
    //!
    //! Number of backspaces in order to erase the last word of the line.
    const int GetNumBackspacesForCompletion(void) const;

    //! @brief Last error.
    //!
    //! This resource is cleared on every parse.
    const std::string GetLastError(void) const;

private:
    //! @brief Splits a command line in words.
    //! @return A collection of words.
    const bool Split(
        const std::string& STR_Line,            //!< Input command line in its string form.
        std::vector<std::string>& VSTR_Words,    //!< Word list.
        int& I_LastWordPosition                 //!< Last word position in the list.
	);

    //! @brief Element referencing.
    //! @return The CcliCommandLine object itself.
    //!
    //! Pushes an element reference during analysis.
    CcliCommandLine& AddElement(
        const CcliElement* const PCLI_Element //!< New element reference.
	);

private:
    //! Collection of corresponding element references for the command line.
    CcliElementList m_cliElements;
    //! List of element references to destroy automatically in the CcliCommandLine destructor.
    CcliElementList m_cliAutoDelete;
    //! Current menu reference.
    const CcliElement* m_pcliMenu;
    //! Last word used for completion.
    std::string m_strLastWord;
    //! Number of backspaces for completion.
    int m_iNumBackspacesForCompletion;
    //! Last error.
    std::string m_strError;
};

#endif // _CLI_COMMAND_LINE_H_

