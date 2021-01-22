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
//! @brief SyntaxNode class definition.

#ifndef _CLI_SYNTAX_NODE_H_
#define _CLI_SYNTAX_NODE_H_

#include <cli/element.h>


namespace cli {

    // Forward declarations.
    class Help;


    //! @brief Syntax node elements.
    //!
    //! Syntax node elements are element that accepts child elements.
    //! They store a list of possible childs,
    //! and offer operations based on this list.
    class SyntaxNode : public Element
    {
    public:
        //! @brief Constructor.
        SyntaxNode(
            const std::string& STR_Keyword, //!< Keyword of the element.
            const Help& CLI_Help            //!< Corresponding help.
            );

        //! @brief Destructor.
        virtual ~SyntaxNode(void);

    public:
        //! @brief Possible element addition.
        Element& AddElement(
            Element* const PCLI_Element     //!< New element.
            );

        //! @brief Returns the list of child elements coresponding to a keyword
        //!         or the beginning of a keyword.
        virtual const bool FindElements(
            ElementList& CLI_ExactList,     //!< Exact matching keywords output list.
            ElementList& CLI_NearList,      //!< All matching keywords output list.
            const char* const STR_Keyword   //!< Keyword or beginning of a keyword.
                                            //!< NULL means no keyword begun.
            ) const;

    private:
        //! List of possible child elements.
        ElementMap m_cliElements;
    };

};

#endif // _CLI_SYNTAX_NODE_H_
