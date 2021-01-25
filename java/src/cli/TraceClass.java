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


/** Trace class description. */
public class TraceClass extends NativeObject {

    /** Native JNI trace class singleton.
        @return Native JNI trace class instance. */
    public static final TraceClass getJniTraceClass() {
        if (m_cliJniTraceClass == null) {
            m_cliJniTraceClass = new TraceClass(__getJniTraceClass());
            m_cliJniTraceClass.dontFinalize(); // Do not try to finalize native JNI trace class.
        }
        return m_cliJniTraceClass;
    }
    private static TraceClass m_cliJniTraceClass = null;
    private static final native long __getJniTraceClass();

    /** Copy constructor.
        @param CLI_Class Source class object. */
    public TraceClass(TraceClass CLI_Class) {
        super(__TraceClass(CLI_Class.getClassName(), CLI_Class.getHelp().getNativeRef()));
    }
    /** Constructor with a trace class name and a corresponding help given.
        @param STR_ClassName Trace class name.
        @param CLI_Help Corresponding help objet. */
    public TraceClass(String STR_ClassName, Help CLI_Help) {
        super(__TraceClass(STR_ClassName, CLI_Help.getNativeRef()));
    }
    private static final native long __TraceClass(String STR_ClassName, long I64_NativeHelpRef);

    /** By native reference constructor.
        Almost for getJniTraceClass() singleton.
        @param I64_NativeTraceClassRef Native trace class reference. */
    private TraceClass(long I64_NativeTraceClassRef) {
        super(I64_NativeTraceClassRef);
    }

    /** Class name accessor.
        @return Class name. null if an error occurred. */
    public String getClassName() {
        return __getClassName(getNativeRef());
    }
    private static final native String __getClassName(long I64_NativeTraceClassRef);

    /** Help object accessor.
        @return Help string. null if an error occurred. */
    public Help getHelp() {
        Help cli_Help = (Help) NativeObject.getObject(__getHelp(this.getNativeRef()));
        // This trace class instance will not be deleted by the native code.
        NativeObject.forget(cli_Help);
        return cli_Help;
    }
    private static final native long __getHelp(long I64_NativeTraceClassRef);

    /** Direct help string accessor.
        Facility provided in order to shorten .getHelp().getHelp(cli.Help.LANG_EN) calls into .getHelp(cli.Help.LANG_EN).
        @param E_Lang Language identifier.
        @return The help string defined for the given language. */
    public String getHelp(int E_Lang) {
        return getHelp().getHelp(E_Lang);
    }
}
