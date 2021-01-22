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
//! @brief InputFileDevice and OutputFileDevice classes definition.

#ifndef _CLI_FILE_DEVICE_H_
#define _CLI_FILE_DEVICE_H_

#include <fstream>

#include <cli/io_device.h>


namespace cli {

    //! @brief Input file device class.
    class InputFileDevice : public IODevice
    {
    public:
        //! @brief Constructor.
        InputFileDevice(
            const std::string& STR_FileName,    //!< Input file name.
            OutputDevice& CLI_Out,              //!< Output device.
                                                //!< This output device will be opened automatically
                                                //!< when the input device is opened.
            const bool B_AutoDelete             //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~InputFileDevice(void);

    protected:
        virtual const bool OpenDevice(void);
        virtual const bool CloseDevice(void);
    public:
        virtual const KEY GetKey(void) const;
        virtual void PutString(const std::string& STR_Out) const;
        virtual void Beep(void) const;

    public:
        //! @brief File name accessor.
        const std::string& GetFileName(void) const;

        //! @brief Current line accessor.
        const int GetCurrentLine(void) const;

        //! @brief Current column accessor.
        const int GetCurrentColumn(void) const;

    private:
        //! @brief Input file name.
        const std::string m_strFileName;

        //! Input file.
        mutable std::basic_ifstream<char> m_stdFile;

        //! Output device.
        OutputDevice& m_cliOutput;

        //! Current Line.
        mutable int m_iCurrentLine;

        //! Current column.
        mutable int m_iCurrentColumn;

        //! Next character line.
        mutable int m_iNextLine;

        //! Next column.
        mutable int m_iNextColumn;
    };

    //! @brief Output file device.
    class OutputFileDevice : public OutputDevice
    {
    public:
        //! @brief Constructor.
        OutputFileDevice(
            const std::string& STR_OutputFileName,  //!< Output file name.
            const bool B_AutoDelete                 //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~OutputFileDevice(void);

    protected:
        virtual const bool OpenDevice(void);
        virtual const bool CloseDevice(void);
    public:
        virtual void PutString(const std::string& STR_Out) const;
        virtual void Beep(void) const;

    public:
        //! @brief File name accessor.
        const std::string& GetFileName(void) const;

    private:
        //! @brief Output file name.
        const std::string m_strFileName;

        //! Output file.
        mutable std::basic_ofstream<char> m_stdFile;
    };

};

#endif // _CLI_FILE_DEVICE_H_
