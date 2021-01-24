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

#include "cli/help.h"

#include "cli_Help.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_Help__1_1Help__(JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeTraces::TraceMethod("Help.__Help()");
    cli::Help* pcli_Help = NULL;
    if ((pcli_Help = new cli::Help()))
    {
        NativeObject::Use(pcli_Help);
    }
    NativeTraces::TraceReturn("Help.__Help()", "%d", (int) pcli_Help);
    return (jint) pcli_Help;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Help__1_1Help__I(JNIEnv* PJ_Env, jclass PJ_Class, jint I_NativeHelpRef)
{
    NativeTraces::TraceMethod("Help.__Help(I_NativeHelpRef)");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    cli::Help* pcli_Help = NULL;
    if (const cli::Help* const pcli_Src = (const cli::Help*) I_NativeHelpRef)
    {
        if ((pcli_Help = new cli::Help(*pcli_Src)))
        {
            NativeObject::Use(pcli_Help);
        }
    }
    NativeTraces::TraceReturn("Help.__Help()", "%d", (int) pcli_Help);
    return (jint) pcli_Help;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Help__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeHelpRef)
{
    NativeTraces::TraceMethod("Help.__finalize(I_NativeHelpRef)");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    if (const cli::Help* const pcli_Help = (const cli::Help*) I_NativeHelpRef)
    {
        NativeObject::Free(pcli_Help);
    }
    NativeTraces::TraceReturn("Help.__finalize()");
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Help__1_1addHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeHelpRef, jint E_Lang, jstring PJ_Help)
{
    NativeTraces::TraceMethod("Help.__addHelp(I_NativeHelpRef, E_Lang, PJ_Help)");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    NativeTraces::TraceParam("E_Lang", "%d", E_Lang);
    bool b_Res = false;
    if (cli::Help* const pcli_Help = (cli::Help*) I_NativeHelpRef)
    {
        if (PJ_Env != NULL)
        {
            if (const char* const str_Help = PJ_Env->GetStringUTFChars(PJ_Help, 0))
            {
                NativeTraces::TraceParam("PJ_Help", "%s", str_Help);
                pcli_Help->AddHelp((cli::Help::LANG) E_Lang, str_Help);
                b_Res = true;
                PJ_Env->ReleaseStringUTFChars(PJ_Help, str_Help);
            }
        }
    }
    NativeTraces::TraceReturn("Help.__addHelp()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Help__1_1hasHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeHelpRef, jint E_Lang)
{
    NativeTraces::TraceMethod("Help.__hasHelp()");
    jboolean b_Res = false;
    if (const cli::Help* const pcli_Help = (cli::Help*) I_NativeHelpRef)
    {
        b_Res = pcli_Help->HasString((cli::Help::LANG) E_Lang);
    }
    NativeTraces::TraceReturn("Help.__hasHelp()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jstring JNICALL Java_cli_Help__1_1getHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeHelpRef, jint E_Lang)
{
    NativeTraces::TraceMethod("Help.__getHelp()");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    NativeTraces::TraceParam("E_Lang", "%d", E_Lang);
    jstring j_Help = NULL;
    if (const cli::Help* const pcli_Help = (cli::Help*) I_NativeHelpRef)
    {
        if (PJ_Env != NULL)
        {
            const std::string str_Help = (const char*) pcli_Help->GetString((cli::Help::LANG) E_Lang);
            NativeTraces::TraceReturn("Help.__getHelp()", "%s", str_Help.c_str());
            j_Help = PJ_Env->NewStringUTF(str_Help.c_str());
        }
    }
    return j_Help;
}
