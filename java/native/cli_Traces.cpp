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

#include "cli/traces.h"
#include "cli/io_device.h"

#include "cli_CommandLine.h"

#include "NativeTraces.h"


extern "C" JNIEXPORT jboolean JNICALL Java_cli_Traces__1_1setStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeOutputDeviceRef)
{
    jboolean b_Res = false;
    if (cli::OutputDevice* const pcli_Stream = (cli::OutputDevice*) I_NativeOutputDeviceRef)
    {
        b_Res = cli::GetTraces().SetStream(*pcli_Stream);
    }
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Traces__1_1unsetStream(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    jboolean b_Res = false;
    b_Res = cli::GetTraces().UnsetStream();
    return b_Res;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Traces__1_1setFilter(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeTraceClassRef, jboolean B_ShowTraces)
{
    if (const cli::TraceClass* const pcli_TraceClass = (const cli::TraceClass*) I_NativeTraceClassRef)
    {
        cli::GetTraces().Declare(*pcli_TraceClass);
        cli::GetTraces().SetFilter(*pcli_TraceClass, B_ShowTraces);
    }
}

extern "C" JNIEXPORT void JNICALL Java_cli_Traces__1_1setAllFilter(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jboolean B_ShowTraces)
{
    cli::GetTraces().SetAllFilter(B_ShowTraces);
}

extern "C" JNIEXPORT void JNICALL Java_cli_Traces__1_1trace(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_TraceClassNativeRef, jstring PJ_Text)
{
    if (const cli::TraceClass* const pcli_TraceClass = (const cli::TraceClass*) I_TraceClassNativeRef)
    {
        if (const char* const str_Text = PJ_Env->GetStringUTFChars(PJ_Text, 0))
        {
            cli::GetTraces().Trace(*pcli_TraceClass) << str_Text << cli::endl;
            PJ_Env->ReleaseStringUTFChars(PJ_Text, str_Text);
        }
    }
}

extern "C" JNIEXPORT void JNICALL Java_cli_Traces__1_1traceMethod(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_Method)
{
    if (const char* const str_Method = PJ_Env->GetStringUTFChars(PJ_Method, 0))
    {
        NativeTraces::TraceMethod(str_Method);
        PJ_Env->ReleaseStringUTFChars(PJ_Method, str_Method);
    }
    else
    {
        NativeTraces::TraceMethod("?");
    }
}

extern "C" JNIEXPORT void JNICALL Java_cli_Traces__1_1traceParam(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_ParamName, jstring PJ_ParamValue)
{
    if (const char* const str_ParamName = PJ_Env->GetStringUTFChars(PJ_ParamName, 0))
    {
        if (const char* const str_ParamValue = PJ_Env->GetStringUTFChars(PJ_ParamValue, 0))
        {
            NativeTraces::TraceParam(str_ParamName, "%s", str_ParamValue);
            PJ_Env->ReleaseStringUTFChars(PJ_ParamValue, str_ParamValue);
        }
        else
        {
            NativeTraces::TraceParam(str_ParamName, "%s", "?");
        }
        PJ_Env->ReleaseStringUTFChars(PJ_ParamName, str_ParamName);
    }
    else
    {
        NativeTraces::TraceParam("?", "%s", "");
    }
}

extern "C" JNIEXPORT void JNICALL Java_cli_Traces__1_1traceValue(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_VarName, jstring PJ_VarValue)
{
    if (const char* const str_VarName = PJ_Env->GetStringUTFChars(PJ_VarName, 0))
    {
        if (const char* const str_VarValue = PJ_Env->GetStringUTFChars(PJ_VarValue, 0))
        {
            NativeTraces::TraceValue(str_VarName, "%s", str_VarValue);
            PJ_Env->ReleaseStringUTFChars(PJ_VarValue, str_VarValue);
        }
        else
        {
            NativeTraces::TraceValue(str_VarName, "%s", "?");
        }
        PJ_Env->ReleaseStringUTFChars(PJ_VarName, str_VarName);
    }
    else
    {
        NativeTraces::TraceValue("?", "%s", "");
    }
}

extern "C" JNIEXPORT void JNICALL Java_cli_Traces__1_1traceReturn__Ljava_lang_String_2(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_Method)
{
    if (const char* const str_Method = PJ_Env->GetStringUTFChars(PJ_Method, 0))
    {
        NativeTraces::TraceReturn(str_Method);
        PJ_Env->ReleaseStringUTFChars(PJ_Method, str_Method);
    }
    else
    {
        NativeTraces::TraceReturn("?");
    }
}

extern "C" JNIEXPORT void JNICALL Java_cli_Traces__1_1traceReturn__Ljava_lang_String_2Ljava_lang_String_2(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_Method, jstring PJ_Result)
{
    if (const char* const str_Method = PJ_Env->GetStringUTFChars(PJ_Method, 0))
    {
        if (const char* const str_Result = PJ_Env->GetStringUTFChars(PJ_Result, 0))
        {
            NativeTraces::TraceReturn(str_Method, "%s", str_Result);
            PJ_Env->ReleaseStringUTFChars(PJ_Result, str_Result);
        }
        else
        {
            NativeTraces::TraceReturn(str_Method, "%s", "?");
        }
        PJ_Env->ReleaseStringUTFChars(PJ_Method, str_Method);
    }
    else
    {
        NativeTraces::TraceReturn("?", "%s", "");
    }
}

