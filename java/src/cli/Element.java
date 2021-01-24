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


public abstract class Element extends NativeObject
{
    protected Element(int I_NativeRef) {
        super(I_NativeRef);
    }

    public final String getKeyword() {
        return __getKeyword(this.getNativeRef());
    }
    private static final native String __getKeyword(int I_NativeElementRef);

    public final Help getHelp() {
        Help cli_Help = new Help();
        if (cli_Help != null) {
            for (int e_Lang=0; e_Lang<Help.LANG_COUNT; e_Lang++) {
                cli_Help.addHelp(e_Lang, __getHelp(this.getNativeRef(), e_Lang));
            }
        }
        return cli_Help;
    }
    private static final native String __getHelp(int I_NativeElementRef, int E_LangId);

    public final OutputDevice getOutputStream() {
        return (OutputDevice) NativeObject.getObject(__getOutputStream(this.getNativeRef()));
    }
    private static final native int __getOutputStream(int I_NativeElementRef);

    public final OutputDevice getErrorStream() {
        return (OutputDevice) NativeObject.getObject(__getErrorStream(this.getNativeRef()));
    }
    private static final native int __getErrorStream(int I_NativeElementRef);
}
