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
//! @brief Object consistency insurance.

#ifndef _CLI_CONSISTENCY_H_
#define _CLI_CONSISTENCY_H_

namespace cli {

    //! @brief Ensures common input / output devices creation.
    //!
    //! This method is basically called in constructors of objects
    //! that may need common devices all along their validity,
    //! even in their constructors.
    void EnsureCommonDevices(void);

    //! @brief Ensures the trace object creation.
    //!
    //! This method is basically called in constructors of objects
    //! that may generate traces all along their validity,
    //! even in their destructor.
    void EnsureTraces(void);

};

#endif // _CLI_CONSISTENCY_H_
