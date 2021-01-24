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

#include <jni.h>

#include "cli/command_line.h"
#include "cli/param_string.h"
#include "cli/param_int.h"
#include "cli/param_float.h"
#include "cli/param_host.h"

#include "NativeObject.h"
#include "NativeTraces.h"


NativeObject::ObjectMap NativeObject::m_mapTokens;

const jobject NativeObject::GetJavaObject(JNIEnv* const PJ_Env, int I_NativeObjectRef)
{
    jobject pj_Object = NULL;
    NativeTraces::TraceMethod("GetJavaObject(I_NativeObjectRef)");
    NativeTraces::TraceParam("I_NativeObjectRef", "%d", I_NativeObjectRef);
    if (PJ_Env != NULL)
    {
        if (const jclass pj_Class = PJ_Env->FindClass("cli/NativeObject"))
        {
            NativeTraces::TraceValue("pj_Class", "0x%08x", pj_Class);
            PJ_Env->ExceptionClear();
            if (const jmethodID j_MethodID = PJ_Env->GetStaticMethodID(pj_Class, "getObject", "(I)Lcli/NativeObject;"))
            {
                NativeTraces::TraceValue("j_MethodID", "%d", j_MethodID);
                pj_Object = PJ_Env->CallStaticObjectMethod(pj_Class, j_MethodID, I_NativeObjectRef);
            }
        }
    }
    NativeTraces::TraceReturn("GetJavaObject()", "0x%08x", pj_Object);
    return pj_Object;
}

const bool NativeObject::CreateFromNative(JNIEnv* const PJ_Env, const cli::Object& CLI_Object)
{
    NativeTraces::TraceMethod("NativeObject::CreateFromNative(PJ_Env, CLI_Object)");
    NativeTraces::TraceParam("PJ_Env", "0x%08x", PJ_Env);
    NativeTraces::TraceParam("CLI_Object", "%d", (int) & CLI_Object);

    bool b_Res = false;

    if (const jclass pj_Class = PJ_Env->FindClass(GetJavaClassName(CLI_Object).c_str()))
    {
        NativeTraces::TraceValue("pj_Class", "0x%08x", pj_Class);

        const jmethodID pj_CreateMethodID = PJ_Env->GetStaticMethodID(pj_Class, "createFromNative", "(I)V");
            NativeTraces::TraceValue("pj_CreateMethodID", "0x%08x", pj_CreateMethodID);
        const jmethodID pj_DeleteMethodID = PJ_Env->GetStaticMethodID(pj_Class, "deleteFromNative", "(I)V");
            NativeTraces::TraceValue("pj_DeleteMethodID", "0x%08x", pj_DeleteMethodID);
        if ((pj_CreateMethodID != NULL) && (pj_DeleteMethodID != NULL))
        {
            // Create a java object for the command line.
            PJ_Env->CallStaticVoidMethod(pj_Class, pj_CreateMethodID, (int) & CLI_Object);
            b_Res = true;
        }
    }

    NativeTraces::TraceReturn("NativeObject::CreateFromNative()", "%d", (int) b_Res);
    return b_Res;
}

const bool NativeObject::DeleteFromNative(JNIEnv* const PJ_Env, const cli::Object& CLI_Object)
{
    NativeTraces::TraceMethod("NativeObject::DeleteFromNative(PJ_Env, CLI_Object)");
    NativeTraces::TraceParam("PJ_Env", "0x%08x", PJ_Env);
    NativeTraces::TraceParam("CLI_Object", "%d", (int) & CLI_Object);

    bool b_Res = false;

    if (const jclass pj_Class = PJ_Env->FindClass(GetJavaClassName(CLI_Object).c_str()))
    {
        NativeTraces::TraceValue("pj_Class", "0x%08x", pj_Class);

        const jmethodID pj_CreateMethodID = PJ_Env->GetStaticMethodID(pj_Class, "createFromNative", "(I)V");
            NativeTraces::TraceValue("pj_CreateMethodID", "0x%08x", pj_CreateMethodID);
        const jmethodID pj_DeleteMethodID = PJ_Env->GetStaticMethodID(pj_Class, "deleteFromNative", "(I)V");
            NativeTraces::TraceValue("pj_DeleteMethodID", "0x%08x", pj_DeleteMethodID);
        if ((pj_CreateMethodID != NULL) && (pj_DeleteMethodID != NULL))
        {
            // Create a java object for the command line.
            PJ_Env->CallStaticVoidMethod(pj_Class, pj_DeleteMethodID, (int) & CLI_Object);
            b_Res = true;
        }
    }

    NativeTraces::TraceReturn("NativeObject::DeleteFromNative()", "%d", (int) b_Res);
    return b_Res;
}

