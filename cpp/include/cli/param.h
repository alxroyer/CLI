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
//! @brief Param and ParamT classes definition.

#ifndef _CLI_PARAM_H_
#define _CLI_PARAM_H_

#include <cli/syntax_node.h>


namespace cli {

    // Forward declarations.
    class Help;


    //! @brief Base parameter class.
    //!
    //! Base class for any kind of parameter elements.
    //! Not final class.
    class Param : public SyntaxNode
    {
    public:
        //! @brief Constructor.
        Param(
            const std::string& STR_Keyword, //!< Keyword.
                                            //!< Does not mean much for a parameter.
                                            //!< Something like a description of the type of parameter.
            const Help& CLI_Help            //!< Corresponding help.
            );

        //! @brief Destructor.
        virtual ~Param(void);

    public:
        //! @brief Keyword access.
        //!
        //! Correction of Element::GetKeyword().
        //! Does not return m_strKeyword, but m_strValue.
        virtual const std::string GetKeyword(void) const;

        //! @brief Elements research.
        virtual const bool FindElements(
            ElementList& CLI_ExactList,     //!< List of elements corresponding exactly.
            ElementList& CLI_NearList,      //!< List of elements corresponding.
            const char* const STR_Keyword   //!< Beginning of a keyword.
            ) const;

        //! @brief Value setting.
        //! @return true: the value has been set correctly.
        //! @return false: the value has not been set.
        //!
        //! To be overloaded by derived classes.
        virtual const bool SetstrValue(
            const std::string& STR_Value    //!< New value.
            ) const = 0;

        //! @brief Value access in its string form.
        const std::string GetstrValue(void) const;

        //! @brief Parameter cloning handler.
        //! @return A newly created parameter object of the correct type.
        //!
        //! To be overloaded by derived classes.
        virtual const Param* const Clone(void) const = 0;

        //! @brief Cloned parameter access.
        const Param* const GetCloned(void) const;

        //! @brief Value copy handler.
        virtual const Param& CopyValue(
            const Param& CLI_Param          //!< Parameter to copy the value from.
            ) const = 0;

    protected:
        //! @brief Value setting from derived class.
        void SetValue(
            const std::string& STR_Value    //!< New value.
            ) const;

        //! @brief Clone initialization.
        const Param* const InitClone(
            Param& CLI_Param                //!< Clone parameter to initialize.
            ) const;

        //! @brief Cloned parameter reference setting.
        void SetCloned(
            const Param& CLI_Cloned         //!< Clone parameter reference.
            );

    private:
        //! Value in its string form.
        mutable std::string m_strValue;

        //! Cloned parameter reference.
        const Param* m_pcliCloned;
    };


    //! @brief Template parameter class.
    template <class T> class ParamT : public Param
    {
    public:
        //! @brief Constructor.
        ParamT<T>(
            const std::string& STR_Keyword, //!< Keyword.
            const Help& CLI_Help            //!< Corresponding help.
            )
          : Param(STR_Keyword, CLI_Help)
        {
        }

        //! @brief Destructor.
        virtual ~ParamT<T>(void)
        {
        }

    public:
        //! @brief Implicit cast operator.
        operator const T(void) const
        {
            return m_tValue;
        }

        //! @brief Typed value copy handler.
        virtual const Param& CopyValue(const Param& CLI_Param) const
        {
            if (const ParamT<T>* const pcli_Param = dynamic_cast<const ParamT<T>*>(& CLI_Param))
            {
                SetValue(CLI_Param.GetKeyword(), *pcli_Param);
            }
            return *this;
        }

    protected:
        //! @brief Value setting for derived class.
        void SetValue(
            const std::string& STR_Value,   //!< New value in its string form.
            const T& T_Value                //!< New value in its typed form.
            ) const
        {
            Param::SetValue(STR_Value);
            m_tValue = T_Value;
        }

    private:
        //! Controlled value.
        mutable T m_tValue;
    };

};

#endif // _CLI_PARAM_H_

