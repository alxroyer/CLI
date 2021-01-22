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


#include <iostream>
#include "cli/io_device.h"
#include "cli/cli.h"
#include "cli/traces.h"

using namespace cli;


const IOEndl cli::endl;

static const TraceClass TRACE_IO_DEVICE_INSTANCES("IO_DEVICE_INSTANCES", "IO device instance management");
static const TraceClass TRACE_IO_DEVICE_OPENING("IO_DEVICE_OPENING", "IO device opening management");


OutputDevice::OutputDevice(
        const std::string& STR_DbgName,
        const std::string& STR_Endl,
        const bool B_AutoDelete)
  : m_strDbgName(STR_DbgName),
    m_strEndl(STR_Endl),
    m_iInstanceLock(B_AutoDelete ? 0 : 1), m_iOpenLock(0)
{
    // Please, no traces in constructor for consistency reasons.
}

OutputDevice::~OutputDevice(void)
{
}

const int OutputDevice::UseInstance(const CallInfo& CLI_CallInfo)
{
    GetTraces().Trace(TRACE_IO_DEVICE_INSTANCES)
        << "One more user for instance " << m_strDbgName << "/" << this << ", "
        << "user count: " << m_iInstanceLock << " -> " << m_iInstanceLock + 1 << ", "
        << "from " << CLI_CallInfo.GetFunction() << " "
        << "at " << CLI_CallInfo.GetFile() << ":" << CLI_CallInfo.GetLine() << endl;
    m_iInstanceLock ++;
    return m_iInstanceLock;
}

const int OutputDevice::FreeInstance(const CallInfo& CLI_CallInfo)
{
    GetTraces().Trace(TRACE_IO_DEVICE_INSTANCES)
        << "One less user for instance " << m_strDbgName << "/" << this << ", "
        << "user count: " << m_iInstanceLock << " -> " << m_iInstanceLock - 1 << ", "
        << "from " << CLI_CallInfo.GetFunction() << " "
        << "at " << CLI_CallInfo.GetFile() << ":" << CLI_CallInfo.GetLine() << endl;
    if (m_iInstanceLock == 1)
    {
        GetTraces().Trace(TRACE_IO_DEVICE_INSTANCES)
            << "Deleting the device " << m_strDbgName << "/" << this << endl;
        delete this;
        return 0;
    }
    else
    {
        m_iInstanceLock --;
        assert(m_iInstanceLock > 0);
        return m_iInstanceLock;
    }
}

const int OutputDevice::GetInstanceUsers(void) const
{
    return m_iInstanceLock;
}

const bool OutputDevice::OpenUp(const CallInfo& CLI_CallInfo)
{
    GetTraces().Trace(TRACE_IO_DEVICE_OPENING)
        << "One more user for instance " << m_strDbgName << "/" << this << ", "
        << "user count: " << m_iOpenLock << " -> " << m_iOpenLock + 1 << ", "
        << "from " << CLI_CallInfo.GetFunction() << " "
        << "at " << CLI_CallInfo.GetFile() << ":" << CLI_CallInfo.GetLine() << endl;

    m_iOpenLock ++;

    if (m_iOpenLock == 1)
    {
        GetTraces().Trace(TRACE_IO_DEVICE_OPENING)
            << "Opening the device " << m_strDbgName << "/" << this << endl;
        if (! OpenDevice())
        {
            return false;
        }
    }

    return true;
}

const bool OutputDevice::CloseDown(const CallInfo& CLI_CallInfo)
{
    bool b_Res = true;

    if (m_iOpenLock > 0)
    {
        GetTraces().Trace(TRACE_IO_DEVICE_OPENING)
            << "One less user for instance " << m_strDbgName << "/" << this << ", "
            << "user count: " << m_iOpenLock << " -> " << m_iOpenLock - 1 << ", "
            << "from " << CLI_CallInfo.GetFunction() << " "
            << "at " << CLI_CallInfo.GetFile() << ":" << CLI_CallInfo.GetLine() << endl;

        if (m_iOpenLock == 1)
        {
            GetTraces().Trace(TRACE_IO_DEVICE_OPENING)
                << "Closing the device " << m_strDbgName << "/" << this << endl;
            b_Res = CloseDevice();
        }

        m_iOpenLock --;
    }
    else
    {
        GetTraces().Trace(TRACE_IO_DEVICE_OPENING)
            << "No more closing down for instance " << m_strDbgName << "/" << this << ", "
            << "user count = " << m_iOpenLock << ", "
            << "from " << CLI_CallInfo.GetFunction() << " "
            << "at " << CLI_CallInfo.GetFile() << ":" << CLI_CallInfo.GetLine() << endl;
    }

    return b_Res;
}

const int OutputDevice::GetOpenUsers(void) const
{
    return m_iOpenLock;
}

const OutputDevice& OutputDevice::operator <<(const std::string& STR_Out) const
{
    PutString(STR_Out);
    return *this;
}

