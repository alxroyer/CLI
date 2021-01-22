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


#ifndef _CLI_IO_DEVICE_H_
#define _CLI_IO_DEVICE_H_

#include <string>


class CcliIOEndl { public: CcliIOEndl(void) {} };
extern const CcliIOEndl CLI_ENDL;


class CcliOutputDevice
{
public:
    CcliOutputDevice(const std::string& STR_Endl);
    virtual ~CcliOutputDevice(void);

public:
    const CcliOutputDevice& operator<<(const std::string& STR_Out) const;
    const CcliOutputDevice& operator<<(const char* STR_Out) const;
    const CcliOutputDevice& operator<<(int I_Out) const;
    const CcliOutputDevice& operator<<(unsigned int UI_Out) const;
    const CcliOutputDevice& operator<<(char C_Out) const;
    const CcliOutputDevice& operator<<(unsigned char UC_Out) const;
    const CcliOutputDevice& operator<<(short S_Out) const;
    const CcliOutputDevice& operator<<(unsigned short US_Out) const;
    const CcliOutputDevice& operator<<(long L_Out) const;
    const CcliOutputDevice& operator<<(unsigned long UL_Out) const;
    const CcliOutputDevice& operator<<(void* PV_Out) const;
    template <class T>
    const CcliOutputDevice& operator<<(T* PT_Out) const
    {
        *this << (void*) PT_Out;
    }
    const CcliOutputDevice& operator<<(const CcliIOEndl& CLI_IOEndl) const;

public:
    static const CcliOutputDevice& GetNullDevice(void);
    static const CcliOutputDevice& GetStdOut(void);
    static const CcliOutputDevice& GetStdErr(void);

private:
    virtual void PutString(const std::string& STR_Out) const = 0;

private:
    const std::string m_strEndl;
};

class CcliIODevice : public CcliOutputDevice
{
public:
    CcliIODevice(const std::string& STR_Endl);
    virtual ~CcliIODevice(void);

public:
    virtual bool OpenDevice(void) = 0;
    virtual bool CloseDevice(void) = 0;
    virtual int GetChar(void) const = 0;
};


#endif // _CLI_IO_DEVICE_H_


