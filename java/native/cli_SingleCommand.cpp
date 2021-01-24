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

#include "cli/single_command.h"

#include "cli_SingleCommand.h"

#include "NativeObject.h"
#include "NativeTraces.h"



extern "C" JNIEXPORT jint JNICALL Java_cli_SingleCommand__1_1SingleCommand(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_CommandLine, jint I_NativeOutputDeviceRef)
{
    NativeTraces::TraceMethod("SingleCommand.__SingleCommand(PJ_CommandLine, I_NativeOutputDeviceRef)");
    NativeTraces::TraceParam("I_NativeOutputDeviceRef", "%d", I_NativeOutputDeviceRef);
    cli::SingleCommand* pcli_Device = NULL;
    if (const char* const str_CommandLine = PJ_Env->GetStringUTFChars(PJ_CommandLine, 0))
    {
        NativeTraces::TraceParam("PJ_CommandLine", "%s", str_CommandLine);
        if (cli::OutputDevice* const pcli_OutputDevice = (cli::OutputDevice*) I_NativeOutputDeviceRef)
        {
            if ((pcli_Device = new cli::SingleCommand(str_CommandLine, *pcli_OutputDevice, true)))
            {
                NativeObject::Use(pcli_Device);
            }
        }
        PJ_Env->ReleaseStringUTFChars(PJ_CommandLine, str_CommandLine);
    }
    NativeTraces::TraceReturn("SingleCommand.__SingleCommand()", "%d", (int) pcli_Device);
    return (jint) pcli_Device;
}

extern "C" JNIEXPORT void JNICALL Java_cli_SingleCommand__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeDeviceRef)
{
    NativeTraces::TraceMethod("SingleCommand.__finalize(I_NativeDeviceRef)");
    NativeTraces::TraceParam("I_NativeDeviceRef", "%d", I_NativeDeviceRef);
    if (const cli::SingleCommand* const pcli_Device = (const cli::SingleCommand*) I_NativeDeviceRef)
    {
        NativeObject::Free(pcli_Device);
    }
    NativeTraces::TraceReturn("SingleCommand.__finalize()");
}
