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
//! @brief CcliSyntaxTag and CcliSyntaxRef classes definition.

#ifndef _CLI_SYNTAX_TAG_H_
#define _CLI_SYNTAX_TAG_H_

#include "cli_syntax_node.h"


//! @brief Syntax tag element class.
class CcliSyntaxTag : public CcliSyntaxNode
{
public:
    //! @brief Constructor.
    CcliSyntaxTag(
        const bool B_Hollow     //!< Hollow attribute.
        );

    //! @brief Destructor.
    virtual ~CcliSyntaxTag(void);

public:
    //! @brief Retrieves the hollow attribute.
    const bool GetbHollow(void) const;

private:
    //! Hollow attribute.
    const bool m_bHollow;
};


//! @brief Syntax tag reference element.
class CcliSyntaxRef : public CcliElement
{
public:
    //! @brief Constructor.
    CcliSyntaxRef(
        const CcliSyntaxTag& CLI_Tag    //!< Referenced tag element.
        );

    //! @brief Destructor.
    virtual ~CcliSyntaxRef(void);

public:
    //! @brief Tag reference access.
    const CcliSyntaxTag& GetTag(void) const;

protected:
    //! Tag reference.
    const CcliSyntaxTag* const m_pcliTag;
};

#endif // _CLI_SYNTAX_TAG_H_
