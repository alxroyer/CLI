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
//! @brief cli::IOMux class definition.


#ifndef _CLI_IO_MUX_H_
#define _CLI_IO_MUX_H_

#include <deque>

#include <cli/io_device.h>
#include <cli/shell.h>


namespace cli {

    //! @brief Input / output device multiplexer.
    //!
    //! @warning Please ensure one of the following conditions for each device passed to instances of this class:
    //!     - Either the device is an auto-deleted device,
    //!     - or it will be destroyed after this input / output multiplexer,
    //!     - or it will be dereferenced on termination.
    //! Otherwise you could experience consistency troubles.
    //! The null device and standard devices are not subject to this remark.
    class IOMux : public IODevice
    {
    public:
        //! @brief Default constructor.
        IOMux(
            const bool AutoDelete               //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~IOMux(void);

    protected:
        virtual const bool OpenDevice(void);
        virtual const bool CloseDevice(void);
    public:
        virtual void PutString(const std::string& STR_Out) const;
        virtual void Beep(void) const;
        virtual const KEY GetKey(void) const;

    public:
        //! @brief Output stream accessor.
        const OutputDevice& GetOutput(
            const STREAM_TYPE E_StreamType          //!< Output stream identifier.
            ) const;

        //! @brief Output stream setting.
        const bool SetOutput(
            const STREAM_TYPE E_StreamType,         //!< Output stream identifier.
            OutputDevice* const PCLI_Welcome        //!< Welcome stream.
            );

        //! @brief Current input device accessor.
        const IODevice* const GetInput(void) const;

        //! @brief Input device addition in the list.
        const bool AddInput(
            IODevice* const PCLI_Input          //!< Input device.
            );

        //! @brief Switch to next input device.
        const bool NextInput(void);

        //! @brief Reset input device list.
        const bool ResetInputList(void);

    protected:
        //! @brief Method called when an input device is needed.
        //! @return New input device.
        //! @return NULL means no more input
        //!         unless new input devices have been pushed through AddInput().
        virtual IODevice* const CreateInputDevice(void);

    private:
        //! @brief Check input list is not empty.
        //!
        //! Calls CreateInputDevice() when the list is empty.
        //! Tries to open the device when m_bOpened equals true.
        const bool CheckInput(void) const;

        //! @brief Releases first input device.
        //!
        //! Closure and deletion (if required) of first input device.
        //! Checks this first device is not used somewhere else, either in other inputs or outputs.
        const bool ReleaseFirstInputDevice(void);

        //! @brief Releases output device.
        //!
        //! Closure and deletion (if required) of the given output stream.
        //! Checks this first device is not used somewhere else, either in other inputs or outputs.
        const bool ReleaseOutputDevice(
            const STREAM_TYPE E_StreamType, //!< Output stream type.
            const bool B_Delete             //!< Do device deletion if required.
            );

    private:
        //! Output streams.
        OutputDevice* m_arpcliOutputs[STREAM_TYPES_COUNT];

        //! Input device list.
        std::deque<IODevice*> m_qInputs;

        //! Protection against infinite loop on output device.
        mutable bool m_bIOLocked;
    };

};

#endif // _CLI_IO_MUX_H_
