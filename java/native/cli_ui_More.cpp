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

#include "cli/ui_more.h"

#include "cli_ui_More.h"

#include "NativeObject.h"
#include "NativeExec.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jlong JNICALL Java_cli_ui_More__1_1More(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeParentContextRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ui.More.__More(I64_NativeParentContextRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeParentContextRef", I64_NativeParentContextRef) << cli::endl;
    NativeObject::REF i64_MoreRef = 0;
    cli::ui::More* pcli_More = NULL;
    if (I64_NativeParentContextRef != 0)
    {
        if (cli::ExecutionContext* const pcli_ParentContext = NativeObject::GetNativeObject<cli::ExecutionContext*>(I64_NativeParentContextRef))
        {
            pcli_More = new cli::ui::More(*pcli_ParentContext, 0, 0); // UI_MaxLines & UI_MaxLineLength not taken in account by tk STL implementation
        }
    }
    else
    {
        pcli_More = new cli::ui::More(0, 0); // UI_MaxLines & UI_MaxLineLength not taken in account by tk STL implementation
    }
    if (pcli_More != NULL)
    {
        NativeObject::Use(*pcli_More);
        i64_MoreRef = NativeObject::GetNativeRef(*pcli_More);
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("ui.More.__More()", i64_MoreRef) << cli::endl;
    return i64_MoreRef;
}

extern "C" JNIEXPORT jlong JNICALL Java_cli_ui_More__1_1getText(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jlong I64_NativeMoreRef)
{
    NativeExec::GetInstance().RegJNIEnv(PJ_Env);

    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::Begin("ui.More.__getText(I64_NativeMoreRef)") << cli::endl;
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::ParamInt64("I64_NativeMoreRef", I64_NativeMoreRef) << cli::endl;
    NativeObject::REF i64_TextRef = 0;
    if (cli::ui::More* const pcli_More = NativeObject::GetNativeObject<cli::ui::More*>(I64_NativeMoreRef))
    {
        i64_TextRef = NativeObject::GetNativeRef(pcli_More->GetText());
    }
    cli::GetTraces().Trace(TRACE_JNI) << NativeTraces::EndInt64("ui.More.__getText()", i64_TextRef) << cli::endl;
    return i64_TextRef;
}
