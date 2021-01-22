/*
    Copyright (c) 2006-2007, Alexis Royer

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
        * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
