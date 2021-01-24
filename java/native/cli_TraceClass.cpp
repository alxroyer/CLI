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

#include "cli/traces.h"
#include "cli/help.h"

#include "cli_TraceClass.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_TraceClass__1_1TraceClass(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_TraceClassName, jint I_NativeHelpRef)
{
    NativeTraces::TraceMethod("TraceClass.__TraceClass(PJ_TraceClassName, I_NativeHelpRef)");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    cli::TraceClass* pcli_TraceClass = NULL;
    if (const char* const str_TraceClassName = PJ_Env->GetStringUTFChars(PJ_TraceClassName, 0))
    {
        NativeTraces::TraceParam("PJ_TraceClassName", "%s", str_TraceClassName);
        if (const cli::Help* const pcli_Help = (const cli::Help*) I_NativeHelpRef)
        {
            if ((pcli_TraceClass = new cli::TraceClass(str_TraceClassName, *pcli_Help)))
            {
                NativeObject::Use(pcli_TraceClass);
            }
        }
        else
        {
            if ((pcli_TraceClass = new cli::TraceClass(str_TraceClassName, cli::Help())))
            {
                NativeObject::Use(pcli_TraceClass);
            }
        }
        PJ_Env->ReleaseStringUTFChars(PJ_TraceClassName, str_TraceClassName);
    }
    NativeTraces::TraceReturn("TraceClass.__TraceClass()", "%d", (int) pcli_TraceClass);
    return (jint) pcli_TraceClass;
}

extern "C" JNIEXPORT void JNICALL Java_cli_TraceClass__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeTraceClassRef)
{
    NativeTraces::TraceMethod("TraceClass.__finalize(I_NativeTraceClassRef)");
    NativeTraces::TraceParam("I_NativeTraceClassRef", "%d", I_NativeTraceClassRef);
    if (const cli::TraceClass* const pcli_TraceClass = (const cli::TraceClass*) I_NativeTraceClassRef)
    {
        NativeObject::Free(pcli_TraceClass);
    }
    NativeTraces::TraceReturn("TraceClass.__finalize()");
}

extern "C" JNIEXPORT jstring JNICALL Java_cli_TraceClass__1_1getClassName(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeTraceClassRef)
{
    NativeTraces::TraceMethod("TraceClass.__getClassName(I_NativeTraceClassRef)");
    NativeTraces::TraceParam("I_NativeTraceClassRef", "%d", I_NativeTraceClassRef);
    jstring pj_ClassName = NULL;
    if (const cli::TraceClass* const pcli_TraceClass = (const cli::TraceClass*) I_NativeTraceClassRef)
    {
        NativeTraces::TraceReturn("TraceClass.__getClassName()", "%s", (const char*) pcli_TraceClass->GetName());
        pj_ClassName = PJ_Env->NewStringUTF(pcli_TraceClass->GetName());
    }
    return pj_ClassName;
}

extern "C" JNIEXPORT jstring JNICALL Java_cli_TraceClass__1_1getHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeTraceClassRef, jint I_Language)
{
    NativeTraces::TraceMethod("TraceClass.__getHelp(I_NativeTraceClassRef, I_Language)");
    NativeTraces::TraceParam("I_NativeTraceClassRef", "%d", I_NativeTraceClassRef);
    NativeTraces::TraceParam("I_Language", "%d", I_Language);
    jstring pj_Help = NULL;
    if ((I_Language >= 0) && (I_Language < cli::Help::LANG_COUNT))
    {
        if (const cli::TraceClass* const pcli_TraceClass = (const cli::TraceClass*) I_NativeTraceClassRef)
        {
            const std::string str_Help = (const char*) pcli_TraceClass->GetHelp().GetString((cli::Help::LANG) I_Language);
            NativeTraces::TraceReturn("TraceClass.__getHelp()", "%s", str_Help.c_str());
            pj_Help = PJ_Env->NewStringUTF(str_Help.c_str());
        }
    }
    return pj_Help;
}

