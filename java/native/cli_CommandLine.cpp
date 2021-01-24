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

#include "cli/command_line.h"

#include "cli_CommandLine.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_CommandLine__1_1CommandLine(
        JNIEnv* PJ_Env, jclass PJ_Class)
{
    NativeTraces::TraceMethod("CommandLine.__CommandLine()");
    cli::CommandLine* pcli_CmdLine = NULL;
    if ((pcli_CmdLine = new cli::CommandLine()))
    {
        NativeObject::Use(pcli_CmdLine);
    }
    NativeTraces::TraceReturn("CommandLine.__CommandLine()", "%d", (int) pcli_CmdLine);
    return (jint) pcli_CmdLine;
}

extern "C" JNIEXPORT void JNICALL Java_cli_CommandLine__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeCmdLineRef)
{
    NativeTraces::TraceMethod("CommandLine.__finalize(I_NativeCmdLineRef)");
    NativeTraces::TraceParam("I_NativeCmdLineRef", "%d", I_NativeCmdLineRef);
    if (const cli::CommandLine* const pcli_CmdLine = (const cli::CommandLine*) I_NativeCmdLineRef)
    {
        NativeObject::Free(pcli_CmdLine);
    }
    NativeTraces::TraceReturn("CommandLine.__finalize()");
}

extern "C" JNIEXPORT jint JNICALL Java_cli_CommandLine__1_1getElementCount(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeCmdLineRef)
{
    NativeTraces::TraceMethod("CommandLine.__getElementCount(I_NativeCmdLineRef)");
    NativeTraces::TraceParam("I_NativeCmdLineRef", "%d", I_NativeCmdLineRef);
    jint i_Count = 0;
    if (const cli::CommandLine* const pcli_CmdLine = (const cli::CommandLine*) I_NativeCmdLineRef)
    {
        for (   cli::CommandLineIterator it(*pcli_CmdLine);
                it.StepIt(); )
        {
            i_Count ++;
        }
    }
    NativeTraces::TraceReturn("CommandLine.__getElementCount()", "%d", i_Count);
    return i_Count;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_CommandLine__1_1getElementAt(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeCmdLineRef, jint I_Position)
{
    NativeTraces::TraceMethod("CommandLine.__getElementAt(I_NativeCmdLineRef, I_Position)");
    NativeTraces::TraceParam("I_NativeCmdLineRef", "%d", I_NativeCmdLineRef);
    NativeTraces::TraceParam("I_Position", "%d", I_Position);
    const cli::Element* pcli_Element = NULL;
    if (const cli::CommandLine* const pcli_CmdLine = (const cli::CommandLine*) I_NativeCmdLineRef)
    {
        int i_Count = 0;
        for (   cli::CommandLineIterator it(*pcli_CmdLine);
                it.StepIt() && (pcli_Element == NULL);
                )
        {
            if (i_Count >= I_Position)
            {
                pcli_Element = *it;
            }
            i_Count ++;
        }
    }
    NativeTraces::TraceReturn("CommandLine.__getElementAt()", "%d", (jint) pcli_Element);
    return (jint) pcli_Element;
}
