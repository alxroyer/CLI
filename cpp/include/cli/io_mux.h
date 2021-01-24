/*
    Copyright (c) 2006-2008, Alexis Royer

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
//! @brief cli::IOMux class definition.


#ifndef _CLI_IO_MUX_H_
#define _CLI_IO_MUX_H_

#include "cli/namespace.h"
#include "cli/io_device.h"
#include "cli/shell.h"
#include "cli/tk.h"


CLI_NS_BEGIN(cli)

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
    private:
        //! @brief No default constructor.
        IOMux(void);
        //! @brief No copy constructor.
        IOMux(const IOMux&);

    public:
        //! @brief Main constructor.
        IOMux(
            const bool AutoDelete               //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~IOMux(void);

    private:
        //! @brief No assignment operator.
        IOMux& operator=(const IOMux&);

    protected:
        virtual const bool OpenDevice(void);
        virtual const bool CloseDevice(void);
    public:
        virtual void PutString(const char* const STR_Out) const;
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
            OutputDevice* const PCLI_Stream         //!< Device reference.
            );

        //! @brief Current input device accessor.
        const IODevice* const GetInput(void) const;

        //! @brief Input device addition in the list.
        //! @return true: The device has been added.
        //! @return false: The device has not been added.
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
        struct Output_t
        {
            OutputDevice* pcliOutput;
            bool bDoOpenClose;
        };
        struct Output_t m_arsOutputs[STREAM_TYPES_COUNT];

        //! Input device list.
        tk::Queue<IODevice*> m_qInputs;

        //! Protection against infinite loop on output device.
        mutable bool m_bIOLocked;
    };

CLI_NS_END(cli)

#endif // _CLI_IO_MUX_H_
