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
//! @brief SyntaxTag and SyntaxRef classes definition.

#ifndef _CLI_SYNTAX_TAG_H_
#define _CLI_SYNTAX_TAG_H_

#include <cli/syntax_node.h>


namespace cli {

    //! @brief Syntax tag element class.
    class SyntaxTag : public SyntaxNode
    {
    private:
        //! @brief Default constructor forbidden.
        SyntaxTag(void);
        //! @brief Copy constructor forbidden.
        SyntaxTag(const SyntaxTag&);

    public:
        //! @brief Constructor.
        SyntaxTag(
            const bool B_Hollow     //!< Hollow attribute.
            );

        //! @brief Destructor.
        virtual ~SyntaxTag(void);

    public:
        //! @brief Access to the string identifier.
        virtual const std::string GetKeyword(void) const;

        //! @brief Retrieves the hollow attribute.
        const bool GetbHollow(void) const;

    private:
        //! Hollow attribute.
        const bool m_bHollow;
    };


    //! @brief Syntax tag reference element.
    class SyntaxRef : public Element
    {
    public:
        //! @brief Constructor.
        SyntaxRef(
            const SyntaxTag& CLI_Tag        //!< Referenced tag element.
            );

        //! @brief Destructor.
        virtual ~SyntaxRef(void);

    public:
        //! @brief Access to the string identifier.
        virtual const std::string GetKeyword(void) const;

        //! @brief Tag reference access.
        const SyntaxTag& GetTag(void) const;

    protected:
        //! Tag reference.
        const SyntaxTag* const m_pcliTag;
    };

};

#endif // _CLI_SYNTAX_TAG_H_
