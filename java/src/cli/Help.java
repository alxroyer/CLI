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


public final class Help extends NativeObject
{
    public Help() {
        super(__Help());
    }
    public Help(Help CLI_Help) {
        super(__Help(CLI_Help.getNativeRef()));
    }
    private static final native int __Help();
    private static final native int __Help(int I_NativeHelpRef);

    protected void finalize() throws Throwable {
        __finalize(getNativeRef());
        super.finalize();
    }
    private static final native void __finalize(int I_NativeHelpRef);

    public static final int LANG_EN = 0;
    public static final int LANG_FR = 1;
    public static final int LANG_COUNT = 2;

    public final Help addHelp(int E_Lang, String J_Help) {
        __addHelp(this.getNativeRef(), E_Lang, J_Help);
        return this;
    }
    private static final native boolean __addHelp(int I_NativeHelpRef, int E_Lang, String J_Help);

    public final boolean hasHelp(int E_Lang) {
        return __hasHelp(this.getNativeRef(), E_Lang);
    }
    private static final native boolean __hasHelp(int I_NativeHelpRef, int E_Lang);

    public final String getHelp(int E_Lang) {
        return __getHelp(this.getNativeRef(), E_Lang);
    }
    private static final native String __getHelp(int I_NativeHelpRef, int E_Lang);
}
