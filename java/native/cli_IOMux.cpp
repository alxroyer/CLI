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

#include "cli/io_mux.h"

#include "cli_IOMux.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jlong JNICALL Java_cli_IOMux__1_1IOMux(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("IOMux.__IOMux()") << cli::endl;
    NativeObject::REF i64_IOMuxRef = 0;
    if (cli::IOMux* const pcli_IOMux = new cli::IOMux(true))
    {
        NativeObject::Use(*pcli_IOMux);
        i64_IOMuxRef = NativeObject::GetNativeRef(*pcli_IOMux);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("IOMux.__IOMux()", i64_IOMuxRef) << cli::endl;
    return i64_IOMuxRef;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_IOMux__1_1addDevice(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeIOMuxRef, jlong I64_NativeDeviceRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    bool b_Res = false;
    if (cli::IOMux* const pcli_IOMux = NativeObject::GetNativeObject<cli::IOMux*>(I64_NativeIOMuxRef))
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::Begin("IOMux.__addDevice(I64_NativeIOMuxRef, I64_NativeInputRef)") << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::ParamInt64("I64_NativeIOMuxRef", I64_NativeIOMuxRef) << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::ParamInt64("I64_NativeDeviceRef", I64_NativeDeviceRef) << cli::endl;

        if (cli::IODevice* const pcli_Device = NativeObject::GetNativeObject<cli::IODevice*>(I64_NativeDeviceRef))
        {
            b_Res = pcli_IOMux->AddDevice(pcli_Device);
        }

        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::EndBool("IOMux.__addDevice()", b_Res) << cli::endl;
    }
    return b_Res;
}

extern "C" JNIEXPORT jlong JNICALL Java_cli_IOMux__1_1getCurrentDevice(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeIOMuxRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    NativeObject::REF i64_CurrentDeviceRef = 0;
    if (const cli::IOMux* const pcli_IOMux = NativeObject::GetNativeObject<const cli::IOMux*>(I64_NativeIOMuxRef))
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::Begin("IOMux.__getCurrentDevice(I64_NativeIOMuxRef)") << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::ParamInt64("I64_NativeIOMuxRef", I64_NativeIOMuxRef) << cli::endl;

        if (const cli::IODevice* const pcli_CurrentDevice = pcli_IOMux->GetCurrentDevice())
        {
            i64_CurrentDeviceRef = NativeObject::GetNativeRef(*pcli_CurrentDevice);
        }

        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::EndInt64("IOMux.__getCurrentDevice()", i64_CurrentDeviceRef) << cli::endl;
    }
    return i64_CurrentDeviceRef;
}

extern "C" JNIEXPORT jlong JNICALL Java_cli_IOMux__1_1switchNextDevice(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeIOMuxRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    NativeObject::REF i64_NextDeviceRef = 0;
    if (cli::IOMux* const pcli_IOMux = NativeObject::GetNativeObject<cli::IOMux*>(I64_NativeIOMuxRef))
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::Begin("IOMux.__switchNextDevice(I64_NativeIOMuxRef)") << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::ParamInt64("I64_NativeIOMuxRef", I64_NativeIOMuxRef) << cli::endl;

        if (const cli::IODevice* const pcli_NextDevice = pcli_IOMux->SwitchNextDevice())
        {
            i64_NextDeviceRef = NativeObject::GetNativeRef(*pcli_NextDevice);
        }

        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::EndInt64("IOMux.__switchNextDevice()", i64_NextDeviceRef) << cli::endl;
    }
    return i64_NextDeviceRef;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_IOMux__1_1resetDeviceList(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeIOMuxRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    jboolean b_Res = false;
    if (cli::IOMux* const pcli_IOMux = NativeObject::GetNativeObject<cli::IOMux*>(I64_NativeIOMuxRef))
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::Begin("IOMux.__resetDeviceList(I64_NativeIOMuxRef)") << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::ParamInt64("I64_NativeIOMuxRef", I64_NativeIOMuxRef) << cli::endl;

        b_Res = pcli_IOMux->ResetDeviceList();

        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IOMux) << NativeTraces::EndBool("IOMux.__resetDeviceList()", b_Res) << cli::endl;
    }
    return b_Res;
}
