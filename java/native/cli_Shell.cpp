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

#include "cli/shell.h"
#include "cli/cli.h"
#include "cli/io_device.h"

#include "cli_Shell.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1Shell(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeCliRef)
{
    NativeTraces::TraceMethod("Shell.__Shell(I_NativeCliRef)");
    NativeTraces::TraceParam("I_NativeCliRef", "%d", I_NativeCliRef);
    cli::Shell* pcli_Shell = NULL;
    if (const cli::Cli* const pcli_Cli = (const cli::Cli*) I_NativeCliRef)
    {
        if ((pcli_Shell = new cli::Shell(*pcli_Cli)))
        {
            NativeObject::Use(pcli_Cli);
            NativeObject::Use(pcli_Shell);
        }
    }
    NativeTraces::TraceReturn("Shell.__Shell()", "%d", (int) pcli_Shell);
    return (jint) pcli_Shell;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        const cli::Cli* const pcli_Cli = & pcli_Shell->GetCli();
        NativeObject::Free(pcli_Shell);
        NativeObject::Free(pcli_Cli);
    }
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getCli(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__getCli(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    const cli::Cli* pcli_Cli = NULL;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        pcli_Cli = & pcli_Shell->GetCli();
    }
    NativeTraces::TraceReturn("Shell.__getCli()", "%d", (int) pcli_Cli);
    return (jint) pcli_Cli;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getInput(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__getInput(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    const cli::IODevice* pcli_Input = NULL;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        pcli_Input = & pcli_Shell->GetInput();
    }
    NativeTraces::TraceReturn("Shell.__getInput()", "%d", (int) pcli_Input);
    return (jint) pcli_Input;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_StreamType)
{
    NativeTraces::TraceMethod("Shell.__getStream(I_NativeShellRef, E_StreamType)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    const cli::OutputDevice* pcli_Stream = NULL;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        pcli_Stream = & pcli_Shell->GetStream((cli::STREAM_TYPE) E_StreamType);
    }
    NativeTraces::TraceReturn("Shell.__getStream()", "%d", (int) pcli_Stream);
    return (jint) pcli_Stream;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1setStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_StreamType, jint I_NativeDeviceRef)
{
    NativeTraces::TraceMethod("Shell.__setStream(I_NativeShellRef, E_StreamType, I_NativeDeviceRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    NativeTraces::TraceParam("I_NativeDeviceRef", "%d", I_NativeDeviceRef);
    jboolean b_Res = false;
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        if (((E_StreamType >= 0) && (E_StreamType < cli::STREAM_TYPES_COUNT))
            || (E_StreamType == cli::ALL_STREAMS))
        {
            if (cli::OutputDevice* const pcli_Device = (cli::OutputDevice*) I_NativeDeviceRef)
            {
                b_Res = pcli_Shell->SetStream((cli::STREAM_TYPE) E_StreamType, *pcli_Device);
            }
        }
    }
    NativeTraces::TraceReturn("Shell.__setStream()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1streamEnabled(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_StreamType)
{
    NativeTraces::TraceMethod("Shell.__streamEnabled(I_NativeShellRef, E_StreamType)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    jboolean b_Res = false;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        b_Res = pcli_Shell->StreamEnabled((cli::STREAM_TYPE) E_StreamType);
    }
    NativeTraces::TraceReturn("Shell.__streamEnabled()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1enableStream(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_StreamType, jboolean B_Enable)
{
    NativeTraces::TraceMethod("Shell.__enableStream(I_NativeShellRef, E_StreamType, B_Enable)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_StreamType", "%d", E_StreamType);
    NativeTraces::TraceParam("B_Enable", "%d", (int) B_Enable);
    jboolean b_Res = false;
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        b_Res = pcli_Shell->EnableStream((cli::STREAM_TYPE) E_StreamType, B_Enable);
    }
    NativeTraces::TraceReturn("Shell.__enableStream()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setWelcomeMessage(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint I_NativeWelcomeMessageRef)
{
    NativeTraces::TraceMethod("Shell.__setWelcomeMessage(I_NativeShellRef, I_NativeWelcomeMessageRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("I_NativeWelcomeMessageRef", "%d", I_NativeWelcomeMessageRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        if (const cli::ResourceString* const pcli_WelcomeMessage = (const cli::ResourceString*) I_NativeWelcomeMessageRef)
        {
            pcli_Shell->SetWelcomeMessage(*pcli_WelcomeMessage);
        }
    }
    NativeTraces::TraceReturn("Shell.__setWelcomeMessage()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setByeMessage(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint I_NativeByeMessageRef)
{
    NativeTraces::TraceMethod("Shell.__setByeMessage(I_NativeShellRef, I_NativeWelcomeMessageRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("I_NativeByeMessageRef", "%d", I_NativeByeMessageRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        if (const cli::ResourceString* const pcli_ByeMessage = (const cli::ResourceString*) I_NativeByeMessageRef)
        {
            pcli_Shell->SetByeMessage(*pcli_ByeMessage);
        }
    }
    NativeTraces::TraceReturn("Shell.__setByeMessage()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setPrompt(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint I_NativePromptRef)
{
    NativeTraces::TraceMethod("Shell.__setPrompt(I_NativeShellRef, I_NativePromptRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("I_NativePromptRef", "%d", I_NativePromptRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        if (const cli::ResourceString* const pcli_Prompt = (const cli::ResourceString*) I_NativePromptRef)
        {
            pcli_Shell->SetPrompt(*pcli_Prompt);
        }
    }
    NativeTraces::TraceReturn("Shell.__setPrompt()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setErrorFormatting(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint I_NativeLocationPrefixRef, jint I_NativeErrorPrefixRef, jint I_NativeErrorSuffixRef)
{
    NativeTraces::TraceMethod("Shell.__setErrorFormatting(I_NativeShellRef, I_NativeLocationPrefixRef, I_NativeErrorPrefixRef, I_NativeErrorSuffixRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("I_NativeLocationPrefixRef", "%d", I_NativeLocationPrefixRef);
    NativeTraces::TraceParam("I_NativeErrorPrefixRef", "%d", I_NativeErrorPrefixRef);
    NativeTraces::TraceParam("I_NativeErrorSuffixRef", "%d", I_NativeErrorSuffixRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        const cli::ResourceString* const pcli_LocationPrefix = (const cli::ResourceString*) I_NativeLocationPrefixRef;
        const cli::ResourceString* const pcli_ErrorPrefix = (const cli::ResourceString*) I_NativeErrorPrefixRef;
        const cli::ResourceString* const pcli_ErrorSuffix = (const cli::ResourceString*) I_NativeErrorSuffixRef;
        if ((pcli_LocationPrefix != NULL) && (pcli_ErrorPrefix != NULL) && (pcli_ErrorSuffix != NULL))
        {
            pcli_Shell->SetErrorFormatting(*pcli_LocationPrefix, *pcli_ErrorPrefix, *pcli_ErrorSuffix);
        }
    }
    NativeTraces::TraceReturn("Shell.__setErrorFormatting()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setLang(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint E_Lang)
{
    NativeTraces::TraceMethod("Shell.__setLang(I_NativeShellRef, E_Lang)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("E_Lang", "%d", E_Lang);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        if ((E_Lang >= 0) && (E_Lang < cli::ResourceString::LANG_COUNT))
        {
            pcli_Shell->SetLang((cli::ResourceString::LANG) E_Lang);
        }
    }
    NativeTraces::TraceReturn("Shell.__setLang()");
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getLang(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__getLang(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    jint e_Lang = cli::ResourceString::LANG_DEFAULT;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        e_Lang = pcli_Shell->GetLang();
    }
    NativeTraces::TraceReturn("Shell.__getLang()", "%d", e_Lang);
    return e_Lang;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setBeep(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jboolean B_Enable)
{
    NativeTraces::TraceMethod("Shell.__setBeep(I_NativeShellRef, B_Enable)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("B_Enable", "%d", (int) B_Enable);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        pcli_Shell->SetBeep(B_Enable);
    }
    NativeTraces::TraceReturn("Shell.__setBeep()");
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1getBeep(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__getBeep(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    jboolean b_Beep = false;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        b_Beep = pcli_Shell->GetBeep();
    }
    NativeTraces::TraceReturn("Shell.__getBeep()", "%d", (int) b_Beep);
    return b_Beep;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1run(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef, jint I_NativeIODeviceRef)
{
    NativeTraces::TraceMethod("Shell.__run(I_NativeShellRef, I_NativeIODeviceRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    NativeTraces::TraceParam("I_NativeIODeviceRef", "%d", I_NativeIODeviceRef);
    jboolean b_Res = false;
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        if (cli::IODevice* const pcli_IODevice = (cli::IODevice*) I_NativeIODeviceRef)
        {
            NativeObject::Use(pcli_IODevice);
            pcli_Shell->Run(*pcli_IODevice);
            b_Res = true;
            NativeObject::Free(pcli_IODevice);
        }
    }
    NativeTraces::TraceReturn("Shell.__run()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Shell__1_1isRunning(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__isRunning(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    jboolean b_IsRunning = false;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        b_IsRunning = pcli_Shell->IsRunning();
    }
    NativeTraces::TraceReturn("Shell.__isRunning()", "%d", (int) b_IsRunning);
    return b_IsRunning;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getHelpMargin(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__getHelpMargin(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    jint i_HelpMargin = 0;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        i_HelpMargin = pcli_Shell->GetHelpMargin();
    }
    NativeTraces::TraceReturn("Shell.__getHelpMargin()", "%d", i_HelpMargin);
    return i_HelpMargin;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getHelpOffset(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__getHelpOffset(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    jint i_HelpOffset = 0;
    if (const cli::Shell* const pcli_Shell = (const cli::Shell*) I_NativeShellRef)
    {
        i_HelpOffset = pcli_Shell->GetHelpOffset();
    }
    NativeTraces::TraceReturn("Shell.__getHelpOffset()", "%d", i_HelpOffset);
    return i_HelpOffset;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1exitMenu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__exitMenu(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        pcli_Shell->ExitMenu();
    }
    NativeTraces::TraceReturn("Shell.__exitMenu()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1quit(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__quit(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        pcli_Shell->Quit();
    }
    NativeTraces::TraceReturn("Shell.__quit()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1quitThreadSafe(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__quitThreadSafe(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        pcli_Shell->QuitThreadSafe();
    }
    NativeTraces::TraceReturn("Shell.__quitThreadSafe()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1displayHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__displayHelp(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        pcli_Shell->DisplayHelp();
    }
    NativeTraces::TraceReturn("Shell.__displayHelp()");
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1printWorkingMenu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeShellRef)
{
    NativeTraces::TraceMethod("Shell.__printWorkingMenu(I_NativeShellRef)");
    NativeTraces::TraceParam("I_NativeShellRef", "%d", I_NativeShellRef);
    if (cli::Shell* const pcli_Shell = (cli::Shell*) I_NativeShellRef)
    {
        pcli_Shell->PrintWorkingMenu();
    }
    NativeTraces::TraceReturn("Shell.__printWorkingMenu()");
}
