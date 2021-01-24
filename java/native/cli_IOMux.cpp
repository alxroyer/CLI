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

#include "cli/io_mux.h"

#include "cli_IOMux.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_IOMux__1_1IOMux(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeTraces::TraceMethod("IOMux.__IOMux()");
    cli::IOMux* pcli_IOMux = NULL;
    if ((pcli_IOMux = new cli::IOMux(true)))
    {
        NativeObject::Use(pcli_IOMux);
    }
    NativeTraces::TraceReturn("IOMux.__IOMux()", "%d", (int) pcli_IOMux);
    return (jint) pcli_IOMux;
}

extern "C" JNIEXPORT void JNICALL Java_cli_IOMux__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeIOMuxRef)
{
    NativeTraces::TraceMethod("IOMux.__finalize(I_NativeIOMuxRef)");
    NativeTraces::TraceParam("I_NativeIOMuxRef", "%d", I_NativeIOMuxRef);
    if (const cli::IOMux* const pcli_IOMux = (const cli::IOMux*) I_NativeIOMuxRef)
    {
        NativeObject::Free(pcli_IOMux);
    }
    NativeTraces::TraceReturn("IOMux.__finalize()");
}

JNIEXPORT jboolean JNICALL Java_cli_IOMux__1_1addInput(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeIOMuxRef, jint I_NativeInputRef)
{
    NativeTraces::TraceMethod("IOMux.__addInput(I_NativeIOMuxRef, I_NativeInputRef)");
    NativeTraces::TraceParam("I_NativeIOMuxRef", "%d", I_NativeIOMuxRef);
    NativeTraces::TraceParam("I_NativeInputRef", "%d", I_NativeInputRef);
    jboolean b_Res = false;
    if (cli::IOMux* const pcli_IOMux = (cli::IOMux*) I_NativeIOMuxRef)
    {
        if (cli::IODevice* const pcli_Input = (cli::IODevice*) I_NativeInputRef)
        {
            b_Res = pcli_IOMux->AddInput(pcli_Input);
        }
    }
    NativeTraces::TraceReturn("IOMux.__addInput()", "%d", (int) b_Res);
    return b_Res;
}

JNIEXPORT jboolean JNICALL Java_cli_IOMux__1_1setOutput(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeIOMuxRef, jint E_StreamType, jint I_NativeStreamRef)
{
    NativeTraces::TraceMethod("IOMux.__setOutput(I_NativeIOMuxRef, I_NativeInputRef)");
    NativeTraces::TraceParam("I_NativeIOMuxRef", "%d", I_NativeIOMuxRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    NativeTraces::TraceParam("I_NativeStreamRef", "%d", I_NativeStreamRef);
    jboolean b_Res = false;
    if (cli::IOMux* const pcli_IOMux = (cli::IOMux*) I_NativeIOMuxRef)
    {
        if (((E_StreamType >= 0) && (E_StreamType < cli::STREAM_TYPES_COUNT))
            || (E_StreamType == cli::ALL_STREAMS))
        {
            if (cli::OutputDevice* const pcli_Stream = (cli::OutputDevice*) I_NativeStreamRef)
            {
                b_Res = pcli_IOMux->SetOutput((cli::STREAM_TYPE) E_StreamType, pcli_Stream);
            }
        }
    }
    NativeTraces::TraceReturn("IOMux.__setOutput()", "%d", (int) b_Res);
    return b_Res;
}
