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

#include "cli/cli.h"

#include "cli_Cli.h"

#include "NativeMenu.h"
#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_Cli__1_1Cli(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_Name, jint I_NativeHelpRef)
{
    NativeTraces::TraceMethod("Cli.__Cli(PJ_Name, I_NativeHelpRef)");
    NativeTraces::TraceParam("CliRef", "%d", I_NativeHelpRef);
    cli::Cli* pcli_Cli = NULL;
    if (const cli::Help* const pcli_Help = (const cli::Help*) I_NativeHelpRef)
    {
        if (const char* const str_Name = PJ_Env->GetStringUTFChars(PJ_Name, 0))
        {
            NativeTraces::TraceParam("PJ_Name", "%s", str_Name);
            if ((pcli_Cli = new NativeMenu<cli::Cli>(PJ_Env, "cli/Cli", str_Name, *pcli_Help)))
            {
                NativeObject::Use(pcli_Cli);
            }
            PJ_Env->ReleaseStringUTFChars(PJ_Name, str_Name);
        }
    }
    NativeTraces::TraceReturn("Cli.__Cli()", "%d", (int) pcli_Cli);
    return (jint) pcli_Cli;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Cli__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeCliRef)
{
    NativeTraces::TraceMethod("Cli.__finalize(I_NativeCliRef)");
    NativeTraces::TraceParam("I_NativeCliRef", "%d", I_NativeCliRef);
    if (const cli::Cli* const pcli_Cli = (const cli::Cli*) I_NativeCliRef)
    {
        NativeObject::Free(pcli_Cli);
    }
    NativeTraces::TraceReturn("Cli.__finalize()");
}

extern "C" JNIEXPORT jintArray JNICALL Java_cli_Cli__1_1findFromName(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_RegExp)
{
    NativeTraces::TraceMethod("Cli.__findFromName(PJ_RegExp)");
    jintArray j_Array = NULL;
    if (PJ_Env != NULL)
    {
        for (unsigned int ui_ListSize = 10; j_Array == NULL; ui_ListSize += 10)
        {
            cli::Cli::List cli_CliList(ui_ListSize);
            if (const char* const str_RegExp = PJ_Env->GetStringUTFChars(PJ_RegExp, 0))
            {
                NativeTraces::TraceParam("PJ_RegExp", "%s", str_RegExp);
                cli::Cli::FindFromName(cli_CliList, str_RegExp);
            }
            // If the list is full, try again with a bigger list.
            if (cli_CliList.GetCount() < ui_ListSize)
            {
                if ((j_Array = PJ_Env->NewIntArray(cli_CliList.GetCount())))
                {
                    if (! cli_CliList.IsEmpty())
                    {
                        if (jint* const pi_Array = new jint[cli_CliList.GetCount()])
                        {
                            unsigned int ui = 0;
                            for (   cli::Cli::List::Iterator it = cli_CliList.GetIterator();
                                    cli_CliList.IsValid(it);
                                    cli_CliList.MoveNext(it))
                            {
                                pi_Array[ui] = (jint) cli_CliList.GetAt(it);
                                NativeTraces::TraceReturn("Cli.__findFromName()", "%d", (int) pi_Array[ui]);
                                ui ++;
                            }
                            PJ_Env->SetIntArrayRegion(j_Array, 0, cli_CliList.GetCount(), pi_Array);
                            delete [] pi_Array;
                        }
                    }
                }
            }
        }
    }
    return j_Array;
}

extern "C" JNIEXPORT jstring JNICALL Java_cli_Cli__1_1getName(JNIEnv* PJ_Env, jclass PJ_Class, jint I_NativeCliRef)
{
    NativeTraces::TraceMethod("Cli.__getName(I_NativeCliRef)");
    NativeTraces::TraceParam("PJ_RegExp", "%d", I_NativeCliRef);
    jstring j_Name = NULL;
    if (const cli::Cli* const pcli_Cli = (const cli::Cli*) I_NativeCliRef)
    {
        if (PJ_Env != NULL)
        {
            NativeTraces::TraceReturn("Cli.__getName()", "%s", (const char*) pcli_Cli->GetName());
            j_Name = PJ_Env->NewStringUTF(pcli_Cli->GetName());
        }
    }
    return j_Name;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Cli__1_1addMenu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeCliRef, jint I_NativeMenuRef)
{
    NativeTraces::TraceMethod("Cli.addMenu(I_NativeCliRef, I_NativeMenuRef)");
    NativeTraces::TraceParam("I_NativeCliRef", "%d", I_NativeCliRef);
    NativeTraces::TraceParam("I_NativeMenuRef", "%d", I_NativeMenuRef);
    jboolean b_Res = false;
    if (cli::Cli* const pcli_Cli = (cli::Cli*) I_NativeCliRef)
    {
        if (cli::Menu* const pcli_Menu = (cli::Menu*) I_NativeMenuRef)
        {
            pcli_Cli->AddMenu(pcli_Menu);
            NativeObject::Delegate(pcli_Menu, pcli_Cli);
            b_Res = true;
        }
    }
    NativeTraces::TraceReturn("Cli.addMenu()", "%d", (int) b_Res);
    return b_Res;
}

extern "C" JNIEXPORT jboolean JNICALL Java_cli_Cli__1_1enableConfigMenu(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeCliRef, jboolean B_Enable)
{
    NativeTraces::TraceMethod("Cli.enableConfigMenu(I_NativeCliRef, B_Enable)");
    NativeTraces::TraceParam("I_NativeCliRef", "%d", I_NativeCliRef);
    NativeTraces::TraceParam("B_Enable", "%d", B_Enable);
    jboolean b_Res = false;
    if (cli::Cli* const pcli_Cli = (cli::Cli*) I_NativeCliRef)
    {
        if (cli::Menu* const pcli_Menu = (cli::Menu*) I_NativeCliRef)
        {
            pcli_Cli->EnableConfigMenu(B_Enable);
            b_Res = true;
        }
    }
    NativeTraces::TraceReturn("Cli.enableConfigMenu()", "%d", b_Res);
    return b_Res;
}

