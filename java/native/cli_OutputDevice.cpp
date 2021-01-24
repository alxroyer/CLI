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

#include "cli/io_device.h"

#include "cli_OutputDevice.h"

#include "NativeTraces.h"


extern "C" JNIEXPORT void JNICALL Java_cli_OutputDevice__1_1print__ILjava_lang_String_2(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeOutputDeviceRef, jstring PJ_Text)
{
    NativeTraces::TraceMethod("OutputDevice.__print(I_NativeOutputDeviceRef, PJ_Text)");
    NativeTraces::TraceParam("I_NativeOutputDeviceRef", "%d", I_NativeOutputDeviceRef);
    if (const cli::OutputDevice* const pcli_OutputDevice = (const cli::OutputDevice*) I_NativeOutputDeviceRef)
    {
        if (const char* const str_Text = PJ_Env->GetStringUTFChars(PJ_Text, 0))
        {
            NativeTraces::TraceParam("PJ_Text", "%d", str_Text);
            pcli_OutputDevice->operator<<(str_Text);
            PJ_Env->ReleaseStringUTFChars(PJ_Text, str_Text);
        }
    }
    NativeTraces::TraceReturn("OutputDevice.__print()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_OutputDevice__1_1print__II(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeOutputDeviceRef, jint I_Integer)
{
    NativeTraces::TraceMethod("OutputDevice.__print(I_NativeOutputDeviceRef, I_Integer)");
    NativeTraces::TraceParam("I_NativeOutputDeviceRef", "%d", I_NativeOutputDeviceRef);
    NativeTraces::TraceParam("I_Integer", "%d", I_Integer);
    if (const cli::OutputDevice* const pcli_OutputDevice = (const cli::OutputDevice*) I_NativeOutputDeviceRef)
    {
        pcli_OutputDevice->operator<<(I_Integer);
    }
    NativeTraces::TraceReturn("OutputDevice.__print()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_OutputDevice__1_1print__IF(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeOutputDeviceRef, jfloat F_Float)
{
    NativeTraces::TraceMethod("OutputDevice.__print(I_NativeOutputDeviceRef, F_Float)");
    NativeTraces::TraceParam("I_NativeOutputDeviceRef", "%d", I_NativeOutputDeviceRef);
    NativeTraces::TraceParam("F_Float", "%d", F_Float);
    if (const cli::OutputDevice* const pcli_OutputDevice = (const cli::OutputDevice*) I_NativeOutputDeviceRef)
    {
        pcli_OutputDevice->operator<<(F_Float);
    }
    NativeTraces::TraceReturn("OutputDevice.__print()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_OutputDevice__1_1print__ID(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeOutputDeviceRef, jdouble D_Double)
{
    NativeTraces::TraceMethod("OutputDevice.__print(I_NativeOutputDeviceRef, D_Double)");
    NativeTraces::TraceParam("I_NativeOutputDeviceRef", "%d", I_NativeOutputDeviceRef);
    NativeTraces::TraceParam("D_Double", "%d", D_Double);
    if (const cli::OutputDevice* const pcli_OutputDevice = (const cli::OutputDevice*) I_NativeOutputDeviceRef)
    {
        pcli_OutputDevice->operator<<(D_Double);
    }
    NativeTraces::TraceReturn("OutputDevice.__print()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_OutputDevice__1_1endl(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeOutputDeviceRef)
{
    NativeTraces::TraceMethod("OutputDevice.__endl(I_NativeOutputDeviceRef, PJ_Text)");
    NativeTraces::TraceParam("I_NativeOutputDeviceRef", "%d", I_NativeOutputDeviceRef);
    if (const cli::OutputDevice* const pcli_OutputDevice = (const cli::OutputDevice*) I_NativeOutputDeviceRef)
    {
        pcli_OutputDevice->operator<<(cli::endl);
    }
    NativeTraces::TraceReturn("OutputDevice.__endl()");
}

extern "C" JNIEXPORT jint JNICALL Java_cli_OutputDevice__1_1getNullDevice(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeTraces::TraceMethod("OutputDevice.__getNullDevice()");
    const cli::OutputDevice* const pcli_NullDevice = & cli::OutputDevice::GetNullDevice();
    NativeTraces::TraceReturn("OutputDevice.__getNullDevice()", "%d", (int) pcli_NullDevice);
    return (jint) pcli_NullDevice;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_OutputDevice__1_1getStdOut(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeTraces::TraceMethod("OutputDevice.__getStdOut()");
    const cli::OutputDevice* const pcli_StdOut = & cli::OutputDevice::GetStdOut();
    NativeTraces::TraceReturn("OutputDevice.__getStdOut()", "%d", (int) pcli_StdOut);
    return (jint) pcli_StdOut;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_OutputDevice__1_1getStdErr(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeTraces::TraceMethod("OutputDevice.__getStdErr()");
    const cli::OutputDevice* const pcli_StdErr = & cli::OutputDevice::GetStdErr();
    NativeTraces::TraceReturn("OutputDevice.__getStdErr()", "%d", (int) pcli_StdErr);
    return (jint) pcli_StdErr;
}
