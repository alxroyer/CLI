/*
    Copyright (c) 2006-2008, Alexis Royer

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

#ifndef _CLI_NATIVE_MENU_H_
#define _CLI_NATIVE_MENU_H_


#include <string>
#include <jni.h>

#include "cli/help.h"
#include "cli/command_line.h"


const bool __NativeMenu__Execute(
    JNIEnv* const PJ_Env, const std::string& STR_Class,
    const cli::Menu& CLI_Menu, const cli::CommandLine& CLI_CmdLine);

const bool __NativeMenu__OnExit(
    JNIEnv* const PJ_Env, const std::string& STR_Class,
    const cli::Menu& CLI_Menu);


template <class TMenu> class NativeMenu : public TMenu
{
public:
    NativeMenu(
            JNIEnv* const PJ_Env, const char* const STR_Class,
            const char* const STR_Name, const cli::Help& CLI_Help)
      : TMenu(STR_Name, CLI_Help),
        m_pjEnv(PJ_Env), m_strClass(STR_Class)
    {
    }

    virtual ~NativeMenu(void)
    {
    }

public:
    virtual const bool Execute(const cli::CommandLine& CLI_CmdLine) const
    {
        return __NativeMenu__Execute(m_pjEnv, m_strClass, *this, CLI_CmdLine);
    }

    virtual void OnExit(void) const
    {
        __NativeMenu__OnExit(m_pjEnv, m_strClass, *this);
    }

private:
    JNIEnv* const m_pjEnv;
    const std::string m_strClass;
};

#endif // _CLI_NATIVE_MENU_H_
