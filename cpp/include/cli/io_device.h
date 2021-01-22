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


//! @file
//! @author Alexis Royer
//! @brief OutputDevice, IODevice and IOEndl classes defintion.

#ifndef _CLI_IO_DEVICE_H_
#define _CLI_IO_DEVICE_H_

#include <string>

#include <cli/debug.h>


namespace cli {

    //! @brief End of line for input/output devices.
    //! @see endl
    class IOEndl {
    public:
        //! @brief Default constructor.
        IOEndl(void) {}

    private:
        //! @brief No copy constructor.
        IOEndl(const IOEndl&);
    };

    //! The common IOEndl object.
    //! endl can be passed to OutputDevice to print carriage returns.
    //!
    //! It is better to use endl rather to print "\n" or "\r\n"
    //! since this should depend on the OutputDevice.
    extern const IOEndl endl;


    //! @brief Generic output device.
    //! @see endl
    class OutputDevice
    {
    private:
        //! @brief No default constructor.
        OutputDevice(void);
        //! @brief No copy constructor.
        OutputDevice(const OutputDevice&);

    protected:
        //! @brief Constructor.
        OutputDevice(
            const std::string& STR_DbgName, //!< Debug name. Useful for traces only.
            const std::string& STR_Endl,    //!< Carriage return pattern.
            const bool B_AutoDelete         //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~OutputDevice(void);

    public:
        //! @brief Ensures instance validity.
        //! @return Total number of instance users after this call.
        const int UseInstance(
            const CallInfo& CLI_CallInfo    //!< Call information.
            );

        //! @brief Releases the instance.
        //! @return Total number of instance users after this call.
        //!
        //! If the auto-deletion flag has been set during construction,
        //! the object is auto-deleted when the number of users reaches 0 on this call.
        const int FreeInstance(
            const CallInfo& CLI_CallInfo    //!< Call information.
            );

        //! @brief Instance user count access.
        const int GetInstanceUsers(void) const;

    public:
        //! @brief Checks the device is opened.
        //!
        //! Opens the device if not already opened.
        //! Acquire a lock on the open state in any case.
        const bool OpenUp(
            const CallInfo& CLI_CallInfo    //!< Call information.
            );

        //! @brief Indicates the device the client does not need the device to opened anymore.
        //!
        //! Releases the lock on the open state.
        //! When no more user need the device to be opened, it is closed straight forward.
        const bool CloseDown(
            const CallInfo& CLI_CallInfo    //!< Call information.
            );

        //! @brief Open state user count access.
        const int GetOpenUsers(void) const;

    protected:
        //! @brief Device opening handler.
        //! @note Devices have to be prepared to be called several times through this method.
        //!       They should do the opening once only
        //!       (unless they have been closed between OpendDevice() calls),
        //!       and indicate no failure thereafter.
        virtual const bool OpenDevice(void) = 0;

        //! @brief Device closure handler.
        //! @note Devices have to be prepared to be called several times through this method.
        //!       They should do the closure once only
        //!       (unless they have been opened between CloseDevice() calls),
        //!       and indicate no failure thereafter.
        virtual const bool CloseDevice(void) = 0;

    public:
        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const std::string& STR_Out  //!< Output string object.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const char* const STR_Out   //!< Output null terminated string.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const int I_Out             //!< Integer number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const unsigned int UI_Out   //!< Unsigned integer number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const char C_Out            //!< Single character.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const unsigned char UC_Out  //!< Unsigned char number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const short S_Out           //!< Short number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const unsigned short US_Out //!< Unsigned short number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const long L_Out            //!< Long number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const unsigned long UL_Out  //!< Unsigned long number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const float F_Out           //!< Float number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const double D_Out          //!< Double number.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            void* const PV_Out          //!< Void address.
            ) const;

        //! @brief Output operator.
        //! @return The output device itself.
        const OutputDevice& operator<<(
            const IOEndl& CLI_IOEndl    //!< Carriage return.
            ) const;

    public:
        //! @brief Null device.
        static OutputDevice& GetNullDevice(void);

        //! @brief Standard output device.
        static OutputDevice& GetStdOut(void);

        //! @brief Standard error device.
        static OutputDevice& GetStdErr(void);

    public:
        //! @brief Output handler.
        virtual void PutString(
            const std::string& STR_Out  //!< Output string.
            ) const = 0;

        //! @brief Beep handler.
        virtual void Beep(void) const = 0;

    private:
        //! Debug name. Useful for traces only.
        const std::string m_strDbgName;

        //! Carriage return pattern.
        const std::string m_strEndl;

        //! Instance lock count.
        int m_iInstanceLock;

        //! Open state lock count.
        int m_iOpenLock;
    };


