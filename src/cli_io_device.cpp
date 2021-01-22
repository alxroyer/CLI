/*
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006 Alexis Royer.

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

const CcliOutputDevice& CcliOutputDevice::operator <<(const char* const STR_Out) const
{
    PutString(STR_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(const char C_Out) const
{
    PutString(std::string() += C_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(const int I_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%d", I_Out);
    PutString(str_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(const unsigned int UI_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%u", UI_Out);
    PutString(str_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(const float F_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%f", F_Out);
    PutString(str_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(const double D_Out) const
{
    char str_Out[128];
    snprintf(str_Out, sizeof(str_Out), "%f", D_Out);
    PutString(str_Out);
    return *this;
}

const CcliOutputDevice& CcliOutputDevice::operator <<(void* const PV_Out) const
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
    public:
        virtual void Beep(void) const {}
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
    public:
        virtual void Beep(void) const {}
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
    public:
        virtual void Beep(void) const {}
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

const CLI_CHAR CcliIODevice::GetChar(const int I_Char) const
{
    switch (I_Char)
    {
    case 3:             return CLI_BREAK;
    case 4:             return CLI_LOGOUT;
    case 10: case 13:   return CLI_ENTER;
    case 27:            return CLI_ESCAPE;
    case 127:           return CLI_BACKSPACE;
    case 1001:          return CLI_KEY_UP;
    case 1002:          return CLI_KEY_DOWN;
    case 1005:          return CLI_PAGE_UP;
    case 1006:          return CLI_PAGE_DOWN;

    case ' ':           return CLI_SPACE;
    case '\t':          return CLI_TAB;

    case '0':   return CLI_0;
    case '1':   return CLI_1;
    case '2':   return CLI_2;
    case '3':   return CLI_3;
    case '4':   return CLI_4;
    case '5':   return CLI_5;
    case '6':   return CLI_6;
    case '7':   return CLI_7;
    case '8':   return CLI_8;
    case '9':   return CLI_9;

    case 'a':   return CLI_a;
    case 'b':   return CLI_b;
    case 'c':   return CLI_c;
    case 'd':   return CLI_d;
    case 'e':   return CLI_e;
    case 'f':   return CLI_f;
    case 'g':   return CLI_g;
    case 'h':   return CLI_h;
    case 'i':   return CLI_i;
    case 'j':   return CLI_j;
    case 'k':   return CLI_k;
    case 'l':   return CLI_l;
    case 'm':   return CLI_m;
    case 'n':   return CLI_n;
    case 'o':   return CLI_o;
    case 'p':   return CLI_p;
    case 'q':   return CLI_q;
    case 'r':   return CLI_r;
    case 's':   return CLI_s;
    case 't':   return CLI_t;
    case 'u':   return CLI_u;
    case 'v':   return CLI_v;
    case 'w':   return CLI_w;
    case 'x':   return CLI_x;
    case 'y':   return CLI_y;
    case 'z':   return CLI_z;

    case 'A':   return CLI_A;
    case 'B':   return CLI_B;
    case 'C':   return CLI_C;
    case 'D':   return CLI_D;
    case 'E':   return CLI_E;
    case 'F':   return CLI_F;
    case 'G':   return CLI_G;
    case 'H':   return CLI_H;
    case 'I':   return CLI_I;
    case 'J':   return CLI_J;
    case 'K':   return CLI_K;
    case 'L':   return CLI_L;
    case 'M':   return CLI_M;
    case 'N':   return CLI_N;
    case 'O':   return CLI_O;
    case 'P':   return CLI_P;
    case 'Q':   return CLI_Q;
    case 'R':   return CLI_R;
    case 'S':   return CLI_S;
    case 'T':   return CLI_T;
    case 'U':   return CLI_U;
    case 'V':   return CLI_V;
    case 'W':   return CLI_W;
    case 'X':   return CLI_X;
    case 'Y':   return CLI_Y;
    case 'Z':   return CLI_Z;

    case '+':   return CLI_PLUS;
    case '-':   return CLI_MINUS;
    case '*':   return CLI_STAR;
    case '/':   return CLI_SLASH;
    case '<':   return CLI_LOWER_THAN;
    case '>':   return CLI_GREATER_THAN;
    case '=':   return CLI_EQUAL;

    case '_':   return CLI_UNDERSCORE;
    case '@':   return CLI_AROBASE;
    case '#':   return CLI_SHARP;
    case '&':   return CLI_AMPERCENT;
    case '$':   return CLI_DOLLAR;
    case '\\':  return CLI_BACKSLASH;

    case '?':   return CLI_QUESTION;
    case '!':   return CLI_EXCLAMATION;
    case ':':   return CLI_COLUMN;
    case '.':   return CLI_DOT;
    case ',':   return CLI_COMA;
    case ';':   return CLI_SEMI_COLUMN;
    case '\'':  return CLI_QUOTE;
    case '"':   return CLI_DOUBLE_QUOTE;

    case '(':   return CLI_OPENING_BRACE;
    case ')':   return CLI_CLOSING_BRACE;
    case '{':   return CLI_OPENING_CURLY_BRACE;
    case '}':   return CLI_CLOSING_CURLY_BRACE;
    case '[':   return CLI_OPENING_BRACKET;
    case ']':   return CLI_CLOSING_BRACKET;

    default:
        // Unrecognized character.
        return CLI_NULL_KEY;
    }
}
