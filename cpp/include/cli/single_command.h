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
//! @brief SingleCommand class definition.

#ifndef _CLI_SINGLE_COMMAND_H_
#define _CLI_SINGLE_COMMAND_H_

#include <string>

#include <cli/io_device.h>


namespace cli {

    //! @brief Single command device.
    class SingleCommand : public IODevice
    {
    public:
        //! @brief Constructor.
        SingleCommand(
            const std::string& STR_Command,     //!< Command.
            OutputDevice& CLI_Output,           //!< Output.
            const bool B_AutoDelete             //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~SingleCommand(void);

    public:
        //! @brief Command accessor.
        const std::string& GetCommand(void) const;

    protected:
        virtual const bool OpenDevice(void);
        virtual const bool CloseDevice(void);
    public:
        virtual const KEY GetKey(void) const;
        virtual void PutString(const std::string& STR_Out) const;
        virtual void Beep(void) const;

    private:
        //! Output device.
        OutputDevice& m_cliOutput;

        //! The command.
        const std::string m_strCommand;

        //! Current position in the command.
        mutable int m_iPosition;
    };

};

#endif // _CLI_SINGLE_COMMAND_H_
