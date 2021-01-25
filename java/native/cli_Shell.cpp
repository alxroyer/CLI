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

#include "cli/shell.h"
#include "cli/cli.h"
#include "cli/io_device.h"

#include "cli_Shell.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jlong JNICALL Java_cli_Shell__1_1Shell(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeParentContextRef, jlong I64_NativeCliRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__Shell(I64_NativeParentContextRef, I64_NativeCliRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeParentContextRef", I64_NativeParentContextRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeCliRef", I64_NativeCliRef) << cli::endl;
    NativeObject::REF i64_ShellRef = 0;
    if (const cli::Cli* const pcli_Cli = NativeObject::GetNativeObject<const cli::Cli*>(I64_NativeCliRef))
    {
        cli::Shell* pcli_Shell = NULL;
        if (I64_NativeParentContextRef != 0)
        {
            if (cli::ExecutionContext* const pcli_ParentContext = NativeObject::GetNativeObject<cli::ExecutionContext*>(I64_NativeParentContextRef))
            {
                pcli_Shell = new cli::Shell(*pcli_ParentContext, *pcli_Cli);
            }
        }
        else
        {
            pcli_Shell = new cli::Shell(*pcli_Cli);
        }
        if (pcli_Shell != NULL)
        {
            NativeObject::Use(*pcli_Cli);
            NativeObject::Use(*pcli_Shell);
            i64_ShellRef = NativeObject::GetNativeRef(*pcli_Shell);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("Shell.__Shell()", i64_ShellRef) << cli::endl;
    return i64_ShellRef;
}

extern "C" JNIEXPORT jlong JNICALL Java_cli_Shell__1_1getCli(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__getCli(I64_NativeShellRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    NativeObject::REF i64_CliRef = 0;
    if (const cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<const cli::Shell*>(I64_NativeShellRef))
    {
        const cli::Cli& cli_Cli = pcli_Shell->GetCli();
        i64_CliRef = NativeObject::GetNativeRef(cli_Cli);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("Shell.__getCli()", i64_CliRef) << cli::endl;
    return i64_CliRef;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setWelcomeMessage(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef, jlong I64_NativeWelcomeMessageRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__setWelcomeMessage(I64_NativeShellRef, I64_NativeWelcomeMessageRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeWelcomeMessageRef", I64_NativeWelcomeMessageRef) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        if (const cli::ResourceString* const pcli_WelcomeMessage = NativeObject::GetNativeObject<const cli::ResourceString*>(I64_NativeWelcomeMessageRef))
        {
            pcli_Shell->SetWelcomeMessage(*pcli_WelcomeMessage);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__setWelcomeMessage()") << cli::endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setByeMessage(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef, jlong I64_NativeByeMessageRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__setByeMessage(I64_NativeShellRef, I64_NativeWelcomeMessageRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeByeMessageRef", I64_NativeByeMessageRef) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        if (const cli::ResourceString* const pcli_ByeMessage = NativeObject::GetNativeObject<const cli::ResourceString*>(I64_NativeByeMessageRef))
        {
            pcli_Shell->SetByeMessage(*pcli_ByeMessage);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__setByeMessage()") << cli::endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1setPrompt(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef, jlong I64_NativePromptRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__setPrompt(I64_NativeShellRef, I64_NativePromptRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativePromptRef", I64_NativePromptRef) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        if (const cli::ResourceString* const pcli_Prompt = NativeObject::GetNativeObject<const cli::ResourceString*>(I64_NativePromptRef))
        {
            pcli_Shell->SetPrompt(*pcli_Prompt);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__setPrompt()") << cli::endl;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getHelpMargin(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__getHelpMargin(I64_NativeShellRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    int i_HelpMargin = 0;
    if (const cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<const cli::Shell*>(I64_NativeShellRef))
    {
        i_HelpMargin = pcli_Shell->GetHelpMargin();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("Shell.__getHelpMargin()", i_HelpMargin) << cli::endl;
    return i_HelpMargin;
}

extern "C" JNIEXPORT jint JNICALL Java_cli_Shell__1_1getHelpOffset(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__getHelpOffset(I64_NativeShellRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    int i_HelpOffset = 0;
    if (const cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<const cli::Shell*>(I64_NativeShellRef))
    {
        i_HelpOffset = pcli_Shell->GetHelpOffset();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt("Shell.__getHelpOffset()", i_HelpOffset) << cli::endl;
    return i_HelpOffset;
}

extern "C" JNIEXPORT jlong JNICALL Java_cli_Shell__1_1getCurrentMenu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef, jint I_MenuIndex)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__getCurrentMenu(I64_NativeShellRef, I_MenuIndex)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt("I_MenuIndex", I_MenuIndex) << cli::endl;
    NativeObject::REF i64_CurrentMenuRef = 0;
    if (const cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<const cli::Shell*>(I64_NativeShellRef))
    {
        if (const cli::Menu* const pcli_CurrentMenu = pcli_Shell->GetCurrentMenu(I_MenuIndex))
        {
            i64_CurrentMenuRef = NativeObject::GetNativeRef(*pcli_CurrentMenu);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("Shell.__getCurrentMenu()", i64_CurrentMenuRef) << cli::endl;
    return i64_CurrentMenuRef;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1enterMenu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef, jlong I64_NativeMenuRef, jboolean B_PromptMenu)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__enterMenu(I64_NativeShellRef, I64_NativeMenuRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeMenuRef", I64_NativeMenuRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamBool("B_PromptMenu", B_PromptMenu) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        if (const cli::Menu* const pcli_Menu = NativeObject::GetNativeObject<const cli::Menu*>(I64_NativeMenuRef))
        {
            pcli_Shell->EnterMenu(*pcli_Menu, B_PromptMenu);
        }
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__enterMenu()") << cli::endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1exitMenu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef, jboolean B_PromptMenu)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__exitMenu(I64_NativeShellRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamBool("B_PromptMenu", B_PromptMenu) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        pcli_Shell->ExitMenu(B_PromptMenu);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__exitMenu()") << cli::endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1quit(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__quit(I64_NativeShellRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        pcli_Shell->Quit();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__quit()") << cli::endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1displayHelp(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__displayHelp(I64_NativeShellRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        pcli_Shell->DisplayHelp();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__displayHelp()") << cli::endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1printWorkingMenu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__printWorkingMenu(I64_NativeShellRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        pcli_Shell->PrintWorkingMenu();
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__printWorkingMenu()") << cli::endl;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Shell__1_1cleanScreen(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeShellRef, jboolean B_PromptMenu)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("Shell.__cleanScreen(I64_NativeShellRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeShellRef", I64_NativeShellRef) << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamBool("B_PromptMenu", B_PromptMenu) << cli::endl;
    if (cli::Shell* const pcli_Shell = NativeObject::GetNativeObject<cli::Shell*>(I64_NativeShellRef))
    {
        pcli_Shell->CleanScreen(B_PromptMenu);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndVoid("Shell.__cleanScreen()") << cli::endl;
}
