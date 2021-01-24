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

#include <cli/menu.h>

#include "cli_Menu.h"

#include "NativeMenu.h"
#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_Menu__1_1Menu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_Name, jint I_NativeHelpRef)
{
    NativeTraces::TraceMethod("Menu.__Menu(PJ_Name, I_NativeHelpRef)");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    cli::Menu* pcli_Menu = NULL;
    if (const cli::Help* const pcli_Help = (const cli::Help*) I_NativeHelpRef)
    {
        if (const char* const str_Name = PJ_Env->GetStringUTFChars(PJ_Name, 0))
        {
            NativeTraces::TraceParam("PJ_Name", "%s", str_Name);
            if ((pcli_Menu = new NativeMenu<cli::Menu>(PJ_Env, "cli/Menu", str_Name, *pcli_Help)))
            {
                NativeObject::Use(pcli_Menu);
            }
            PJ_Env->ReleaseStringUTFChars(PJ_Name, str_Name);
        }
    }
    NativeTraces::TraceReturn("Menu.__Menu()", "%d", (int) pcli_Menu);
    return (jint) pcli_Menu;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Menu__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeMenuRef)
{
    NativeTraces::TraceMethod("Menu.__finalize(PJ_Name, I_NativeMenuRef)");
    NativeTraces::TraceParam("I_NativeMenuRef", "%d", I_NativeMenuRef);
    if (const cli::Menu* const pcli_Menu = (const cli::Menu*) I_NativeMenuRef)
    {
        NativeObject::Free(pcli_Menu);
    }
    NativeTraces::TraceReturn("Menu.__finalize()");
}

