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

