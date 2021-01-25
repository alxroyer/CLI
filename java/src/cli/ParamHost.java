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

package cli;


/** Host parameter element.
    Host parameter are basically IP addresses, URLs... */
public class ParamHost extends Param {

    /** Constructor.
        @param CLI_Help Help to be attached to the parameter. */
    public ParamHost(Help CLI_Help) {
        super(__ParamHost(CLI_Help.getNativeRef()));
    }
    private static final native long __ParamHost(long I64_NativeHelpRef);

    /** Creation from native code.
        Useful for cloned parameters created for a specific host value given in a command line.
        @param I64_NativeParamRef Native instance reference. */
    protected static void createFromNative(long I64_NativeParamRef) {
        Traces.trace(NativeTraces.CLASS, NativeTraces.begin("ParamHost.createFromNative(I64_NativeParamRef)"));
        Traces.trace(NativeTraces.CLASS, NativeTraces.param("I64_NativeParamRef", new Long(I64_NativeParamRef).toString()));

        NativeObject.createdFromNative(new ParamHost(I64_NativeParamRef));

        Traces.trace(NativeTraces.CLASS, NativeTraces.end("ParamHost.createFromNative()"));
    }
    private ParamHost(long I64_NativeParamRef) {
        super(I64_NativeParamRef);
    }

    /** Host value accessor.
        @return The host value controlled by the parameter. */
    public final String getValue() {
        return new String(__getValue(this.getNativeRef()));
    }
    private static final native String __getValue(long I64_NativeParamRef);

}
