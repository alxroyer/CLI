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

#include <cli/endl.h>
#include <cli/menu.h>

#include "cli_Endl.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_Endl__1_1Endl(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeHelpRef)
{
    NativeTraces::TraceMethod("Endl.__Endl(I_NativeHelpRef)");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    cli::Endl* pcli_Endl = NULL;
    if (const cli::Help* const pcli_Help = (const cli::Help*) I_NativeHelpRef)
    {
        if ((pcli_Endl = new cli::Endl(*pcli_Help)))
        {
            NativeObject::Use(pcli_Endl);
        }
    }
    NativeTraces::TraceReturn("Endl.__Endl()", "%d", (int) pcli_Endl);
    return (jint) pcli_Endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Endl__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeEndlRef)
{
    NativeTraces::TraceMethod("Endl.__finalize(I_NativeEndlRef)");
    NativeTraces::TraceParam("I_NativeEndlRef", "%d", I_NativeEndlRef);
    if (const cli::Endl* const pcli_Endl = (const cli::Endl* const) I_NativeEndlRef)
    {
        NativeObject::Free(pcli_Endl);
    }
    NativeTraces::TraceReturn("Endl.__finalize()");
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Endl__1_1setMenuRef(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeEndlRef, jint I_NativeMenuRefRef)
{
    NativeTraces::TraceMethod("Endl.__setMenuRef(I_NativeEndlRef, I_NativeMenuRef)");
    NativeTraces::TraceParam("I_NativeEndlRef", "%d", I_NativeEndlRef);
    NativeTraces::TraceParam("I_NativeMenuRefRef", "%d", I_NativeMenuRefRef);
    jboolean b_Res = false;
    if (cli::Endl* const pcli_Endl = (cli::Endl*) I_NativeEndlRef)
    {
        if (cli::MenuRef* const pcli_MenuRef = (cli::MenuRef*) I_NativeMenuRefRef)
        {
            pcli_Endl->SetMenuRef(pcli_MenuRef);
        }
    }
    NativeTraces::TraceReturn("Endl::__setMenuRef()", "%d", (int) b_Res);
    return b_Res;
}

