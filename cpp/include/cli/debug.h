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
//! @brief Debug facilities.

#ifndef _CLI_DEBUG_H_
#define _CLI_DEBUG_H_

#include <string>


namespace cli {

    //! @brief Call information object.
    class CallInfo
    {
    private:
        //! No default constructor.
        CallInfo(void);

    public:
        //! @warning Copy constructor is declared public for compilation reasons,
        //!          but not implemented.
        CallInfo(const CallInfo&);

        //! @brief Regular constructor.
        CallInfo(
            const std::string& STR_File,    //!< File of call.
            const unsigned int I_Line,      //!< Position in file.
            const std::string& STR_Function //!< Function of call.
            );

        //! @brief Destructor.
        ~CallInfo(void);

    public:
        //! @brief File accessor.
        const std::string& GetFile(void) const;

        //! @brief Line accessor
        const unsigned int GetLine(void) const;

        //! @brief Function accessor.
        const std::string& GetFunction(void) const;

    private:
        const std::string& m_strFile;       //!< File of call.
        const unsigned int m_iLine;         //!< Position in file.
        const std::string& m_strFunction;   //!< Function of call.
    };

    //! @brief Call information filling.
    //!
    //! Use this constant when call information is needed.
    #define __CALL_INFO__ cli::CallInfo(__FILE__, __LINE__, __FUNCTION__)

};

#endif // _CLI_DEBUG_H_

