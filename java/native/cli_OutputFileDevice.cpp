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

#include "cli/file_device.h"

#include "cli_OutputFileDevice.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_OutputFileDevice__1_1OutputFileDevice(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_OutputFileName)
{
    NativeTraces::TraceMethod("OutputFileDevice.__OutputFileDevice(PJ_OutputFileName)");
    cli::OutputFileDevice* pcli_FileDevice = NULL;
    if (const char* const str_OutputFileName = PJ_Env->GetStringUTFChars(PJ_OutputFileName, 0))
    {
        NativeTraces::TraceParam("PJ_OutputFileName", "%s", str_OutputFileName);
        if ((pcli_FileDevice = new cli::OutputFileDevice(str_OutputFileName, true)))
        {
            NativeObject::Use(pcli_FileDevice);
        }
        PJ_Env->ReleaseStringUTFChars(PJ_OutputFileName, str_OutputFileName);
    }
    NativeTraces::TraceReturn("OutputFileDevice.__OutputFileDevice()", "%d", (int) pcli_FileDevice);
    return (jint) pcli_FileDevice;
}

extern "C" JNIEXPORT void JNICALL Java_cli_OutputFileDevice__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeFileDeviceRef)
{
    NativeTraces::TraceMethod("OutputFileDevice.__finalize(I_NativeFileDeviceRef)");
    NativeTraces::TraceParam("I_NativeFileDeviceRef", "%d", I_NativeFileDeviceRef);
    if (const cli::OutputFileDevice* const pcli_FileDevice = (const cli::OutputFileDevice*) I_NativeFileDeviceRef)
    {
        NativeObject::Free(pcli_FileDevice);
    }
    NativeTraces::TraceReturn("OutputFileDevice.__finalize()");
}

