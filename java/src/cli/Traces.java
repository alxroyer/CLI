/*
    Copyright (c) 2006-2007, Alexis Royer

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

package cli;


public final class Traces
{
    public static final void setFilter(TraceClass CLI_Class, boolean B_ShowTraces) {
        __setFilter(CLI_Class.getNativeRef(), B_ShowTraces);
    }
    private static final native void __setFilter(int I_NativeTraceClassRef, boolean B_ShowTraces);

    public static final void setAllFilter(boolean B_ShowTraces) {
        __setAllFilter(B_ShowTraces);
    }
    public static final native void __setAllFilter(boolean B_ShowTraces);

    public static final void trace(TraceClass CLI_TraceClass, String STR_Text) {
        __trace(CLI_TraceClass.getNativeRef(), STR_Text);
    }
    private static final native void __trace(int I_TraceClassNativeRef, String STR_Text);

    public static final void traceMethod(String STR_Method) {
        __traceMethod(STR_Method);
    }
    private static final native void __traceMethod(String STR_Method);

    public static final void traceParam(String STR_ParamName, String STR_ParamValue) {
        __traceParam(STR_ParamName, STR_ParamValue);
    }
    private static final native void __traceParam(String STR_ParamName, String STR_ParamValue);

    public static final void traceValue(String STR_VarName, String STR_VarValue) {
        __traceValue(STR_VarName, STR_VarValue);
    }
    private static final native void __traceValue(String STR_VarName, String STR_VarValue);

    public static final void traceReturn(String STR_Method) {
        __traceReturn(STR_Method);
    }
    public static final void traceReturn(String STR_Method, String STR_Result) {
        if (STR_Result != null) {
            __traceReturn(STR_Method, STR_Result);
        } else {
            __traceReturn(STR_Method);
        }
    }
    private static final native void __traceReturn(String STR_Method);
    private static final native void __traceReturn(String STR_Method, String STR_Result);
}