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

#include "cli/help.h"

#include "cli_Help.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jlong JNICALL Java_cli_Help__1_1Help__(JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Help.__Help()") << cli::endl;
    NativeObject::REF i64_HelpRef = 0;
    if (cli::Help* const pcli_Help = new cli::Help())
    {
        NativeObject::Use(*pcli_Help);
        i64_HelpRef = NativeObject::GetNativeRef(*pcli_Help);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("Help.__Help()", i64_HelpRef) << cli::endl;
    return i64_HelpRef;
}

extern "C" JNIEXPORT jlong JNICALL Java_cli_Help__1_1Help__I(JNIEnv* PJ_Env, jclass PJ_Class, jlong I64_NativeHelpRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Help.__Help(I64_NativeHelpRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeHelpRef", I64_NativeHelpRef) << cli::endl;
    NativeObject::REF i64_HelpRef = 0;
    if (const cli::Help* const pcli_Src = NativeObject::GetNativeObject<const cli::Help*>(I64_NativeHelpRef))
    {
        if (cli::Help* const pcli_Help = new cli::Help(*pcli_Src))
        {
            NativeObject::Use(*pcli_Help);
            i64_HelpRef = NativeObject::GetNativeRef(*pcli_Help);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("Help.__Help()", i64_HelpRef) << cli::endl;
    return i64_HelpRef;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Help__1_1addHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeHelpRef, jint E_Lang, jstring PJ_Help)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Help.__addHelp(I64_NativeHelpRef, E_Lang, PJ_Help)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeHelpRef", I64_NativeHelpRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_Lang", E_Lang) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamStr("PJ_Help", NativeExec::Java2Native(PJ_Help).c_str()) << cli::endl;
    bool b_Res = false;
    if (cli::Help* const pcli_Help = NativeObject::GetNativeObject<cli::Help*>(I64_NativeHelpRef))
    {
        pcli_Help->AddHelp((cli::Help::LANG) E_Lang, NativeExec::Java2Native(PJ_Help).c_str());
        b_Res = true;
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("Help.__addHelp()", b_Res) << cli::endl;
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Help__1_1hasHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeHelpRef, jint E_Lang)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Help.__hasHelp(I64_NativeHelpRef, E_Lang)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeHelpRef", I64_NativeHelpRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_Lang", E_Lang) << cli::endl;
    bool b_Res = false;
    if (const cli::Help* const pcli_Help = NativeObject::GetNativeObject<const cli::Help*>(I64_NativeHelpRef))
    {
        b_Res = pcli_Help->HasString((cli::Help::LANG) E_Lang);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndBool("Help.__hasHelp()", b_Res) << cli::endl;
    return b_Res;
}

extern "C" JNIEXPORT jstring JNICALL Java_cli_Help__1_1getHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeHelpRef, jint E_Lang)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Help.__getHelp(I64_NativeHelpRef, E_Lang)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeHelpRef", I64_NativeHelpRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("E_Lang", E_Lang) << cli::endl;
    std::string str_Help;
    if (const cli::Help* const pcli_Help = NativeObject::GetNativeObject<const cli::Help*>(I64_NativeHelpRef))
    {
        str_Help = (const char*) pcli_Help->GetString((cli::Help::LANG) E_Lang);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndStr("Help.__getHelp()", str_Help.c_str()) << cli::endl;
    return NativeExec::Native2Java(str_Help);
}
