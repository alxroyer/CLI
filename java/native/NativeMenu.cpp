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

#include <deque>
#include <iostream>

#include "cli/menu.h"
#include "cli/shell.h"
#include "cli/param.h"
#include "cli/endl.h"
#include "cli/debug.h"

#include "NativeMenu.h"
#include "NativeObject.h"
#include "NativeTraces.h"


const bool __NativeMenu__Execute(
        JNIEnv* const PJ_Env, const std::string& STR_Class,
        const cli::Menu& CLI_Menu, const cli::CommandLine& CLI_CmdLine)
{
    NativeTraces::TraceMethod("__NativeMenu__Execute(CLI_CmdLine)");
    NativeTraces::TraceParam("PJ_Env", "0x%08x", PJ_Env);
    NativeTraces::TraceParam("STR_Class", "%s", STR_Class.c_str());
    NativeTraces::TraceParam("CLI_Menu", "%d", (int) & CLI_Menu);
    NativeTraces::TraceParam("CLI_CmdLine", "%d", (int) & CLI_CmdLine);

    jboolean b_Res = false;

    if (PJ_Env != NULL)
    {
        // For each parameter, create a corresponding Java object.
        std::deque<const cli::Param*> q_Params;
        const cli::Element* pcli_Element = NULL;
        for (   cli::CommandLineIterator it(CLI_CmdLine);
                it.StepIt() && (pcli_Element = *it); )
        {
            NativeTraces::TraceValue("word", "%s", (const char*) pcli_Element->GetKeyword());
            if (const cli::Param* const pcli_Param = dynamic_cast<const cli::Param*>(pcli_Element))
            {
                if (pcli_Param->GetCloned())
                {
                    if (NativeObject::CreateFromNative(PJ_Env, *pcli_Param))
                    {
                        q_Params.push_back(pcli_Param);
                    }
                }
            }
        }

        // Command line object interfacing.
        if (NativeObject::CreateFromNative(PJ_Env, CLI_CmdLine))
        {
            // Java menu execution.
            if (const jclass pj_MenuClass = PJ_Env->FindClass(STR_Class.c_str()))
            {
                NativeTraces::TraceValue("pj_MenuClass", "0x%08x", pj_MenuClass);
                if (const jmethodID pj_ExecuteMethodID = PJ_Env->GetMethodID(pj_MenuClass, "__execute", "(I)Z"))
                {
                    NativeTraces::TraceValue("pj_ExecuteMethodID", "0x%08x", pj_ExecuteMethodID);
                    if (const jobject pj_Object = NativeObject::GetJavaObject(PJ_Env, (int) & CLI_Menu))
                    {
                        NativeTraces::TraceValue("pj_Object", "0x%08x", pj_Object);
                        b_Res = PJ_Env->CallBooleanMethod(pj_Object, pj_ExecuteMethodID, (int) & CLI_CmdLine);
                        // Display error when Java code did not execute the command.
                        if (! b_Res)
                        {
                            std::string str_CommandLine;
                            const cli::Element* pcli_Element = NULL;
                            for (   cli::CommandLineIterator it(CLI_CmdLine);
                                    it.StepIt() && (pcli_Element = *it); )
                            {
                                if (! dynamic_cast<const cli::Endl*>(pcli_Element))
                                {
                                    if (! str_CommandLine.empty())
                                    {
                                        str_CommandLine += " ";
                                    }
                                    str_CommandLine += pcli_Element->GetKeyword();
                                }
                            }
                            // Display unless the command is a well known command.
                            if (true
                                && (str_CommandLine != "cli-config")
                                && (str_CommandLine != "exit")
                                && (str_CommandLine != "help")
                                && (str_CommandLine != "pwm")
                                && (str_CommandLine != "quit")
                                && (str_CommandLine != "traces"))
                            {
                                CLI_Menu.GetShell().GetStream(cli::ERROR_STREAM)
                                    << "Java failed while executing command: "
                                    << "'" << str_CommandLine.c_str() << "'"
                                    << cli::endl;
                            }
                        }
                    }
                }
            }

            NativeObject::DeleteFromNative(PJ_Env, CLI_CmdLine);
        }

        // For each parameter, release the corresponding Java object.
        while (! q_Params.empty())
        {
            if (const cli::Param* const pcli_Param = q_Params.back())
            {
                NativeObject::DeleteFromNative(PJ_Env, *pcli_Param);
            }
            q_Params.pop_back();
        }
    }

    NativeTraces::TraceReturn("__NativeMenu__Execute()", "%d", (int) b_Res);
    return b_Res;
}

