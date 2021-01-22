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
//! @brief CcliParamHost class definition.

#ifndef _CLI_PARAM_HOST_H_
#define _CLI_PARAM_HOST_H_

#include "cli_param.h"


// Forward declarations.
class CcliHelp;


//! @brief Host names, IP adresses parameter element class.
class CcliParamHost : public CcliParamT<std::string>
{
public:
    //! @brief Constructor.
    CcliParamHost(
        const CcliHelp& CLI_Help    //!< Corresponding help.
        );

    //!< @brief Destructor.
    virtual ~CcliParamHost(void);

public:
    //! @brief Value setting handler.
    virtual const bool SetstrValue(const std::string& STR_Value) const;

    //! @brief Parameter cloning handler.
    virtual const CcliParam* const Clone(void) const;
};

#endif // _CLI_PARAM_HOST_H_
