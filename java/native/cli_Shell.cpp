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

#include <cli/shell.h>
#include <cli/cli.h>
#include <cli/io_device.h>

#include "cli_Shell.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1Shell(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeCliRef)
{
    NativeTraces::TraceMethod("Shell.__Shell(I_NativeCliRef)");
    NativeTraces::TraceParam("I_NativeCliRef", "%d", I_NativeCliRef);
    cli::Shell* pcli_Shell = NULL;
    if (const cli::Cli* const pcli_Cli = (const cli::Cli*) I_NativeCliRef)
    {
        if ((pcli_Shell = new cli::Shell(*pcli_Cli)))
        {
            NativeObject::Use(pcli_Cli);
            NativeObject::Use(pcli_Shell);
        }
    }
    NativeTraces::TraceReturn("Shell.__Shell()", "%d", (int) pcli_Shell);
    return (jint) pcli_Shell;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        const cli::Cli* const pcli_Cli = & pcli_Shell->GetCli();
        NativeObject::Free(pcli_Shell);
        NativeObject::Free(pcli_Cli);
    }
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_StreamType)
{
    NativeTraces::TraceMethod("Shell.__getStream(I_NativeShellRef, E_StreamType)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    const cli::OutputDevice* pcli_Stream = NULL;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        pcli_Stream = & pcli_Shell->GetStream((cli::STREAM_TYPE) E_StreamType);
    }
    NativeTraces::TraceReturn("Shell.__getStream()", "%d", (int) pcli_Stream);
    return (jint) pcli_Stream;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1setStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_StreamType, jint I_NativeDeviceRef)
{
    NativeTraces::TraceMethod("Shell.__setStream(I_NativeShellRef, E_StreamType, I_NativeDeviceRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    NativeTraces::TraceParam("I_NativeDeviceRef", "%d", I_NativeDeviceRef);
    bool b_Res = false;
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        if (cli::OutputDevice* const pcli_Device = (cli::OutputDevice*) I_NativeDeviceRef)
        {
            b_Res = pcli_Shell->SetStream((cli::STREAM_TYPE) E_StreamType, *pcli_Device);
        }
    }
    NativeTraces::TraceReturn("Shell.__setStream()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1streamEnabled(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_StreamType)
{
    NativeTraces::TraceMethod("Shell.__streamEnabled(I_NativeShellRef, E_StreamType)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    bool b_Res = false;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        b_Res = pcli_Shell->StreamEnabled((cli::STREAM_TYPE) E_StreamType);
    }
    NativeTraces::TraceReturn("Shell.__streamEnabled()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1enableStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_StreamType, jboolean B_Enable)
{
    NativeTraces::TraceMethod("Shell.__enableStream(I_NativeShellRef, E_StreamType, B_Enable)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    NativeTraces::TraceParam("B_Enable", "%d", (int) B_Enable);
    bool b_Res = false;
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        b_Res = pcli_Shell->EnableStream((cli::STREAM_TYPE) E_StreamType, B_Enable);
    }
    NativeTraces::TraceReturn("Shell.__enableStream()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1run(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint I_NativeIODeviceRef)
{
    NativeTraces::TraceMethod("Shell.__run(I_NativeShellRef, I_NativeIODeviceRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("I_NativeIODeviceRef", "%d", I_NativeIODeviceRef);
    bool b_Res = false;
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        if (cli::IODevice* const pcli_IODevice = (cli::IODevice*) I_NativeIODeviceRef)
        {
            NativeObject::Use(pcli_IODevice);
            pcli_Shell->Run(*pcli_IODevice);
            b_Res = true;
            NativeObject::Free(pcli_IODevice);
        }
    }
    NativeTraces::TraceReturn("Shell.__run()", "%d", (int) b_Res);
    return b_Res;
}
