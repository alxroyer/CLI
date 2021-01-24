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

#ifndef _CLI_NATIVE_TRACES_H_
#define _CLI_NATIVE_TRACES_H_

#include "cli/traces.h"
#include "cli/io_device.h"

#include "jni.h"


class NativeTraces
{
public:
    static void TraceMethod(const char* const STR_Method)
    {
        //  if (GetbTraceOn())
        //  {
        //      printf(">> %s\n", STR_Method);
        //  }
        GetTraceDevice() << ">> " << STR_Method << cli::endl;
    }

    template <class T> static void TraceParam(
            const char* const STR_ParamName,
            const char* const STR_Format,
            const T T_Value)
    {
        //  if (GetbTraceOn())
        //  {
        //      printf("\t%s = ", );
        //      printf(STR_Format, );
        //      printf("\n");
        //  }
        GetTraceDevice() << "\t" << STR_ParamName << " = " << T_Value << cli::endl;
    }

    template <class T> static void TraceValue(
            const char* const STR_ValueName,
            const char* const STR_Format,
            const T T_Value)
    {
        //  if (GetbTraceOn())
        //  {
        //      printf("\t\t-> %s = ", STR_ValueName);
        //      printf(STR_Format, T_Value);
        //      printf("\n");
        //  }
        GetTraceDevice() << "\t" << "-> " << STR_ValueName << " = " << T_Value << cli::endl;
    }

    static void TraceReturn(const char* const STR_Method)
    {
        //  if (GetbTraceOn())
        //  {
        //      printf("<< %s\n", STR_Method);
        //  }
        GetTraceDevice() << "<< " << STR_Method << cli::endl;
    }

    template <class T> static void TraceReturn(
            const char* const STR_Method,
            const char* const STR_Format,
            const T T_Value)
    {
        //  if (GetbTraceOn())
        //  {
        //      printf("<< %s : ", STR_Method);
        //      printf(STR_Format, T_Value);
        //      printf("\n");
        //  }
        GetTraceDevice() << "<< " << STR_Method << " : " << T_Value << cli::endl;
    }

    static void TraceInstance(
            const int I_NativeObjectRef,
            const int I_Tokens,
            const bool B_AutoDelete)
    {
        //  if (GetbTraceOn())
        //  {
        //      printf("[object %d] tokens = %d, auto-delete: %s",
        //          I_NativeObjectRef, I_Tokens, B_AutoDelete ? "yes" : "no");
        //      if ((I_Tokens <= 0) && (B_AutoDelete))
        //      {
        //          printf(" -> deletion");
        //      }
        //      printf("\n");
        //  }
        const cli::OutputDevice& cli_Trace = GetTraceDevice();
        cli_Trace << "[object " << I_NativeObjectRef << "] ";
        cli_Trace << "tokens = " << I_Tokens << ", ";
        cli_Trace << "auto-delete: " << (B_AutoDelete ? "yes" : "no");
        if ((I_Tokens <= 0) && (B_AutoDelete))
        {
            cli_Trace << " -> deletion";
        }
        cli_Trace << cli::endl;
    }

public:
    static const cli::TraceClass& GetTraceClass(void);
private:
    static const cli::OutputDevice& GetTraceDevice(void);
};

#endif // _CLI_NATIVE_TRACES_H_
