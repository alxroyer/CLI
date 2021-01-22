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


#include <assert.h>

#include "cli/file_device.h"
#include "consistency.h"

using namespace cli;


InputFileDevice::InputFileDevice(
        const std::string& STR_FileName,
        OutputDevice& CLI_Output,
        const bool B_AutoDelete)
  : IODevice("input-file[" + STR_FileName + "]", "\n", B_AutoDelete),
    m_strFileName(STR_FileName), m_stdFile(),
    m_cliOutput(CLI_Output),
    m_iCurrentLine(0), m_iCurrentColumn(0), m_iNextLine(1), m_iNextColumn(1)
{
    EnsureCommonDevices();

    m_cliOutput.UseInstance(__CALL_INFO__);
}

InputFileDevice::~InputFileDevice(void)
{
    m_cliOutput.FreeInstance(__CALL_INFO__);
}

const bool InputFileDevice::OpenDevice(void)
{
    assert(& m_cliOutput != this);
    if (! m_cliOutput.OpenUp(__CALL_INFO__))
    {
        return false;
    }

    if (! m_stdFile.is_open())
    {
        m_stdFile.open(m_strFileName.c_str());
    }

    return m_stdFile.is_open();
}

const bool InputFileDevice::CloseDevice(void)
{
    bool b_Res = true;

    if (m_stdFile.is_open())
    {
        m_stdFile.close();
        if (m_stdFile.is_open())
        {
            b_Res = false;
        }
    }

    if (! m_cliOutput.CloseDown(__CALL_INFO__))
    {
        b_Res = false;
    }

    return b_Res;
}

const KEY InputFileDevice::GetKey(void) const
{
    if (m_stdFile.is_open())
    {
        m_iCurrentLine = m_iNextLine;
        m_iCurrentColumn = m_iNextColumn;
        int i_Char = m_stdFile.get();
        if (i_Char != m_stdFile.eof())
        {
            if (i_Char == '\n')
            {
                m_iNextLine ++;
                m_iNextColumn = 1;
            }
            else
            {
                m_iNextColumn ++;
            }
            return IODevice::GetKey(i_Char);
        }
    }

    return NULL_KEY;
}

void InputFileDevice::PutString(const std::string& STR_Out) const
{
    m_cliOutput.PutString(STR_Out);
}

void InputFileDevice::Beep(void) const
{
    m_cliOutput.Beep();
}

const std::string& InputFileDevice::GetFileName(void) const
{
    return m_strFileName;
}

const int InputFileDevice::GetCurrentLine(void) const
{
    return m_iCurrentLine;
}

const int InputFileDevice::GetCurrentColumn(void) const
{
    return m_iCurrentColumn;
}


OutputFileDevice::OutputFileDevice(const std::string& STR_FileName, const bool B_AutoDelete)
  : OutputDevice("output-file[" + STR_FileName + "]", "\n", B_AutoDelete),
    m_strFileName(STR_FileName), m_stdFile()
{
}

OutputFileDevice::~OutputFileDevice(void)
{
}

const bool OutputFileDevice::OpenDevice(void)
{
    if (! m_stdFile.is_open())
    {
        m_stdFile.open(m_strFileName.c_str());
    }

    return m_stdFile.is_open();
}

const bool OutputFileDevice::CloseDevice(void)
{
    if (m_stdFile.is_open())
    {
        m_stdFile.close();
    }

    return (! m_stdFile.is_open());
}

void OutputFileDevice::PutString(const std::string& STR_Out) const
{
    if (m_stdFile.is_open())
    {
        m_stdFile << STR_Out;
    }
}

void OutputFileDevice::Beep(void) const
{
}

const std::string& OutputFileDevice::GetFileName(void) const
{
    return m_strFileName;
}

