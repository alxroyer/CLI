/*
    Copyright 2006 Alexis Royer

    This file is part of the CLI library.

    The CLI library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <iostream>
#include "cli_io_device.h"


const CcliIOEndl CLI_ENDL;


CcliOutputDevice::CcliOutputDevice(const std::string& STR_Endl)
  : m_strEndl(STR_Endl)
{
}

CcliOutputDevice::~CcliOutputDevice(void)
{
}

const CcliOutputDevice& CcliOutputDevice::operator <<(const std::string& STR_Out) const
{
    PutString(STR_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(const char* STR_Out) const
{
    PutString(STR_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(char C_Out) const
{
    PutString(std::string() += C_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(int I_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%d", I_Out);
    PutString(str_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(unsigned int UI_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%u", UI_Out);
    PutString(str_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(void* PV_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "0x%08x", PV_Out);
    PutString(str_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(const CcliIOEndl& CLI_IOEndl) const
{
    PutString(m_strEndl);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::GetNullDevice(void)
{
    class CcliNullDevice : public CcliOutputDevice
    {
    public:
        CcliNullDevice(void) : CcliOutputDevice("") {}
        virtual ~CcliNullDevice(void) {}

    private:
        virtual void PutString(const std::string& STR_Out) const {}
    };

    static CcliNullDevice cli_Null;
    return cli_Null;
}

const CcliOutputDevice& CcliOutputDevice::GetStdOut(void)
{
    class CcliStdOutDevice : public CcliOutputDevice
    {
    public:
        CcliStdOutDevice(void) : CcliOutputDevice("\n") {}
        virtual ~CcliStdOutDevice(void) {}

    private:
        virtual void PutString(const std::string& STR_Out) const { std::cout << STR_Out; }
    };

    static CcliStdOutDevice cli_StdOut;
    return cli_StdOut;
}

const CcliOutputDevice& CcliOutputDevice::GetStdErr(void)
{
    class CcliStdErrDevice : public CcliOutputDevice
    {
    public:
        CcliStdErrDevice(void) : CcliOutputDevice("\n") {}
        virtual ~CcliStdErrDevice(void) {}

    private:
        virtual void PutString(const std::string& STR_Out) const { std::cerr << STR_Out; }
    };

    static CcliStdErrDevice cli_StdErr;
    return cli_StdErr;
}


CcliIODevice::CcliIODevice(const std::string& STR_Endl)
  : CcliOutputDevice(STR_Endl)
{
}

CcliIODevice::~CcliIODevice(void)
{
}

