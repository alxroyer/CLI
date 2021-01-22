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


//! @file
//! @author Alexis Royer
//! @brief CcliOutputDevice, CcliIODevice and CcliIOEndl classes defintions.

#ifndef _CLI_IO_DEVICE_H_
#define _CLI_IO_DEVICE_H_

#include <string>


//! @brief End of line for input/output devices.
//! @see CLI_ENDL
class CcliIOEndl { public: CcliIOEndl(void) {} };

//! The common CcliIOEndl object.
//! CLI_ENDL can be passed to CcliOutputDevice to print carriage returns.
//!
//! It is better to use CLI_ENDL rather to print "\n" or "\r\n"
//! since this should depend on the CcliOutputDevice.
extern const CcliIOEndl CLI_ENDL;


//! @brief Generic output device.
//! @see CLI_ENDL
class CcliOutputDevice
{
public:
    //! @brief Constructor.
    CcliOutputDevice(		    
        const std::string& STR_Endl //!< Carriage return pattern.
        );

    //! @brief Destructor.
    virtual ~CcliOutputDevice(void);

public:
    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const std::string& STR_Out  //!< Output string object.
	) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const char* const STR_Out   //!< Output null terminated string.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const int I_Out             //!< Integer number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const unsigned int UI_Out   //!< Unsigned integer number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const char C_Out            //!< Single character.
	) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const unsigned char UC_Out  //!< Unsigned char number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const short S_Out           //!< Short number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const unsigned short US_Out //!< Unsigned short number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const long L_Out            //!< Long number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const unsigned long UL_Out  //!< Unsigned long number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const float F_Out           //!< Float number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const double D_Out          //!< Double number.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        void* const PV_Out          //!< Void address.
        ) const;

    //! @brief Output operator.
    //! @return The output device itself.
    template <class T>
    const CcliOutputDevice& operator<<(
        const T* const PT_Out       //!< Any type address.
        ) const
    {
        *this << (void*) PT_Out;
    }

    //! @brief Output operator.
    //! @return The output device itself.
    const CcliOutputDevice& operator<<(
        const CcliIOEndl& CLI_IOEndl    //!< Carriage return.
        ) const;

public:
    //! @brief Null device.
    static const CcliOutputDevice& GetNullDevice(void);

    //! @brief Standard output device.
    static const CcliOutputDevice& GetStdOut(void);

    //! @brief Standard error device.
    static const CcliOutputDevice& GetStdErr(void);

protected:
    //! @brief Output handler.
    virtual void PutString(
        const std::string& STR_Out  //!< Output string.
        ) const = 0;

public:
    //! @brief Beep handler.
    virtual void Beep(void) const = 0;

private:
    //! Carriage return pattern.
    const std::string m_strEndl;
};


