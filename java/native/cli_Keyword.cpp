/*
    Copyright (c) 2006-2010, Alexis Royer, http://alexis.royer.free.fr/CLI

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

#include "cli/keyword.h"

#include "cli_Keyword.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_Keyword__1_1Keyword(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jstring PJ_Keyword, jint I_NativeHelpRef)
{
    NativeTraces::TraceMethod("Keyword.__Keyword(PJ_Keyword, I_NativeHelpRef)");
    NativeTraces::TraceParam("I_NativeHelpRef", "%d", I_NativeHelpRef);
    cli::Keyword* pcli_Keyword = NULL;
    if (const cli::Help* const pcli_Help = (const cli::Help*) I_NativeHelpRef)
    {
        if (PJ_Env != NULL)
        {
            if (const char* const str_Keyword = PJ_Env->GetStringUTFChars(PJ_Keyword, 0))
            {
                NativeTraces::TraceParam("PJ_Keyword", "%s", str_Keyword);
                if ((pcli_Keyword = new cli::Keyword(str_Keyword, *pcli_Help)))
                {
                    NativeObject::Use(pcli_Keyword);
                }
                PJ_Env->ReleaseStringUTFChars(PJ_Keyword, str_Keyword);
            }
        }
    }
    NativeTraces::TraceReturn("Keyword.__Keyword()", "%d", (int) pcli_Keyword);
    return (jint) pcli_Keyword;
}

extern "C" JNIEXPORT void JNICALL Java_cli_Keyword__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeKeywordRef)
{
    NativeTraces::TraceMethod("Keyword.__finalize(PJ_Keyword, I_NativeKeywordRef)");
    NativeTraces::TraceParam("I_NativeKeywordRef", "%d", I_NativeKeywordRef);
    if (const cli::Keyword* const pcli_Keyword = (const cli::Keyword* const) I_NativeKeywordRef)
    {
        NativeObject::Free(pcli_Keyword);
    }
    NativeTraces::TraceReturn("Keyword.__finalize()");
}