const OutputDevice& OutputDevice::operator <<(const char* const STR_Out) const
{
    if (STR_Out != NULL)
    {
        PutString(STR_Out);
    }
    return *this;
}

const OutputDevice& OutputDevice::operator <<(const unsigned char UC_Out) const
{
    return this->operator<<((unsigned int) UC_Out);
}

const OutputDevice& OutputDevice::operator <<(const char C_Out) const
{
    PutString(std::string() += C_Out);
    return *this;
}

const OutputDevice& OutputDevice::operator <<(const short S_Out) const
{
    return this->operator<<((int) S_Out);
}

const OutputDevice& OutputDevice::operator <<(const unsigned short US_Out) const
{
    return this->operator<<((unsigned int) US_Out);
}

const OutputDevice& OutputDevice::operator <<(const long L_Out) const
{
    return this->operator <<((int) L_Out);
}

const OutputDevice& OutputDevice::operator <<(const unsigned long UL_Out) const
{
    return this->operator <<((unsigned int) UL_Out);
}

const OutputDevice& OutputDevice::operator <<(const int I_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%d", I_Out);
    PutString(str_Out);
    return *this;
}

const OutputDevice& OutputDevice::operator <<(const unsigned int UI_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%u", UI_Out);
    PutString(str_Out);
    return *this;
}

const OutputDevice& OutputDevice::operator <<(const float F_Out) const
{
    return this->operator <<((double) F_Out);
}

const OutputDevice& OutputDevice::operator <<(const double D_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%f", D_Out);
    PutString(str_Out);
    return *this;
}

const OutputDevice& OutputDevice::operator <<(void* const PV_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "0x%08x", (unsigned int) PV_Out);
    PutString(str_Out);
    return *this;
}

const OutputDevice& OutputDevice::operator <<(const IOEndl& CLI_IOEndl) const
{
    PutString(m_strEndl);
    return *this;
}

OutputDevice& OutputDevice::GetNullDevice(void)
{
    class NullDevice : public OutputDevice
    {
    public:
        NullDevice(void) : OutputDevice("null", "", false) {}
        virtual ~NullDevice(void) {}

    protected:
        virtual const bool OpenDevice(void) { return true; }
        virtual const bool CloseDevice(void) { return true; }
    public:
        virtual void PutString(const std::string& STR_Out) const {}
        virtual void Beep(void) const {}
    };

    static NullDevice cli_Null;
    return cli_Null;
}

OutputDevice& OutputDevice::GetStdOut(void)
{
    class StdOutDevice : public OutputDevice
    {
    public:
        StdOutDevice(void) : OutputDevice("stdout", "\n", false) {}
        virtual ~StdOutDevice(void) {}

    protected:
        virtual const bool OpenDevice(void) { return true; }
        virtual const bool CloseDevice(void) { return true; }
    public:
        virtual void PutString(const std::string& STR_Out) const { std::cout << STR_Out; }
        virtual void Beep(void) const {}
    };

    static StdOutDevice cli_StdOut;
    return cli_StdOut;
}

OutputDevice& OutputDevice::GetStdErr(void)
{
    class StdErrDevice : public OutputDevice
    {
    public:
        StdErrDevice(void) : OutputDevice("stderr", "\n", false) {}
        virtual ~StdErrDevice(void) {}

    protected:
        virtual const bool OpenDevice(void) { return true; }
        virtual const bool CloseDevice(void) { return true; }
    public:
        virtual void PutString(const std::string& STR_Out) const { std::cerr << STR_Out; }
        virtual void Beep(void) const {}
    };

    static StdErrDevice cli_StdErr;
    return cli_StdErr;
}


IODevice::IODevice(
        const std::string& STR_DbgName,
        const std::string& STR_Endl,
        const bool B_AutoDelete)
  : OutputDevice(STR_DbgName, STR_Endl, B_AutoDelete)
{
}

IODevice::~IODevice(void)
{
}

IODevice& IODevice::GetNullDevice(void)
{
    class NullDevice : public IODevice
    {
    public:
        NullDevice(void) : IODevice("null", "", false) {}
        virtual ~NullDevice(void) {}

    protected:
        virtual const bool OpenDevice(void) { return true; }
        virtual const bool CloseDevice(void) { return true; }
    public:
        virtual void PutString(const std::string& STR_Out) const {}
        virtual void Beep(void) const {}
        virtual const KEY GetKey(void) const { return NULL_KEY; }
    };

    static NullDevice cli_Null;
    return cli_Null;
}

