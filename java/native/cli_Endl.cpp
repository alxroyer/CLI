/*
    Copyright (c) 2006-2018, Alexis Royer, http://alexis.royer.free.fr/CLI

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
          and/or other materials provided with the distribution.
        * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
          without specific prior written permission.

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


#include "cli/pch.h"

#include "cli/endl.h"
#include "cli/menu.h"

#include "cli_Endl.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jlong JNICALL Java_cli_Endl__1_1Endl(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeHelpRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Endl.__Endl(I64_NativeHelpRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeHelpRef", I64_NativeHelpRef) << cli::endl;
    NativeObject::REF i64_EndlRef = 0;
    if (const cli::Help* const pcli_Help = NativeObject::GetNativeObject<const cli::Help*>(I64_NativeHelpRef))
    {
        if (cli::Endl* const pcli_Endl = new cli::Endl(*pcli_Help))
        {
            NativeObject::Use(*pcli_Endl);
            i64_EndlRef = NativeObject::GetNativeRef(*pcli_Endl);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("Endl.__Endl()", i64_EndlRef) << cli::endl;
    return i64_EndlRef;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Endl__1_1setMenuRef(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeEndlRef, jlong I64_NativeMenuRefRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Endl.__setMenuRef(I64_NativeEndlRef, I64_NativeMenuRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeEndlRef", I64_NativeEndlRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeMenuRefRef", I64_NativeMenuRefRef) << cli::endl;
    bool b_Res = false;
    if (cli::Endl* const pcli_Endl = NativeObject::GetNativeObject<cli::Endl*>(I64_NativeEndlRef))
    {
        if (cli::MenuRef* const pcli_MenuRef = NativeObject::GetNativeObject<cli::MenuRef*>(I64_NativeMenuRefRef))
        {
            pcli_Endl->SetMenuRef(pcli_MenuRef);
            // Delegate the MenuRef instance deletion to the Endl instance set with.
            NativeObject::Delegate(*pcli_MenuRef, *pcli_Endl);
            b_Res = true;
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("Endl::__setMenuRef()", b_Res) << cli::endl;
    return b_Res;
}
