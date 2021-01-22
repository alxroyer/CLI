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
//! @brief CommandLine class definition.

#ifndef _CLI_COMMAND_LINE_H_
#define _CLI_COMMAND_LINE_H_

#include <string>
#include <vector>

#include <cli/element.h>


namespace cli {

    // Forward declarations.
    class Menu;


    //! @brief Command line parsing.
    //!
    //! Parses a command line in its string form,
    //! and returns the command line in its Element collection form after analysis.
    class CommandLine
    {
    public:
        //! @brief Default constructor.
        CommandLine(void);

        //! @brief Destructor.
        virtual ~CommandLine(void);

    public:
        //! @brief Parse and analysis invocation.
        //! @return true: Success.
        //! @return false: Failure.
        //!
        //! If this method succeeds,
        //! the result is stored in the object itself,
        //! and can be accessed through the following public methods.
        const bool Parse(
            const Menu& CLI_Menu,           //!< Current menu.
            const std::string& STR_Line,    //!< Input command line in its string form.
            const bool B_Execution          //!< Flag set when parsing is done for execution.
                                            //!< Implicitely say completion otherwise.
            );

        //! @brief Element access.
        const Element& operator[](
            const int I_Pos                 //!< Element position.
            ) const;

        //! @brief Last element access.
        const Element& GetLastElement(void) const;

        //! @brief Element count.
        const int GetElementCount(void) const;

        //! @brief Last word (for completion).
        //! @return NULL when no last word.
        //! @return The last word otherwise.
        //!         After parsing, the buffer is valid as far as the command line object is valid.
        //! 
        //! When used for completion, this object does not analyse the last word and just stores it.
        //! This method retrieves this last word.
        const char* const GetLastWord(void) const;

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
            std::vector<std::string>& VSTR_Words,   //!< Word list.
            int& I_LastWordPosition                 //!< Last word position in the list.
            );

        //! @brief Element referencing.
        //! @return The CommandLine object itself.
        //!
        //! Pushes an element reference during analysis.
        CommandLine& AddElement(
            const Element* const PCLI_Element       //!< New element reference.
            );

    private:
        //! Collection of corresponding element references for the command line.
        ElementList m_cliElements;
        //! List of element references to destroy automatically in the CommandLine destructor.
        ElementList m_cliAutoDelete;
        //! Current menu reference.
        const Element* m_pcliMenu;
        //! Last word used for completion.
        std::string m_strLastWord;
        //! Last word validity flag.
        bool m_bLastWordValid;
        //! Number of backspaces for completion.
        int m_iNumBackspacesForCompletion;
        //! Last error.
        std::string m_strError;
    };

};

#endif // _CLI_COMMAND_LINE_H_

