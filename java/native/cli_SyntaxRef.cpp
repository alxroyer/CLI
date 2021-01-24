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

#include "cli/syntax_tag.h"

#include "cli_SyntaxRef.h"

#include "NativeObject.h"
#include "NativeTraces.h"


extern "C" JNIEXPORT jint JNICALL Java_cli_SyntaxRef__1_1SyntaxRef(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeTagRef)
{
    NativeTraces::TraceMethod("SyntaxRef.__SyntaxRef(I_NativeTagRef)");
    NativeTraces::TraceParam("I_NativeTagRef", "%d", I_NativeTagRef);
    cli::SyntaxRef* pcli_SyntaxRef = NULL;
    if (const cli::SyntaxTag* const pcli_SyntaxTag = (const cli::SyntaxTag*) I_NativeTagRef)
    {
        if ((pcli_SyntaxRef = new cli::SyntaxRef(*pcli_SyntaxTag)))
        {
            NativeObject::Use(pcli_SyntaxRef);
        }
    }
    NativeTraces::TraceReturn("SyntaxRef.__SyntaxRef()", "%d", (int) pcli_SyntaxRef);
    return (jint) pcli_SyntaxRef;
}

extern "C" JNIEXPORT void JNICALL Java_cli_SyntaxRef__1_1finalize(
        JNIEnv* PJ_Env, jclass PJ_Class,
        jint I_NativeTagRefRef)
{
    NativeTraces::TraceMethod("SyntaxRef.__finalize(I_NativeTagRefRef)");
    NativeTraces::TraceParam("I_NativeTagRefRef", "%d", I_NativeTagRefRef);
    if (const cli::SyntaxRef* const pcli_SyntaxRef = (const cli::SyntaxRef*) I_NativeTagRefRef)
    {
        NativeObject::Free(pcli_SyntaxRef);
    }
    NativeTraces::TraceReturn("SyntaxRef.__finalize()");
}

