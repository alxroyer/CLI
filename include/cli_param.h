/*
    Copyright 2006 Alexis Royer

    This file is part of the CLI library.

    The CLI library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _CLI_PARAM_H_
#define _CLI_PARAM_H_

#include "cli_syntax_node.h"


class CcliHelp;


class CcliParam : public CcliSyntaxNode
{
public:
    CcliParam(const std::string& STR_Keyword, const CcliHelp& CLI_Help);
    virtual ~CcliParam(void);

public:
    virtual std::string GetKeyword(void) const;
    virtual bool FindElements(CcliElementList& CLI_ExactList, CcliElementList& CLI_NearList, const std::string& STR_Keyword) const;
    virtual bool SetstrValue(const std::string& STR_Value) const = 0;
    std::string GetstrValue(void) const;
    virtual const CcliParam* Clone(void) const = 0;
    const CcliParam* GetCloned(void) const;
    virtual const CcliParam& CopyValue(const CcliParam& CLI_Param) const = 0;

protected:
    void SetValue(const std::string& STR_Value) const;
    const CcliParam* InitClone(CcliParam* PCLI_Param) const;
    void SetCloned(const CcliParam* PCLI_Cloned);

private:
    mutable std::string m_strValue;
    const CcliParam* m_pcliCloned;
};


template <class T> class CcliParamT : public CcliParam
{
public:
    CcliParamT<T>(const std::string& STR_Keyword, const CcliHelp& CLI_Help)
      : CcliParam(STR_Keyword, CLI_Help)
    {
    }

    virtual ~CcliParamT<T>(void)
    {
    }

public:
    operator T(void) const
    {
        return m_tValue;
    }

    virtual const CcliParam& CopyValue(const CcliParam& CLI_Param) const
    {
        if (const CcliParamT<T>* pcli_Param = dynamic_cast<const CcliParamT<T>*>(& CLI_Param))
        {
            SetValue(CLI_Param.GetKeyword(), *pcli_Param);
        }
        return *this;
    }

protected:
    CcliParamT<T>(T T_Value)
    {
        m_tValue = T_Value;
    }

    void SetValue(const std::string& STR_Value, const T& T_Value) const
    {
        CcliParam::SetValue(STR_Value);
        m_tValue = T_Value;
    }

private:
    mutable T m_tValue;
};

#endif // _CLI_PARAM_H_

