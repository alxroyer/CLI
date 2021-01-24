/*
    Copyright (c) 2006-2009, Alexis Royer

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


#include "cli/pch.h"

#include "cli/param.h"

#include "cli_Param.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jboolean JNICALL Java_cli_Param__1_1copyValue(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeDestParamRef, jint I_NativeSrcParamRef)
{
    NativeTraces::TraceMethod("Param.copyValue(I_NativeDestParamRef, I_NativeSrcParamRef)");
    NativeTraces::TraceParam("I_NativeDestParamRef", "%d", I_NativeDestParamRef);
    NativeTraces::TraceParam("I_NativeSrcParamRef", "%d", I_NativeSrcParamRef);

    bool b_Res = false;
    if (const cli::Param* const pcli_DestParam = (const cli::Param*) I_NativeDestParamRef)
    {
        if (const cli::Param* const pcli_SrcParam = (const cli::Param*) I_NativeSrcParamRef)
        {
            pcli_DestParam->CopyValue(*pcli_SrcParam);
            b_Res = true;
        }
    }

    NativeTraces::TraceReturn("Param.copyValue()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Param__1_1getCloned(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeParamRef)
{
    NativeTraces::TraceMethod("Param.getCloned(I_NativeParamRef)");
    NativeTraces::TraceParam("I_NativeParamRef", "%d", I_NativeParamRef);

    const cli::Param* pcli_Cloned = NULL;
    if (const cli::Param* const pcli_Param = (const cli::Param*) I_NativeParamRef)
    {
        pcli_Cloned = pcli_Param->GetCloned();
    }

    NativeTraces::TraceReturn("Param.getCloned()", "%d", (int) pcli_Cloned);
    return (int) pcli_Cloned;
}
