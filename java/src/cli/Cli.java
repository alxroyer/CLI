/*
    Copyright (c) 2006-2009, Alexis Royer

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

import java.util.Collection;


/** Command Line Interface class. */
public abstract class Cli extends Menu
{
    static {
        // Loading 'cli' library.
        System.loadLibrary("cli");
    }

    /** Does nothing but loading the native library. */
    public static void load() {
    }

    /** Constructor.
        @param J_Name   Name of the CLI.
        @param CLI_Help Help of the CLI. */
    public Cli(String J_Name, Help CLI_Help) {
        super(__Cli(J_Name, CLI_Help.getNativeRef()));
    }
    private static final native int __Cli(String J_Name, int I_NativeHelpRef);

    /** Destructor. */
    protected void finalize() throws Throwable {
        if (getbDoFinalize()) {
            __finalize(getNativeRef());
        }
        super.finalize();
    }
    private static final native void __finalize(int I_NativeCliRef);

    /** Find from name.
        @param J_CliList    Output CLI list.
        @param J_RegExp     Regular expression describing the CLI name searched.
        @return Number of CLI instances found. */
    public static final int findFromName(Collection<Cli> J_CliList, String J_RegExp) {
        int i_Count = 0;
        int[] ari_Refs = __findFromName(J_RegExp);
        for (int i=0; i<ari_Refs.length; i++) {
            NativeObject cli_Cli = NativeObject.getObject(ari_Refs[i]);
            if (cli_Cli != null) {
                J_CliList.add((Cli) cli_Cli);
                i_Count ++;
            }
        }
        return i_Count;
    }
    private static final native int[] __findFromName(String J_RegExp);

    /** Name accessor.
        @return CLI name. */
    public final String getName() {
        return __getName(this.getNativeRef());
    }
    private static final native String __getName(int I_NativeRef);

    /** Menu addition.
        @param CLI_Menu Menu to add to the CLI.
        @return The menu itself. */
    public final Menu addMenu(Menu CLI_Menu) {
        if (__addMenu(this.getNativeRef(), CLI_Menu.getNativeRef())) {
            return CLI_Menu;
        }
        return null;
    }
    private static final native boolean __addMenu(int I_NativeCliRef, int I_NativeMenuRef);

    /** Configuration menu enabling.
        @param B_Enable Enable or disable the configuration menu. */
    public final void enableConfigMenu(boolean B_Enable) {
        __enableConfigMenu(this.getNativeRef(), B_Enable);
    }
    private static final native boolean __enableConfigMenu(int I_NativeCliRef, boolean B_Enable);
}
