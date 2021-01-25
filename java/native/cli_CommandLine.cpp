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

#include "cli/command_line.h"

#include "cli_CommandLine.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jlong JNICALL Java_cli_CommandLine__1_1CommandLine(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("CommandLine.__CommandLine()") << cli::endl;
    NativeObject::REF i64_CmdLineRef = 0;
    if (cli::CommandLine* const pcli_CmdLine = new cli::CommandLine())
    {
        NativeObject::Use(*pcli_CmdLine);
        i64_CmdLineRef = NativeObject::GetNativeRef(*pcli_CmdLine);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("CommandLine.__CommandLine()", i64_CmdLineRef) << cli::endl;
    return i64_CmdLineRef;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_CommandLine__1_1getElementCount(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeCmdLineRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("CommandLine.__getElementCount(I64_NativeCmdLineRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeCmdLineRef", I64_NativeCmdLineRef) << cli::endl;
    int i_Count = 0;
    if (const cli::CommandLine* const pcli_CmdLine = NativeObject::GetNativeObject<const cli::CommandLine*>(I64_NativeCmdLineRef))
    {
        for (   cli::CommandLineIterator it(*pcli_CmdLine);
                it.StepIt(); )
        {
            i_Count ++;
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("CommandLine.__getElementCount()", i_Count) << cli::endl;
    return i_Count;
}

extern "C" JNIEXPORT jlong JNICALL Java_cli_CommandLine__1_1getElementAt(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeCmdLineRef, jint I_Position)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("CommandLine.__getElementAt(I64_NativeCmdLineRef, I_Position)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeCmdLineRef", I64_NativeCmdLineRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_Position", I_Position) << cli::endl;
    NativeObject::REF i64_ElementRef = 0;
    if (const cli::CommandLine* const pcli_CmdLine = NativeObject::GetNativeObject<const cli::CommandLine*>(I64_NativeCmdLineRef))
    {
        int i_Count = 0;
        for (   cli::CommandLineIterator it(*pcli_CmdLine);
                it.StepIt() && (i64_ElementRef == 0);
                )
        {
            if (i_Count >= I_Position)
            {
                if (const cli::Element* const pcli_Element = *it)
                {
                    i64_ElementRef = NativeObject::GetNativeRef(*pcli_Element);
                }
            }
            i_Count ++;
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("CommandLine.__getElementAt()", i64_ElementRef) << cli::endl;
    return i64_ElementRef;
}