void NativeObject::Use(const int I_ObjectRef)
{
    if (I_ObjectRef != 0)
    {
        // Find out if the object is already registered.
        ObjectMap::iterator it = m_mapTokens.find(I_ObjectRef);
        if (it != m_mapTokens.end())
        {
            // If it is, simply increment its token number.
            it->second.iTokens ++;
            NativeTraces::TraceInstance(I_ObjectRef, it->second.iTokens, it->second.bAutoDelete);
        }
        else
        {
            // If not yet, add a new object info structure.
            ObjectInfo t_Info; memset(& t_Info, '\0', sizeof(t_Info));
            t_Info.iTokens = 1;
            t_Info.bAutoDelete = true;
            m_mapTokens[I_ObjectRef] = t_Info;
            NativeTraces::TraceInstance(I_ObjectRef, 1, true);
        }
    }
}

const bool NativeObject::Free(const int I_ObjectRef)
{
    bool b_AutoDelete = false;

    // Find out if the object is registered.
    ObjectMap::iterator it = m_mapTokens.find(I_ObjectRef);
    if (it != m_mapTokens.end())
    {
        // If it is, decrement the token number.
        it->second.iTokens --;
        NativeTraces::TraceInstance(I_ObjectRef, it->second.iTokens, it->second.bAutoDelete);
        // When this number falls down 0, perform deletion.
        if (it->second.iTokens <= 0)
        {
            // Auto-deletion notification.
            b_AutoDelete = it->second.bAutoDelete;
            // Unregistration.
            m_mapTokens.erase(it);
        }
    }

    return b_AutoDelete;
}

void NativeObject::Delegate(const int I_WhatRef, const int I_WhoRef)
{
    // Find out if the object is registered.
    ObjectMap::iterator it = m_mapTokens.find(I_WhatRef);
    if (it != m_mapTokens.end())
    {
        // Notify no deletion for this object.
        it->second.bAutoDelete = false;
        NativeTraces::TraceInstance(I_WhatRef, it->second.iTokens, it->second.bAutoDelete);
    }
}

const std::string NativeObject::GetJavaClassName(const cli::Object& CLI_Object)
{
    NativeTraces::TraceMethod("NativeObject::GetJavaClassName(CLI_Object)");
    NativeTraces::TraceParam("CLI_Object", "%d", (int) & CLI_Object);

    std::string str_ClassName;
    if (0) {}
    else if (dynamic_cast<const cli::CommandLine*>(& CLI_Object)) { str_ClassName = "cli/CommandLine"; }
    else if (dynamic_cast<const cli::ParamString*>(& CLI_Object)) { str_ClassName = "cli/ParamString"; }
    else if (dynamic_cast<const cli::ParamInt*>(& CLI_Object)) { str_ClassName = "cli/ParamInt"; }
    else if (dynamic_cast<const cli::ParamFloat*>(& CLI_Object)) { str_ClassName = "cli/ParamFloat"; }
    else if (dynamic_cast<const cli::ParamHost*>(& CLI_Object)) { str_ClassName = "cli/ParamHost"; }

    NativeTraces::TraceReturn("NativeObject::GetJavaClassName()", "%s", str_ClassName.c_str());
    return str_ClassName;
}
