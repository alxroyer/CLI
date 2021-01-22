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
//! @brief CcliParam and CcliParamT classes definition.

#ifndef _CLI_PARAM_H_
#define _CLI_PARAM_H_

#include "cli_syntax_node.h"


// Forward declarations.
class CcliHelp;


//! @brief Base parameter class.
//!
//! Base class for any kind of parameter elements.
//! Not final class.
class CcliParam : public CcliSyntaxNode
{
public:
    //! @brief Constructor.
    CcliParam(
        const std::string& STR_Keyword, //!< Keyword.
                                        //!< Does not mean much for a parameter.
                                        //!< Something like a description of the type of parameter.
        const CcliHelp& CLI_Help        //!< Corresponding help.
        );

    //! @brief Destructor.
    virtual ~CcliParam(void);

public:
    //! @brief Keyword access.
    //!
    //! Correction of CcliElement::GetKeyword().
    //! Does not return m_strKeyword, but m_strValue.
    virtual const std::string GetKeyword(void) const;

    //! @brief Elements research.
    virtual const bool FindElements(
        CcliElementList& CLI_ExactList, //!< List of elements corresponding exactly.
        CcliElementList& CLI_NearList,  //!< List of elements corresponding.
        const std::string& STR_Keyword  //!< Beginning of a keyword.
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
    virtual const CcliParam* const Clone(void) const = 0;

    //! @brief Cloned parameter access.
    const CcliParam* const GetCloned(void) const;

    //! @brief Value copy handler.
    virtual const CcliParam& CopyValue(
        const CcliParam& CLI_Param      //!< Parameter to copy the value from.
        ) const = 0;

protected:
    //! @brief Value setting from derived class.
    void SetValue(
        const std::string& STR_Value    //!< New value.
        ) const;

    //! @brief Clone initialization.
    const CcliParam* const InitClone(
        CcliParam& CLI_Param            //!< Clone parameter to initialize.
        ) const;

    //! @brief Cloned parameter reference setting.
    void SetCloned(
        const CcliParam& CLI_Cloned     //!< Clone parameter reference.
        );

private:
    //! Value in its string form.
    mutable std::string m_strValue;

    //! Cloned parameter reference.
    const CcliParam* m_pcliCloned;
};


//! @brief Template parameter class.
template <class T> class CcliParamT : public CcliParam
{
public:
    //! @brief Constructor.
    CcliParamT<T>(
        const std::string& STR_Keyword, //!< Keyword.
        const CcliHelp& CLI_Help        //!< Corresponding help.
        )
      : CcliParam(STR_Keyword, CLI_Help)
    {
    }

    //! @brief Destructor.
    virtual ~CcliParamT<T>(void)
    {
    }

public:
    //! @brief Implicit cast operator.
    operator const T(void) const
    {
        return m_tValue;
    }

    //! @brief Typed value copy handler.
    virtual const CcliParam& CopyValue(const CcliParam& CLI_Param) const
    {
        if (const CcliParamT<T>* const pcli_Param = dynamic_cast<const CcliParamT<T>*>(& CLI_Param))
        {
            SetValue(CLI_Param.GetKeyword(), *pcli_Param);
        }
        return *this;
    }

protected:
    //! @brief Constructor by value.
    CcliParamT<T>(const T& T_Value)
    {
        m_tValue = T_Value;
    }

    //! @brief Value setting for derived class.
    void SetValue(
        const std::string& STR_Value,   //!< New value in its string form.
        const T& T_Value                //!< New value in its typed form.
        ) const
    {
        CcliParam::SetValue(STR_Value);
        m_tValue = T_Value;
    }

private:
    //! Controlled value.
    mutable T m_tValue;
};

#endif // _CLI_PARAM_H_