const bool __NativeMenu__OnError(
        JNIEnv* const PJ_Env, const std::string& STR_Class,
        const cli::Menu& CLI_Menu, const cli::ResourceString& CLI_Location, const cli::ResourceString& CLI_ErrorMessage)
{
    NativeTraces::TraceMethod("__NativeMenu__OnError()");
    NativeTraces::TraceParam("PJ_Env", "0x%08x", PJ_Env);
    NativeTraces::TraceParam("STR_Class", "%s", STR_Class.c_str());
    NativeTraces::TraceParam("CLI_Menu", "%d", (int) & CLI_Menu);
    NativeTraces::TraceParam("CLI_Location", "%d", (int) & CLI_Location);
    NativeTraces::TraceParam("CLI_ErrorMessage", "%d", (int) & CLI_ErrorMessage);

    jboolean b_Res = false;
    if (PJ_Env != NULL)
    {
        // Java menu execution.
        if (const jclass pj_MenuClass = PJ_Env->FindClass(STR_Class.c_str()))
        {
            NativeTraces::TraceValue("pj_MenuClass", "0x%08x", pj_MenuClass);
            if (const jmethodID pj_OnErrorMethodID = PJ_Env->GetMethodID(pj_MenuClass, "__onError", "(II)V"))
            {
                NativeTraces::TraceValue("pj_OnErrorMethodID", "0x%08x", pj_OnErrorMethodID);
                if (const jobject pj_Object = NativeObject::GetJavaObject(PJ_Env, (int) & CLI_Menu))
                {
                    if (NativeObject::CreateFromNative(PJ_Env, CLI_Location))
                    {
                        if (NativeObject::CreateFromNative(PJ_Env, CLI_ErrorMessage))
                        {
                            NativeTraces::TraceValue("pj_Object", "0x%08x", pj_Object);
                            PJ_Env->CallVoidMethod(pj_Object, pj_OnErrorMethodID, (int) & CLI_Location, (int) & CLI_ErrorMessage);
                            b_Res = true;

                            NativeObject::DeleteFromNative(PJ_Env, CLI_Location);
                        }
                        NativeObject::DeleteFromNative(PJ_Env, CLI_ErrorMessage);
                    }
                }
            }
        }
    }
    NativeTraces::TraceReturn("__NativeMenu__OnError()", "%d", (int) b_Res);
    return b_Res;
}

const bool __NativeMenu__OnExit(
        JNIEnv* const PJ_Env, const std::string& STR_Class,
        const cli::Menu& CLI_Menu)
{
    NativeTraces::TraceMethod("__NativeMenu__OnExit()");
    NativeTraces::TraceParam("PJ_Env", "0x%08x", PJ_Env);
    NativeTraces::TraceParam("STR_Class", "%s", STR_Class.c_str());
    NativeTraces::TraceParam("CLI_Menu", "%d", (int) & CLI_Menu);

    jboolean b_Res = false;
    if (PJ_Env != NULL)
    {
        // Java menu execution.
        if (const jclass pj_MenuClass = PJ_Env->FindClass(STR_Class.c_str()))
        {
            NativeTraces::TraceValue("pj_MenuClass", "0x%08x", pj_MenuClass);
            if (const jmethodID pj_OnExitMethodID = PJ_Env->GetMethodID(pj_MenuClass, "__onExit", "()V"))
            {
                NativeTraces::TraceValue("pj_OnExitMethodID", "0x%08x", pj_OnExitMethodID);
                if (const jobject pj_Object = NativeObject::GetJavaObject(PJ_Env, (int) & CLI_Menu))
                {
                    NativeTraces::TraceValue("pj_Object", "0x%08x", pj_Object);
                    PJ_Env->CallVoidMethod(pj_Object, pj_OnExitMethodID);
                    b_Res = true;
                }
            }
        }
    }
    NativeTraces::TraceReturn("__NativeMenu__OnExit()", "%d", (int) b_Res);
    return b_Res;
}