IODevice& IODevice::GetStdIn(void)
{
    class StdInDevice : public IODevice
    {
    public:
        StdInDevice(void) : IODevice("stdin", "", false) {
        }
        virtual ~StdInDevice(void) {
        }

    protected:
        virtual const bool OpenDevice(void) {
            OutputDevice::GetStdOut().UseInstance(__CALL_INFO__);
            return OutputDevice::GetStdOut().OpenUp(__CALL_INFO__);
        }
        virtual const bool CloseDevice(void) {
            bool b_Res = OutputDevice::GetStdOut().CloseDown(__CALL_INFO__);
            OutputDevice::GetStdOut().FreeInstance(__CALL_INFO__);
            return b_Res;
        }
    public:
        virtual void PutString(const std::string& STR_Out) const {
            OutputDevice::GetStdOut().PutString(STR_Out);
        }
        virtual void Beep(void) const {
            OutputDevice::GetStdOut().Beep();
        }
        virtual const KEY GetKey(void) const {
            if (! m_strInput.empty())
            {
                const char c_Char = m_strInput[0];
                m_strInput.erase(m_strInput.begin());
                return IODevice::GetKey(c_Char);
            }
            else
            {
                std::cin >> m_strInput;
                m_strInput += "\n";
                return StdInDevice::GetKey();
            }
        }

    private:
        mutable std::string m_strInput;
    };

    static StdInDevice cli_StdIn;
    return cli_StdIn;
}

const KEY IODevice::GetKey(const int I_Char) const
{
    switch (I_Char)
    {
    case 3:             return BREAK;
    case 4:             return LOGOUT;
    case 10: case 13:   return ENTER;
    case 27:            return ESCAPE;
    case 127:           return BACKSPACE;
    case 1001:          return KEY_UP;
    case 1002:          return KEY_DOWN;
    case 1005:          return PAGE_UP;
    case 1006:          return PAGE_DOWN;

    case ' ':           return SPACE;
    case '\t':          return TAB;

    case '0':   return KEY_0;
    case '1':   return KEY_1;
    case '2':   return KEY_2;
    case '3':   return KEY_3;
    case '4':   return KEY_4;
    case '5':   return KEY_5;
    case '6':   return KEY_6;
    case '7':   return KEY_7;
    case '8':   return KEY_8;
    case '9':   return KEY_9;

    case 'a':   return KEY_a;
    case 'b':   return KEY_b;
    case 'c':   return KEY_c;
    case 'd':   return KEY_d;
    case 'e':   return KEY_e;
    case 'f':   return KEY_f;
    case 'g':   return KEY_g;
    case 'h':   return KEY_h;
    case 'i':   return KEY_i;
    case 'j':   return KEY_j;
    case 'k':   return KEY_k;
    case 'l':   return KEY_l;
    case 'm':   return KEY_m;
    case 'n':   return KEY_n;
    case 'o':   return KEY_o;
    case 'p':   return KEY_p;
    case 'q':   return KEY_q;
    case 'r':   return KEY_r;
    case 's':   return KEY_s;
    case 't':   return KEY_t;
    case 'u':   return KEY_u;
    case 'v':   return KEY_v;
    case 'w':   return KEY_w;
    case 'x':   return KEY_x;
    case 'y':   return KEY_y;
    case 'z':   return KEY_z;

    case 'A':   return KEY_A;
    case 'B':   return KEY_B;
    case 'C':   return KEY_C;
    case 'D':   return KEY_D;
    case 'E':   return KEY_E;
    case 'F':   return KEY_F;
    case 'G':   return KEY_G;
    case 'H':   return KEY_H;
    case 'I':   return KEY_I;
    case 'J':   return KEY_J;
    case 'K':   return KEY_K;
    case 'L':   return KEY_L;
    case 'M':   return KEY_M;
    case 'N':   return KEY_N;
    case 'O':   return KEY_O;
    case 'P':   return KEY_P;
    case 'Q':   return KEY_Q;
    case 'R':   return KEY_R;
    case 'S':   return KEY_S;
    case 'T':   return KEY_T;
    case 'U':   return KEY_U;
    case 'V':   return KEY_V;
    case 'W':   return KEY_W;
    case 'X':   return KEY_X;
    case 'Y':   return KEY_Y;
    case 'Z':   return KEY_Z;

    case '+':   return PLUS;
    case '-':   return MINUS;
    case '*':   return STAR;
    case '/':   return SLASH;
    case '<':   return LOWER_THAN;
    case '>':   return GREATER_THAN;
    case '=':   return EQUAL;
    case '%':   return PERCENT;

    case '_':   return UNDERSCORE;
    case '@':   return AROBASE;
    case '#':   return SHARP;
    case '&':   return AMPERCENT;
    case '$':   return DOLLAR;
    case '\\':  return BACKSLASH;
    case '|':   return PIPE;

    case '?':   return QUESTION;
    case '!':   return EXCLAMATION;
    case ':':   return COLUMN;
    case '.':   return DOT;
    case ',':   return COMA;
    case ';':   return SEMI_COLUMN;
    case '\'':  return QUOTE;
    case '"':   return DOUBLE_QUOTE;

    case '(':   return OPENING_BRACE;
    case ')':   return CLOSING_BRACE;
    case '{':   return OPENING_CURLY_BRACE;
    case '}':   return CLOSING_CURLY_BRACE;
    case '[':   return OPENING_BRACKET;
    case ']':   return CLOSING_BRACKET;

    default:
        // Unrecognized character.
        return NULL_KEY;
    }
}
