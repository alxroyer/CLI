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

#include "cli/resource_string.h"

#include "cli_ResourceString.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_ResourceString__1_1ResourceString__(JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeTraces::TraceMethod("ResourceString.__ResourceString()");
    cli::ResourceString* pcli_String = NULL;
    if ((pcli_String = new cli::ResourceString()))
    {
        NativeObject::Use(pcli_String);
    }
    NativeTraces::TraceReturn("ResourceString.__ResourceString()", "%d", (int) pcli_String);
    return (jint) pcli_String;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_ResourceString__1_1ResourceString__I(JNIEnv* PJ_Env, jclass PJ_Class, jint I_NativeStringRef)
{
    NativeTraces::TraceMethod("ResourceString.__ResourceString(I_NativeStringRef)");
    NativeTraces::TraceParam("I_NativeStringRef", "%d", I_NativeStringRef);
    cli::ResourceString* pcli_String = NULL;
    if (const cli::ResourceString* const pcli_Src = (const cli::ResourceString*) I_NativeStringRef)
    {
        if ((pcli_String = new cli::ResourceString(*pcli_Src)))
        {
            NativeObject::Use(pcli_String);
        }
    }
    NativeTraces::TraceReturn("ResourceString.__ResourceString()", "%d", (int) pcli_String);
    return (jint) pcli_String;
}

extern "C" JNIEXPORT void JNICALL Java_cli_ResourceString__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeStringRef)
{
    NativeTraces::TraceMethod("ResourceString.__finalize(I_NativeStringRef)");
    NativeTraces::TraceParam("I_NativeStringRef", "%d", I_NativeStringRef);
    if (const cli::ResourceString* const pcli_String = (const cli::ResourceString*) I_NativeStringRef)
    {
        NativeObject::Free(pcli_String);
    }
    NativeTraces::TraceReturn("ResourceString.__finalize()");
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_ResourceString__1_1addString(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeStringRef, jint E_Lang, jstring PJ_ResourceString)
{
    NativeTraces::TraceMethod("ResourceString.__addString(I_NativeStringRef, E_Lang, PJ_ResourceString)");
    NativeTraces::TraceParam("I_NativeStringRef", "%d", I_NativeStringRef);
    NativeTraces::TraceParam("E_Lang", "%d", E_Lang);
    jboolean b_Res = false;
    if (cli::ResourceString* const pcli_String = (cli::ResourceString*) I_NativeStringRef)
    {
        if (PJ_Env != NULL)
        {
            if (const char* const str_String = PJ_Env->GetStringUTFChars(PJ_ResourceString, 0))
            {
                NativeTraces::TraceParam("PJ_ResourceString", "%s", str_String);
                pcli_String->SetString((cli::ResourceString::LANG) E_Lang, str_String);
                b_Res = true;
                PJ_Env->ReleaseStringUTFChars(PJ_ResourceString, str_String);
            }
        }
    }
    NativeTraces::TraceReturn("ResourceString.__addString()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_ResourceString__1_1hasString(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeStringRef, jint E_Lang)
{
    NativeTraces::TraceMethod("ResourceString.__hasString()");
    jboolean b_Res = false;
    if (const cli::ResourceString* const pcli_String = (cli::ResourceString*) I_NativeStringRef)
    {
        b_Res = pcli_String->HasString((cli::ResourceString::LANG) E_Lang);
    }
    NativeTraces::TraceReturn("ResourceString.__hasString()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jstring JNICALL Java_cli_ResourceString__1_1getString(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeStringRef, jint E_Lang)
{
    NativeTraces::TraceMethod("ResourceString.__getString()");
    NativeTraces::TraceParam("I_NativeStringRef", "%d", I_NativeStringRef);
    NativeTraces::TraceParam("E_Lang", "%d", E_Lang);
    jstring j_ResourceString = NULL;
    if (const cli::ResourceString* const pcli_String = (cli::ResourceString*) I_NativeStringRef)
    {
        if (PJ_Env != NULL)
        {
            const std::string str_String = (const char*) pcli_String->GetString((cli::ResourceString::LANG) E_Lang);
            NativeTraces::TraceReturn("ResourceString.__getString()", "%s", str_String.c_str());
            j_ResourceString = PJ_Env->NewStringUTF(str_String.c_str());
        }
    }
    return j_ResourceString;
}