//! @brief Input characters.
typedef enum
{
    CLI_NULL_KEY = '\0',    //!< Null key.

    CLI_BREAK = 3,          //!< Break (Ctrl+C).
    CLI_LOGOUT = 4,         //!< Logout (Ctrl+D).
    CLI_ENTER = 13,         //!< Enter.
    CLI_ESCAPE = 27,        //!< Escape.
    CLI_SPACE = 32,         //!< Space.
    CLI_BACKSPACE = '\b',   //!< Backspace.
    //CLI_DELETE = 128,       //!< Delete key.
    //CLI_CLEAR = 129,        //!< Clear key.

    CLI_TAB = '\t',
    CLI_0 = '0', CLI_1 = '1', CLI_2 = '2', CLI_3 = '3', CLI_4 = '4', CLI_5 = '5',
    CLI_6 = '6', CLI_7 = '7', CLI_8 = '8', CLI_9 = '9',
    CLI_a = 'a', CLI_b = 'b', CLI_c = 'c', CLI_d = 'd', CLI_e = 'e', CLI_f = 'f',
    CLI_g = 'g', CLI_h = 'h', CLI_i = 'i', CLI_j = 'j', CLI_k = 'k', CLI_l = 'l',
    CLI_m = 'm', CLI_n = 'n', CLI_o = 'o', CLI_p = 'p', CLI_q = 'q', CLI_r = 'r',
    CLI_s = 's', CLI_t = 't', CLI_u = 'u', CLI_v = 'v', CLI_w = 'w', CLI_x = 'x',
    CLI_y = 'y', CLI_z = 'z',
    CLI_A = 'A', CLI_B = 'B', CLI_C = 'C', CLI_D = 'D', CLI_E = 'E', CLI_F = 'F',
    CLI_G = 'G', CLI_H = 'H', CLI_I = 'I', CLI_J = 'J', CLI_K = 'K', CLI_L = 'L',
    CLI_M = 'M', CLI_N = 'N', CLI_O = 'O', CLI_P = 'P', CLI_Q = 'Q', CLI_R = 'R',
    CLI_S = 'S', CLI_T = 'T', CLI_U = 'U', CLI_V = 'V', CLI_W = 'W', CLI_X = 'X',
    CLI_Y = 'Y', CLI_Z = 'Z',

    CLI_PLUS = '+',
    CLI_MINUS = '-',
    CLI_STAR = '*',
    CLI_SLASH = '/',
    CLI_LOWER_THAN = '<',
    CLI_GREATER_THAN = '>',
    CLI_EQUAL = '=',

    CLI_UNDERSCORE = '_',
    CLI_AROBASE = '@',
    CLI_SHARP = '#',
    CLI_AMPERCENT = '&',
    CLI_DOLLAR = '$',
    CLI_BACKSLASH = '\\',

    CLI_QUESTION = '?',
    CLI_EXCLAMATION = '!',
    CLI_COLUMN = ':',
    CLI_DOT = '.',
    CLI_COMA = ',',
    CLI_SEMI_COLUMN = ';',
    CLI_QUOTE = '\'',
    CLI_DOUBLE_QUOTE = '"',

    CLI_OPENING_BRACE = '(',
    CLI_CLOSING_BRACE = ')',
    CLI_OPENING_CURLY_BRACE = '{',
    CLI_CLOSING_CURLY_BRACE = '}',
    CLI_OPENING_BRACKET = '[',
    CLI_CLOSING_BRACKET = ']',

    CLI_KEY_UP = 1001,             //!< Up arrow key.
    CLI_KEY_DOWN = 1002,           //!< Down arrow key.
    CLI_KEY_LEFT = 1003,           //!< Left arrow key.
    CLI_KEY_RIGHT = 1004,          //!< Right arrow key.
    CLI_PAGE_UP = 1005,            //!< Page up arrow key.
    CLI_PAGE_DOWN = 1006,          //!< Page down arrow key.
    //CLI_KEY_BEGIN = 1007,          //!< Begin key.
    //CLI_KEY_END = 1008,            //!< End key.
    //CLI_INSERT = 1009,             //!< Insert key.

    //CLI_COPY,               //!< Copy.
    //CLI_CUT,                //!< Cut.
    //CLI_PASTE,              //!< Paste.

    //CLI_UNDO,               //!< Undo.
    //CLI_REDO,               //!< Redo.
} CLI_CHAR;


//! @brief Generic input/output device.
//! @see CcliOutputDevice
class CcliIODevice : public CcliOutputDevice
{
public:
    //! @brief Constructor.
    CcliIODevice(
        const std::string& STR_Endl     //!< Carriage return pattern.
        );

    //! @brief Destructor.
    virtual ~CcliIODevice(void);

public:
    //! @brief Device opening handler.
    virtual const bool OpenDevice(void) = 0;

    //! @brief Device closure handler.
    virtual const bool CloseDevice(void) = 0;

    //! @brief Input character capture handler.
    virtual const CLI_CHAR GetChar(void) const = 0;

protected:
    //! @brief Common char translation.
    const CLI_CHAR GetChar(const int I_Char) const;
};


#endif // _CLI_IO_DEVICE_H_

