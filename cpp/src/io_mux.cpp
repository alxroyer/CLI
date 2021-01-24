/*
    Copyright (c) 2006-2010, Alexis Royer, http://alexis.royer.free.fr/CLI

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


#include "cli/pch.h"

#include "cli/io_mux.h"
#include "cli/shell.h"
#include "cli/assert.h"
#include "consistency.h"
#include "constraints.h"

CLI_NS_USE(cli)


IOMux::IOMux(const bool B_AutoDelete)
  : IODevice("mux", B_AutoDelete),
    m_qInputs(MAX_IO_MUX_INPUTS),
    m_bIOLocked(false)
{
    EnsureCommonDevices();
    EnsureTraces();

    memset(m_arsOutputs, '\0', sizeof(m_arsOutputs));
}

IOMux::~IOMux(void)
{
    for (int i = 0; i < STREAM_TYPES_COUNT; i++)
    {
        SetOutput((STREAM_TYPE) i, NULL);
    }
    ResetInputList();
}

const bool IOMux::OpenDevice(void)
{
    bool b_Res = true;
    IODevice* pcli_Input = NULL;

    // Input.
    if ((! CheckInput()) || (m_qInputs.IsEmpty()))
    {
        // Note: m_cliLastError has been set within CheckInput()
        b_Res = false;
    }
    else
    {
        if ((pcli_Input = m_qInputs.GetHead()))
        {
            if (! pcli_Input->OpenUp(__CALL_INFO__))
            {
                m_cliLastError = pcli_Input->GetLastError();
                b_Res = false;
            }
        }
        else
        {
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "IOMux: Input list error")
                .SetString(ResourceString::LANG_FR, "IOMux: Erreur sur la liste d'entrées");
            b_Res = false;
        }
    }

    // Outputs.
    for (int i = 0; i < STREAM_TYPES_COUNT; i++)
    {
        // Select default output if none is set.
        if (m_arsOutputs[i].pcliOutput == NULL)
        {
            if (pcli_Input != NULL)
            {
                m_arsOutputs[i].pcliOutput = pcli_Input;
            }
            else
            {
                m_arsOutputs[i].pcliOutput = & OutputDevice::GetNullDevice();
            }
            m_arsOutputs[i].pcliOutput->UseInstance(__CALL_INFO__);
        }

        // Open up the output device.
        CLI_ASSERT(! m_arsOutputs[i].bDoOpenClose);
        if (m_arsOutputs[i].pcliOutput != NULL)
        {
            if (! m_arsOutputs[i].pcliOutput->OpenUp(__CALL_INFO__))
            {
                m_cliLastError = m_arsOutputs[i].pcliOutput->GetLastError();
                b_Res = false;
            }
        }
        m_arsOutputs[i].bDoOpenClose = true;
    }

    if (! b_Res)
    {
        CloseDevice();
    }
    return b_Res;
}

const bool IOMux::CloseDevice(void)
{
    bool b_Res = true;

    // Outputs.
    for (int i = 0; i < STREAM_TYPES_COUNT; i++)
    {
        if (m_arsOutputs[i].pcliOutput != NULL)
        {
            if (m_arsOutputs[i].bDoOpenClose)
            {
                if (! m_arsOutputs[i].pcliOutput->CloseDown(__CALL_INFO__))
                {
                    m_cliLastError = m_arsOutputs[i].pcliOutput->GetLastError();
                    b_Res = false;
                }
            }
            m_arsOutputs[i].bDoOpenClose = false;
        }
    }

    // Inputs.
    while (! m_qInputs.IsEmpty())
    {
        if (! ReleaseFirstInputDevice())
        {
            // Note: m_cliLastError has been set within ReleaseFirstInputDevice()
            b_Res = false;
        }
    }

    return b_Res;
}

void IOMux::PutString(const char* const STR_Out) const
{
    //! @warning This method should not be called. However, we redirect the call to the output stream.
    if ((m_arsOutputs[OUTPUT_STREAM].pcliOutput != NULL)
        // Protection against infinite loop.
        && (! m_bIOLocked))
    {
        m_bIOLocked = true;
        m_arsOutputs[OUTPUT_STREAM].pcliOutput->PutString(STR_Out);
        m_bIOLocked = false;
    }
}

void IOMux::Beep(void) const
{
    //! @warning This method should not be called. However, we redirect the call to the output stream.
    if ((m_arsOutputs[OUTPUT_STREAM].pcliOutput != NULL)
        // Protection against infinite loop.
        && (! m_bIOLocked))
    {
        m_bIOLocked = true;
        m_arsOutputs[OUTPUT_STREAM].pcliOutput->Beep();
        m_bIOLocked = false;
    }
}

const OutputDevice& IOMux::GetActualDevice(void) const
{
    if (m_arsOutputs[OUTPUT_STREAM].pcliOutput != NULL)
    {
        return m_arsOutputs[OUTPUT_STREAM].pcliOutput->GetActualDevice();
    }
    else
    {
        return OutputDevice::GetNullDevice().GetActualDevice();
    }
}

const KEY IOMux::GetKey(void) const
{
    if (CheckInput() && (! m_qInputs.IsEmpty()))
    {
        if (IODevice* const pcli_Input = m_qInputs.GetHead())
        {
            // Protection against infinite loop.
            if (! m_bIOLocked)
            {
                m_bIOLocked = true;
                KEY e_Key = pcli_Input->GetKey();
                m_bIOLocked = false;

                if (e_Key != NULL_KEY)
                {
                    return e_Key;
                }
                else if (const_cast<IOMux*>(this)->NextInput())
                {
                    return GetKey();
                }
            }
        }
    }
    else
    {
        // Note: m_cliLastError has been set within CheckInput()
        if (m_cliLastError.IsEmpty())
        {
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "IOMux: No input device")
                .SetString(ResourceString::LANG_FR, "IOMux: Pas de périphérique d'entrée");
        }
    }

    return cli::NULL_KEY;
}

const ResourceString IOMux::GetLocation(void) const
{
    if (! m_qInputs.IsEmpty())
    {
        if (IODevice* const pcli_Input = m_qInputs.GetHead())
        {
            return pcli_Input->GetLocation();
        }
    }

    return ResourceString();
}

const OutputDevice& IOMux::GetOutput(const STREAM_TYPE E_StreamType) const
{
    if ((E_StreamType >= 0) && (E_StreamType < STREAM_TYPES_COUNT)
        && (m_arsOutputs[E_StreamType].pcliOutput != NULL))
    {
        return *m_arsOutputs[E_StreamType].pcliOutput;
    }
    else
    {
        m_cliLastError
            .SetString(ResourceString::LANG_EN, "IOMux: No output device")
            .SetString(ResourceString::LANG_FR, "IOMux: Pas de périphérique de sortie");
        return OutputDevice::GetNullDevice();
    }
}

const bool IOMux::SetOutput(const STREAM_TYPE E_StreamType, OutputDevice* const PCLI_Stream)
{
    // ALL_STREAMS management.
    if (E_StreamType == ALL_STREAMS)
    {
        for (int i = 0; i < STREAM_TYPES_COUNT; i++)
        {
            if (! SetOutput((STREAM_TYPE) i, PCLI_Stream))
            {
                return false;
            }
        }

        return true;
    }
    if ((E_StreamType < 0) || (E_StreamType >= STREAM_TYPES_COUNT))
    {
        CLI_ASSERT(false);
        return false;
    }
    // End of ALL_STREAMS management.


    // Free previous device.
    if (! ReleaseOutputDevice(E_StreamType))
    {
        // Note: m_cliLastError has been set within ReleaseOutputDevice()
        return false;
    }

    // Store next device reference.
    m_arsOutputs[E_StreamType].pcliOutput = PCLI_Stream;
    // Lock the device instance.
    if (PCLI_Stream != NULL)
    {
        PCLI_Stream->UseInstance(__CALL_INFO__);
    }

    // Possibly open the device.
    if ((GetOpenUsers() > 0)
        && (m_arsOutputs[E_StreamType].pcliOutput != NULL)
        && (m_arsOutputs[E_StreamType].bDoOpenClose))
        //  // Protection against misfunctionning.
        //  && (m_arpcliOutputs[E_StreamType] != this))
    {
        if (! m_arsOutputs[E_StreamType].pcliOutput->OpenUp(__CALL_INFO__))
        {
            m_cliLastError = m_arsOutputs[E_StreamType].pcliOutput->GetLastError();
            return false;
        }
    }

    return true;
}

const IODevice* const IOMux::GetInput(void) const
{
    if (! m_qInputs.IsEmpty())
    {
        return m_qInputs.GetHead();
    }

    return NULL;
}

const bool IOMux::AddInput(IODevice* const PCLI_Input)
{
    if (PCLI_Input != NULL)
    {
        if (m_qInputs.AddTail(PCLI_Input))
        {
            PCLI_Input->UseInstance(__CALL_INFO__);
            return true;
        }
        else
        {
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "IOMux: Cannot add the input device to the input list")
                .SetString(ResourceString::LANG_FR, "IOMux: Impossible d'ajouter un périphérique d'entrée à la liste");
        }
    }
    else
    {
        m_cliLastError
            .SetString(ResourceString::LANG_EN, "IOMux: Cannot add a NULL input device")
            .SetString(ResourceString::LANG_FR, "IOMux: Impossible d'ajouter un périphérique d'entrée invalide");
    }

    return false;
}

const bool IOMux::NextInput(void)
{
    // Terminate head device.
    if (! m_qInputs.IsEmpty())
    {
        if (! ReleaseFirstInputDevice())
        {
            // Note: m_cliLastError has been set within ReleaseFirstInputDevice()
            return false;
        }
    }

    // Prepare next device.
    if (CheckInput() && (! m_qInputs.IsEmpty()))
    {
        if (IODevice* const pcli_Input = m_qInputs.GetHead())
        {
            if (GetOpenUsers() > 0)
            {
                if (! pcli_Input->OpenUp(__CALL_INFO__))
                {
                    // Open failure.
                    m_cliLastError = pcli_Input->GetLastError();
                    return false;
                }
            }

            // Successful return.
            return true;
        }
    }
    else
    {
        // Note: m_cliLastError has been set within CheckInput()
        if (m_cliLastError.IsEmpty())
        {
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "IOMux: No input device")
                .SetString(ResourceString::LANG_FR, "IOMux: Pas de périphérique d'entrée");
        }
    }

    // Default return indicates a failure.
    return false;
}

const bool IOMux::ResetInputList(void)
{
    bool b_Res = true;

    while (! m_qInputs.IsEmpty())
    {
        if (! ReleaseFirstInputDevice())
        {
            // Note: m_cliLastError has been set within ReleaseFirstInputDevice()
            b_Res = false;
        }
    }

    return b_Res;
}

IODevice* const IOMux::CreateInputDevice(void)
{
    return NULL;
}

const bool IOMux::CheckInput(void) const
{
    if (m_qInputs.IsEmpty())
    {
        // Input needed.
        if (IODevice* const pcli_Input = const_cast<IOMux*>(this)->CreateInputDevice())
        {
            if (! const_cast<IOMux*>(this)->AddInput(pcli_Input))
            {
                // Note: m_cliLastError has been set within AddInput().
                return false;
            }
            if (GetOpenUsers() > 0)
            {
                // Input should be opened.
                if (! pcli_Input->OpenUp(__CALL_INFO__))
                {
                    // Open failure.
                    m_cliLastError = pcli_Input->GetLastError();
                    return false;
                }
            }
        }
        else if (m_qInputs.IsEmpty())
        {
            // No more input.
            m_cliLastError
                .SetString(ResourceString::LANG_EN, "IOMux: No more input")
                .SetString(ResourceString::LANG_FR, "IOMux: Aucun périphérique d'entrée");
            return false;
        }
    }

    // Successful return.
    return true;
}

const bool IOMux::ReleaseFirstInputDevice(void)
{
    if (! m_qInputs.IsEmpty())
    {
        bool b_Res = true;

        if (IODevice* const pcli_Input = m_qInputs.RemoveHead())
        {
            if (GetOpenUsers() > 0)
            {
                // Close the device if needed.
                if (! pcli_Input->CloseDown(__CALL_INFO__))
                {
                    m_cliLastError = pcli_Input->GetLastError();
                    b_Res = false;
                }
            }
            // Free the device instance.
            pcli_Input->FreeInstance(__CALL_INFO__);
        }

        return b_Res;
    }
    else
    {
        m_cliLastError
            .SetString(ResourceString::LANG_EN, "IOMux: No more input")
            .SetString(ResourceString::LANG_FR, "IOMux: Aucun périphérique d'entrée");
        return false;
    }
}

const bool IOMux::ReleaseOutputDevice(const STREAM_TYPE E_StreamType)
{
    bool b_Res = true;

    // ALL_STREAMS management.
    if (E_StreamType == ALL_STREAMS)
    {
        for (int i = 0; i < STREAM_TYPES_COUNT; i++)
        {
            if (! ReleaseOutputDevice((STREAM_TYPE) i))
            {
                b_Res = false;
            }
        }

        return b_Res;
    }
    if ((E_StreamType < 0) || (E_StreamType >= STREAM_TYPES_COUNT))
    {
        CLI_ASSERT(false);
        return false;
    }
    // End of ALL_STREAMS management.

    if (m_arsOutputs[E_StreamType].pcliOutput != NULL)
    {
        if ((GetOpenUsers() > 0) && (m_arsOutputs[E_StreamType].bDoOpenClose))
        {
            // Close the device if needed.
            if (! m_arsOutputs[E_StreamType].pcliOutput->CloseDown(__CALL_INFO__))
            {
                m_cliLastError = m_arsOutputs[E_StreamType].pcliOutput->GetLastError();
                b_Res = false;
            }
        }
        // Release the device instance.
        m_arsOutputs[E_StreamType].pcliOutput->FreeInstance(__CALL_INFO__);

        // Remove reference.
        m_arsOutputs[E_StreamType].pcliOutput = NULL;
    }

    return b_Res;
}
