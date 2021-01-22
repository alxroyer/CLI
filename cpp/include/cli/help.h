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
//! @brief Help class definition.

#ifndef _CLI_HELP_H_
#define _CLI_HELP_H_

#include <string>
#include <map>


namespace cli {

    //! @brief Help container class.
    //!
    //! Objects of this class permits help definition of different languages.
    //! An object of this class is owned by each CLI element.
    class Help
    {
    public:
        //! @brief Default constructor.
        Help(void);

        //! @brief Copy constructor.
        Help(
            const Help& CLI_Help    //!< Source help object.
            );

        //! @brief Destructor.
        virtual ~Help(void);

    public:
        //! @brief Languages identifiers.
        typedef enum
        {
            LANG_EN,        //!< English.
            LANG_FR,        //!< French.
            LANG_COUNT      //!< Languages count.
        } LANG;

    public:
        //! @brief Adds help for a given language.
        //!
        //! If help already exists for the given language,
        //! then it is overwritten.
        Help& AddHelp(
            const LANG E_Lang,              //!< Language identifier.
            const std::string& STR_Help     //!< Help string.
            );

        //! @brief Determines whether help exists for a given language.
        const bool HasHelp(
            const LANG E_Lang               //!< Language identifier.
            ) const;

        //! @brief Access to the help of a given language.
        const std::string GetHelp(
            const LANG E_Lang               //!< Language identifier.
            ) const;

    private:
        //! Help map type shortcut.
        typedef std::map<const LANG, std::string> HelpMap;
        //! Help map.
        HelpMap m_mstrHelps;
    };

};

#endif // _CLI_HELP_H_

