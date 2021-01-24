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

#include <string>

#include "cli/element.h"
#include "cli/io_device.h"

#include "cli_Element.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jstring JNICALL Java_cli_Element__1_1getKeyword(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeElementRef)
{
    NativeTraces::TraceMethod("Element.__getKeyword(I_NativeElementRef)");
    NativeTraces::TraceParam("I_NativeElementRef", "%d", I_NativeElementRef);
    jstring pj_Keyword = NULL;
    if (const cli::Element* const pcli_Element = (const cli::Element*) I_NativeElementRef)
    {
        if (PJ_Env != NULL)
        {
            NativeTraces::TraceReturn("Element.__getKeyword()", "%s", (const char*) pcli_Element->GetKeyword());
            pj_Keyword = PJ_Env->NewStringUTF(pcli_Element->GetKeyword());
        }
    }
    return pj_Keyword;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Element__1_1getHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeElementRef)
{
    NativeTraces::TraceMethod("Element.__getHelp(I_NativeElementRef, I_NativeHelpRef)");
    NativeTraces::TraceParam("I_NativeElementRef", "%d", I_NativeElementRef);
    const cli::Help* pcli_Help = NULL;
    if (const cli::Element* const pcli_Element = (const cli::Element*) I_NativeElementRef)
    {
        if (PJ_Env != NULL)
        {
            if ((pcli_Help = (cli::Help*) & pcli_Element->GetHelp()))
            {
                NativeObject::CreateFromNative(PJ_Env, *pcli_Help);
                NativeObject::Delegate(pcli_Help, pcli_Element);
            }
        }
    }
    NativeTraces::TraceReturn("Element.__getHelp()", "%d", (int) pcli_Help);
    return (jint) pcli_Help;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Element__1_1getOutputStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeElementRef)
{
    NativeTraces::TraceMethod("Element.__getOutputStream(I_NativeElementRef)");
    NativeTraces::TraceParam("I_NativeElementRef", "%d", I_NativeElementRef);
    const cli::OutputDevice* pcli_OutputDevice = NULL;
    if (const cli::Element* const pcli_Element = (const cli::Element*) I_NativeElementRef)
    {
        pcli_OutputDevice = & pcli_Element->GetOutputStream();
    }
    NativeTraces::TraceReturn("Element.__getOutputStream()", "%d", (int) pcli_OutputDevice);
    return (jint) pcli_OutputDevice;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Element__1_1getCli(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeElementRef)
{
    NativeTraces::TraceMethod("Element.__getCli(I_NativeElementRef)");
    NativeTraces::TraceParam("I_NativeElementRef", "%d", I_NativeElementRef);
    const cli::Cli* pcli_Cli = NULL;
    if (const cli::Element* const pcli_Element = (const cli::Element*) I_NativeElementRef)
    {
        pcli_Cli = & pcli_Element->GetCli();
    }
    NativeTraces::TraceReturn("Element.__getCli()", "%d", (int) pcli_Cli);
    return (jint) pcli_Cli;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Element__1_1getShell(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeElementRef)
{
    NativeTraces::TraceMethod("Element.__getShell(I_NativeElementRef)");
    NativeTraces::TraceParam("I_NativeElementRef", "%d", I_NativeElementRef);
    const cli::Shell* pcli_Shell = NULL;
    if (const cli::Element* const pcli_Element = (const cli::Element*) I_NativeElementRef)
    {
        pcli_Shell = & pcli_Element->GetShell();
    }
    NativeTraces::TraceReturn("Element.__getShell()", "%d", (int) pcli_Shell);
    return (jint) pcli_Shell;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Element__1_1getErrorStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeElementRef)
{
    NativeTraces::TraceMethod("Element.__getErrorStream(I_NativeElementRef)");
    NativeTraces::TraceParam("I_NativeElementRef", "%d", I_NativeElementRef);
    const cli::OutputDevice* pcli_ErrorDevice = NULL;
    if (const cli::Element* const pcli_Element = (const cli::Element*) I_NativeElementRef)
    {
        pcli_ErrorDevice = & pcli_Element->GetErrorStream();
    }
    NativeTraces::TraceReturn("Element.__getErrorStream()", "%d", (int) pcli_ErrorDevice);
    return (jint) pcli_ErrorDevice;
}
