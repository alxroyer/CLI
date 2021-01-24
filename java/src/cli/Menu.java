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


public abstract class Menu extends SyntaxNode
{
    protected Menu(int I_NativeRef) {
        super(I_NativeRef);
    }
    public Menu(String J_Help, Help CLI_Help) {
        super(__Menu(J_Help, CLI_Help.getNativeRef()));
    }
    private static final native int __Menu(String J_Help, int I_NativeHelpRef);

    protected void finalize() throws Throwable {
        // Caution!
        // When the native C++ inherited cli::Menu object has already been destroyed in Cli.__finalize(),
        // Do not call Menu.__finalize() here.
        if (! (this instanceof Cli)) {
            __finalize(this.getNativeRef());
        }
        super.finalize();
    }
    private static final native void __finalize(int I_NativeMenuRef);

    public abstract void populate();

    /** Command line execution. */
    public abstract boolean execute(CommandLine CLI_CmdLine);
    private final boolean __execute(int I_NativeCmdLineRef) {
        Traces.traceMethod("Menu.__execute(I_NativeCmdLineRef)");
        Traces.traceParam("I_NativeCmdLineRef", new Integer(I_NativeCmdLineRef).toString());

        boolean b_Res = false;
        try {
            NativeObject cli_CommandLine = NativeObject.getObject(I_NativeCmdLineRef);
            Traces.traceValue("cli_CommandLine", cli_CommandLine.toString());
            if (cli_CommandLine instanceof CommandLine) {
                b_Res = execute((CommandLine) cli_CommandLine);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        Traces.traceReturn("Menu.__execute()", new Boolean(b_Res).toString());
        return b_Res;
    }

    public void onExit() {
    }
    private final void __onExit() {
        Traces.traceMethod("Menu.__onExit()");
        onExit();
        Traces.traceReturn("Menu.__onExit()");
    }
}
