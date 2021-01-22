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


#include "cli/single_command.h"
#include "consistency.h"

using namespace cli;


SingleCommand::SingleCommand(
        const std::string& STR_Command,
        OutputDevice& CLI_Output,
        const bool B_AutoDelete)
  : IODevice("cmd[" + STR_Command + "]", "\n", B_AutoDelete), m_cliOutput(CLI_Output),
    m_strCommand(STR_Command), m_iPosition(-1)
{
    EnsureCommonDevices();

    m_cliOutput.UseInstance(__CALL_INFO__);
}

SingleCommand::~SingleCommand(void)
{
    m_cliOutput.FreeInstance(__CALL_INFO__);
}

const std::string& SingleCommand::GetCommand(void) const
{
    return m_strCommand;
}

const bool SingleCommand::OpenDevice(void)
{
    if (! m_cliOutput.OpenUp(__CALL_INFO__))
    {
        return false;
    }

    m_iPosition = 0;
    return true;
}

const bool SingleCommand::CloseDevice(void)
{
    bool b_Res = true;

    m_iPosition = -1;

    if (! m_cliOutput.CloseDown(__CALL_INFO__))
    {
        b_Res = false;
    }

    return b_Res;
}

const KEY SingleCommand::GetKey(void) const
{
    if ((m_iPosition >= 0) && (m_iPosition < (signed) m_strCommand.size()))
    {
        const char c_Char = m_strCommand[m_iPosition];
        m_iPosition ++;
        return IODevice::GetKey(c_Char);
    }
    else if (m_iPosition == (signed) m_strCommand.size())
    {
        m_iPosition ++;
        return IODevice::GetKey('\n');
    }
    else
    {
        return NULL_KEY;
    }
}

void SingleCommand::PutString(const std::string& STR_Out) const
{
    m_cliOutput.PutString(STR_Out);
}

void SingleCommand::Beep(void) const
{
    m_cliOutput.Beep();
}
