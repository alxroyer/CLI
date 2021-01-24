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

#include "cli/syntax_tag.h"

#include "cli_SyntaxTag.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_SyntaxTag__1_1SyntaxTag(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jboolean B_Hollow)
{
    NativeTraces::TraceMethod("SyntaxTag.__SyntaxTag(B_Hollow)");
    NativeTraces::TraceParam("B_Hollow", "%d", (int) B_Hollow);
    cli::SyntaxTag* pcli_Tag = NULL;
    if ((pcli_Tag = new cli::SyntaxTag(B_Hollow)))
    {
        NativeObject::Use(pcli_Tag);
    }
    NativeTraces::TraceReturn("SyntaxTag.__SyntaxTag()", "%d", (int) pcli_Tag);
    return (jint) pcli_Tag;
}

extern "C" JNIEXPORT void JNICALL Java_cli_SyntaxTag__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeSyntaxTagRef)
{
    NativeTraces::TraceMethod("SyntaxTag.__finalize(I_NativeSyntaxTagRef)");
    NativeTraces::TraceParam("I_NativeSyntaxTagRef", "%d", I_NativeSyntaxTagRef);
    if (const cli::SyntaxTag* const pcli_Tag = (const cli::SyntaxTag*) I_NativeSyntaxTagRef)
    {
        NativeObject::Free(pcli_Tag);
    }
    NativeTraces::TraceReturn("SyntaxTag.__finalize()");
}

