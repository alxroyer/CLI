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


public class TraceClass extends NativeObject
{
    public TraceClass(String STR_ClassName) {
        super(__TraceClass(STR_ClassName, 0));
    }
    public TraceClass(String STR_ClassName, Help CLI_Help) {
        super(__TraceClass(STR_ClassName, CLI_Help.getNativeRef()));
    }
    private static final native int __TraceClass(String STR_ClassName, int I_NativeHelpRef);

    protected void finalize() throws Throwable {
        __finalize(this.getNativeRef());
        super.finalize();
    }
    private static final native void __finalize(int I_NativeTraceClassRef);

    public String getClassName() {
        return __getClassName(getNativeRef());
    }
    private static final native String __getClassName(int I_NativeTraceClassRef);

    public String getHelp(int E_Language) {
        return __getHelp(getNativeRef(), E_Language);
    }
    private static final native String __getHelp(int I_NativeTraceClassRef, int E_Language);
}