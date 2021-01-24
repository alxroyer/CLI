/*
    Copyright (c) 2006-2009, Alexis Royer

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
//! @brief MonoThreadDevice class definition.


#ifndef _CLI_MT_DEVICE_H_
#define _CLI_MT_DEVICE_H_

#include "cli/namespace.h"
#include "cli/io_device.h"
#include "cli/shell.h"
#include "cli/tk.h"


CLI_NS_BEGIN(cli)

    //! @brief Mono-Thread input device.
    class MonoThreadDevice : public IODevice
    {
    private:
        //! @brief No default constructor.
        MonoThreadDevice(void);
        //! @brief No copy constructor.
        MonoThreadDevice(const MonoThreadDevice&);

    public:
        //! @brief Main constructor.
        MonoThreadDevice(
            const char* const STR_DbgName,  //!< Debug name.
            const bool B_AutoDelete         //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~MonoThreadDevice(void);

    private:
        //! @brief No assignment operator.
        MonoThreadDevice& operator=(const MonoThreadDevice&);

    public:
        //! @brief IODevice mono-thread implementation.
        virtual const KEY GetKey(void) const;

    public:
        //! @brief Shell reference positionning.
        //! @warning Should be called by the Shell itself only.
        void AttachShell(
            Shell& CLI_Shell    //!< Shell to attach.
            );

        //! @brief Shell reference removal.
        //! @warning Should be called by the Shell itself only.
        void DetachShell(
            Shell& CLI_Shell    //!< Shell to detach.
            );

    protected:
        //! @brief Handler to call when a key is received.
        void OnKey(
            const KEY E_Key //!< Input key.
            );

    private:
        //! Shell reference.
        Shell* m_pcliShell;
    };

CLI_NS_END(cli)

#endif // _CLI_MT_DEVICE_H_