    //! @brief Input characters.
    typedef enum
    {
        NULL_KEY = '\0',    //!< Null key.

        BREAK = 3,          //!< Break (Ctrl+C).
        LOGOUT = 4,         //!< Logout (Ctrl+D).
        ENTER = 13,         //!< Enter.
        ESCAPE = 27,        //!< Escape.
        SPACE = 32,         //!< Space.
        BACKSPACE = '\b',   //!< Backspace.
        //DELETE = 128,       //!< Delete key.
        //CLEAR = 129,        //!< Clear key.

        TAB = '\t',
        KEY_0 = '0', KEY_1 = '1', KEY_2 = '2', KEY_3 = '3', KEY_4 = '4', KEY_5 = '5',
        KEY_6 = '6', KEY_7 = '7', KEY_8 = '8', KEY_9 = '9',
        KEY_a = 'a', KEY_b = 'b', KEY_c = 'c', KEY_d = 'd', KEY_e = 'e', KEY_f = 'f',
        KEY_g = 'g', KEY_h = 'h', KEY_i = 'i', KEY_j = 'j', KEY_k = 'k', KEY_l = 'l',
        KEY_m = 'm', KEY_n = 'n', KEY_o = 'o', KEY_p = 'p', KEY_q = 'q', KEY_r = 'r',
        KEY_s = 's', KEY_t = 't', KEY_u = 'u', KEY_v = 'v', KEY_w = 'w', KEY_x = 'x',
        KEY_y = 'y', KEY_z = 'z',
        KEY_A = 'A', KEY_B = 'B', KEY_C = 'C', KEY_D = 'D', KEY_E = 'E', KEY_F = 'F',
        KEY_G = 'G', KEY_H = 'H', KEY_I = 'I', KEY_J = 'J', KEY_K = 'K', KEY_L = 'L',
        KEY_M = 'M', KEY_N = 'N', KEY_O = 'O', KEY_P = 'P', KEY_Q = 'Q', KEY_R = 'R',
        KEY_S = 'S', KEY_T = 'T', KEY_U = 'U', KEY_V = 'V', KEY_W = 'W', KEY_X = 'X',
        KEY_Y = 'Y', KEY_Z = 'Z',

        PLUS = '+',
        MINUS = '-',
        STAR = '*',
        SLASH = '/',
        LOWER_THAN = '<',
        GREATER_THAN = '>',
        EQUAL = '=',
        PERCENT = '%',

        UNDERSCORE = '_',
        AROBASE = '@',
        SHARP = '#',
        AMPERCENT = '&',
        DOLLAR = '$',
        BACKSLASH = '\\',
        PIPE = '|',

        QUESTION = '?',
        EXCLAMATION = '!',
        COLUMN = ':',
        DOT = '.',
        COMA = ',',
        SEMI_COLUMN = ';',
        QUOTE = '\'',
        DOUBLE_QUOTE = '"',

        OPENING_BRACE = '(',
        CLOSING_BRACE = ')',
        OPENING_CURLY_BRACE = '{',
        CLOSING_CURLY_BRACE = '}',
        OPENING_BRACKET = '[',
        CLOSING_BRACKET = ']',

        KEY_UP = 1001,              //!< Up arrow key.
        KEY_DOWN = 1002,           //!< Down arrow key.
        KEY_LEFT = 1003,           //!< Left arrow key.
        KEY_RIGHT = 1004,          //!< Right arrow key.
        PAGE_UP = 1005,            //!< Page up arrow key.
        PAGE_DOWN = 1006,          //!< Page down arrow key.
        //KEY_BEGIN = 1007,          //!< Begin key.
        //KEY_END = 1008,            //!< End key.
        //INSERT = 1009,             //!< Insert key.

        //COPY,               //!< Copy.
        //CUT,                //!< Cut.
        //PASTE,              //!< Paste.

        //UNDO,               //!< Undo.
        //REDO,               //!< Redo.
    } KEY;


    //! @brief Generic input/output device.
    //! @see endl
    class IODevice : public OutputDevice
    {
    private:
        //! @brief No default constructor.
        IODevice(void);
        //! @brief No copy constructor.
        IODevice(const IODevice&);

    public:
        //! @brief Constructor.
        IODevice(
            const std::string& STR_DbgName, //!< Debug name.
            const std::string& STR_Endl,    //!< Carriage return pattern.
            const bool B_AutoDelete         //!< Auto-deletion flag.
            );

        //! @brief Destructor.
        virtual ~IODevice(void);

    public:
        //! @brief Input key capture handler.
        virtual const KEY GetKey(void) const = 0;

    public:
        //! @brief Null device.
        static IODevice& GetNullDevice(void);

        //! @brief Standard input device.
        static IODevice& GetStdIn(void);

    protected:
        //! @brief Common char translation.
        const KEY GetKey(const int I_Char) const;
    };

};

#endif // _CLI_IO_DEVICE_H_

