/*
    Copyright (c) 2006-2009, Alexis Royer, http://alexis.royer.free.fr/CLI

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
//! @brief Element class definition.

#ifndef _CLI_ELEMENT_H_
#define _CLI_ELEMENT_H_

#include "cli/namespace.h"
#include "cli/object.h"
#include "cli/help.h"
#include "cli/tk.h"


CLI_NS_BEGIN(cli)

    // Forward declarations.
    class Cli;
    class Shell;
    class OutputDevice;
    class Element;
    class Help;


    //! @brief Generic CLI element.
    //!
    //! This class describes a generic CLI element (keywords, parameters, menus...).
    //! It basically contains a keyword i.e. an string identifier, and a help object.
    class Element : public Object
    {
    public:
        // Typedefs.
        //! @brief CLI element list type.
        typedef tk::Queue<const Element*> List;
        //! @brief CLI element map type.
        typedef tk::Map<const tk::String, const Element*> Map;

    public:
        //! @brief Constructor.
        Element(
            const char* const STR_Keyword,  //!< String identifier.
            const Help& STR_Help            //!< Help.
            );

        //! @brief Destructor.
        //!
        //! Pure virtual. Element is not final.
        virtual ~Element(void) = 0;

    private:
        //! @brief No default constructor.
        Element(void);
        //! @brief No copy constructor.
        Element(const Element&);
        //! @brief No assignment operator.
        Element& operator=(const Element&);

    public:
        //! @brief Access to the string identifier.
        virtual const tk::String GetKeyword(void) const;

        //! @brief Access to the help object.
        virtual const Help& GetHelp(void) const;

        //! @brief Sub-elements search.
        virtual const bool FindElements(
            Element::List& CLI_ExactList,   //!< Output list of elements exactly matching STR_Keyword.
            Element::List& CLI_NearList,    //!< Output list of elements matching STR_Keyword exactly or not.
            const char* const STR_Keyword   //!< Keyword or beginning of a keyword.
                                            //!< Can be the whole word, or just the beginning, or something possibly matching (for parameters).
                                            //!< NULL means no keyword begun.
                                            //!< Put NULL to retrieve all sub-elements.
            ) const;

        //! @brief Cli reference setting.
        //!
        //! The m_pcliCli member should be set for each Element object.
        //! However, generally, this method is automaticaly called, as in SyntaxNode::AddElement() for instance.
        virtual void SetCli(
            Cli& CLI_Cli                    //!< Cli reference.
            );

        //! @brief Access to the CLI reference.
        Cli& GetCli(void);
        //! @brief Access to the CLI reference.
        const Cli& GetCli(void) const;

        //! @brief Access to the shell.
        Shell& GetShell(void) const;

        //! @brief Access to the output stream.
        const OutputDevice& GetOutputStream(void) const;

        //! @brief Access to the error stream.
        const OutputDevice& GetErrorStream(void) const;

    private:
        //! String identifier.
        const tk::String m_strKeyword;
        //! Help object.
        const Help m_cliHelp;
        //! CLI reference.
        Cli* m_pcliCli;
    };

CLI_NS_END(cli)

#endif // _CLI_ELEMENT_H_

