/*
    Copyright (c) 2006-2009, Alexis Royer, http://alexis.royer.free.fr/CLI

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

#include "cli/param_float.h"

#include "cli_ParamFloat.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_ParamFloat__1_1ParamFloat(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeHelpRef)
{
    NativeTraces::TraceMethod("ParamFloat.__ParamFloat(I_NativeHelpRef)");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    cli::ParamFloat* pcli_Param = NULL;
    if (const cli::Help* const pcli_Help = (const cli::Help*) I_NativeHelpRef)
    {
        if ((pcli_Param = new cli::ParamFloat(*pcli_Help)))
        {
            NativeObject::Use(pcli_Param);
        }
    }
    NativeTraces::TraceReturn("ParamFloat.__ParamFloat()", "%d", (int) pcli_Param);
    return (jint) pcli_Param;
}

extern "C" JNIEXPORT void JNICALL Java_cli_ParamFloat__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeParamRef)
{
    NativeTraces::TraceMethod("ParamFloat.__finalize(I_NativeParamRef)");
    NativeTraces::TraceParam("I_NativeParamRef", "%d", I_NativeParamRef);
    if (const cli::ParamFloat* const pcli_Param = (const cli::ParamFloat*) I_NativeParamRef)
    {
        NativeObject::Free(pcli_Param);
    }
    NativeTraces::TraceReturn("ParamFloat.__finalize()");
}

extern "C" JNIEXPORT jdouble JNICALL Java_cli_ParamFloat__1_1getValue(JNIEnv* PJ_Env, jclass PJ_Class, jint I_NativeParamRef)
{
    NativeTraces::TraceMethod("ParamFloat.__getValue(I_NativeParamRef)");
    NativeTraces::TraceParam("I_NativeParamRef", "%d", I_NativeParamRef);
    jdouble d_Value = 0;
    if (const cli::ParamFloat* const pcli_Param = (const cli::ParamFloat*) I_NativeParamRef)
    {
        d_Value = (double) (*pcli_Param);
    }
    NativeTraces::TraceReturn("ParamFloat.__getValue()", "%f", d_Value);
    return d_Value;
}
