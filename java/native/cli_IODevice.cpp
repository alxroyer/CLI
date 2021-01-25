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

#include "cli/io_device.h"

#include "cli_IODevice.h"

#include "NativeDevice.h"
#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


// IODevice.Native implementation.

extern "C" JNIEXPORT jint JNICALL Java_cli_IODevice__1_1Native_1_1getKey(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeDeviceRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::KEY e_Key = cli::NULL_KEY;
    if (const cli::IODevice* const pcli_IODevice = NativeObject::GetNativeObject<const cli::IODevice*>(I64_NativeDeviceRef))
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IODevice) << NativeTraces::Begin("IODevice.Native.__getKey(I64_NativeDeviceRef)") << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IODevice) << NativeTraces::ParamInt64("I64_NativeDeviceRef", I64_NativeDeviceRef) << cli::endl;

        e_Key = pcli_IODevice->GetKey();

        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IODevice) << NativeTraces::EndInt("IODevice.Native.__getKey()", e_Key) << cli::endl;
    }
    return e_Key;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_IODevice__1_1Native_1_1getLocation(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeDeviceRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    NativeObject::REF i64_LocationRef = 0;
    if (const cli::IODevice* const pcli_IODevice = NativeObject::GetNativeObject<const cli::IODevice*>(I64_NativeDeviceRef))
    {
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IODevice) << NativeTraces::Begin("IODevice.Native.__getLocation(I64_NativeDeviceRef)") << cli::endl;
        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IODevice) << NativeTraces::ParamInt64("I64_NativeDeviceRef", I64_NativeDeviceRef) << cli::endl;

        if (const cli::ResourceString* const pcli_Location = new cli::ResourceString(pcli_IODevice->GetLocation()))
        {
            NativeObject::CreateFromNative(*pcli_Location);
            i64_LocationRef = NativeObject::GetNativeRef(*pcli_Location);
        }

        cli::GetTraces().SafeTrace(TRACE_JNI, *pcli_IODevice) << NativeTraces::EndInt64("IODevice.Native.__getLocation()", i64_LocationRef) << cli::endl;
    }
    return i64_LocationRef;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_IODevice__1_1Native_1_1wouldInput(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeIODeviceRef, jlong I64_NativeIODevice2Ref)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    // Do not trace! for consistency reasons.
    bool b_Res = false;
    if (const cli::IODevice* const pcli_IODevice1 = NativeObject::GetNativeObject<const cli::IODevice*>(I64_NativeIODeviceRef))
    {
        if (const cli::IODevice* const pcli_IODevice2 = NativeObject::GetNativeObject<const cli::IODevice*>(I64_NativeIODevice2Ref))
        {
            b_Res = pcli_IODevice1->WouldInput(*pcli_IODevice2);
        }
    }
    return b_Res;
}


// IODevice.Java implementation.

extern "C" JNIEXPORT jlong JNICALL Java_cli_IODevice__1_1Java_1_1Java(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_DbgName)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("IODevice.Java.__Java(PJ_DbgName)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamStr("PJ_DbgName", NativeExec::Java2Native(PJ_DbgName).c_str()) << cli::endl;

    NativeObject::REF i64_IODeviceRef = 0;
    if (cli::IODevice* const pcli_IODevice = new NativeDevice<cli::IODevice>(NativeExec::Java2Native(PJ_DbgName).c_str()))
    {
        NativeObject::Use(*pcli_IODevice);
        i64_IODeviceRef = NativeObject::GetNativeRef(*pcli_IODevice);
    }

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("IODevice.Java.__Java()", i64_IODeviceRef) << cli::endl;
    return i64_IODeviceRef;
}


// IODevice static methods implementation.

extern "C" JNIEXPORT jlong JNICALL Java_cli_IODevice__1_1getNullDevice(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("IODevice.__getNullDevice()") << cli::endl;

    static NativeObject::REF i64_NullDeviceRef = 0;
    if (i64_NullDeviceRef == 0)
    {
        const cli::IODevice& cli_NullDevice = cli::IODevice::GetNullDevice();
        i64_NullDeviceRef = NativeObject::GetNativeRef(cli_NullDevice);

        // Note: Java should call this JNI interface once only for IODevice.m_cliNullDevice static member initialization.
        // Ensure one more token there so that System.runFinalizersOnExit(true) does not cause a crash on the Java OutputDevice object finalization.
        const_cast<cli::IODevice&>(cli_NullDevice).UseInstance(__CALL_INFO__);
    }

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("IODevice.__getNullDevice()", i64_NullDeviceRef) << cli::endl;
    return i64_NullDeviceRef;
}

extern "C" JNIEXPORT jlong JNICALL Java_cli_IODevice__1_1getStdIn(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("IODevice.__getStdIn()") << cli::endl;

    static NativeObject::REF i64_StdInDeviceRef = 0;
    if (i64_StdInDeviceRef == 0)
    {
        const cli::IODevice& cli_StdInDevice = cli::IODevice::GetStdIn();
        i64_StdInDeviceRef = NativeObject::GetNativeRef(cli_StdInDevice);

        // Note: Java should call this JNI interface once only for IODevice.m_cliStdIn static member initialization.
        // Ensure one more token there so that System.runFinalizersOnExit(true) does not cause a crash on the Java OutputDevice object finalization.
        const_cast<cli::IODevice&>(cli_StdInDevice).UseInstance(__CALL_INFO__);
    }

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("IODevice.__getStdIn()", i64_StdInDeviceRef) << cli::endl;
    return i64_StdInDeviceRef;
}
