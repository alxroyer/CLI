/*
    Copyright (c) 2006-2008, Alexis Royer

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


/** String parameter element. */
public class ParamString extends Param {

    /** Constructor.
        @param CLI_Help Help to be attached to the parameter. */
    public ParamString(Help CLI_Help) {
        super(__ParamString(CLI_Help.getNativeRef()));
    }
    private static final native int __ParamString(int I_NativeHelpRef);

    /** Creation from native code.
        Useful for cloned parameters created for a specific string value given in a command line.
        @param I_NativeParamRef Native instance reference. */
    protected static void createFromNative(int I_NativeParamRef) {
        Traces.traceMethod("ParamString.createFromNative(I_NativeParamRef)");
        Traces.traceParam("I_NativeParamRef", new Integer(I_NativeParamRef).toString());

        Param cli_Param = new ParamString(I_NativeParamRef);
        if (cli_Param != null) {
            // Do not finalize native-created objects.
            cli_Param.dontFinalize();
            // Object already remembered from the NativeObject constructor...
            //  NativeObject.remember(cli_Param);
        }

        Traces.traceReturn("ParamString.createFromNative()");
    }
    private ParamString(int I_NativeParamRef) {
        super(I_NativeParamRef);
    }

    /** Destructor. */
    protected void finalize() throws Throwable {
        if (getbDoFinalize()) {
            __finalize(getNativeRef());
        }
        super.finalize();
    }
    private static final native void __finalize(int I_NativeParamRef);

    /** Destruction from native code.
        See createFromNative(). */
    protected static void deleteFromNative(int I_NativeParamRef) {
        Traces.traceMethod("ParamString.deleteFromNative(I_NativeParamRef)");
        Traces.traceParam("I_NativeParamRef", new Integer(I_NativeParamRef).toString());

        NativeObject cli_Param = NativeObject.getObject(I_NativeParamRef);
        if (cli_Param != null) {
            Traces.traceValue("cli_Param", cli_Param.toString());
            if (cli_Param instanceof ParamString) {
                NativeObject.forget(cli_Param);
            }
        }

        Traces.traceReturn("ParamString.deleteFromNative()");
    }

    /** String value accessor.
        @return The string value controlled by the parameter. */
    public final String getValue() {
        return new String(__getValue(this.getNativeRef()));
    }
    private static final native String __getValue(int I_NativeParamRef);
}
