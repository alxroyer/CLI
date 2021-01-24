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

package cli;


/** Shell class.
    A shell basically runs a command line interface with the following arguments at the minimum:

        # a Cli object which defines a set of command lines and the execution for each of them;

        # an IODevice used to receive the input characters and to print out characters. */
public class Shell extends NativeObject {

    /** All streams constant. */
    public static final int ALL_STREAMS = -1;
    /** Welcome stream constant. Useful for bye too. */
    public static final int WELCOME_STREAM = 0;
    /** Prompt stream constant. */
    public static final int PROMPT_STREAM = 1;
    /** Echo stream constant. */
    public static final int ECHO_STREAM = 2;
    /** Output stream constant. */
    public static final int OUTPUT_STREAM = 3;
    /** Error stream constant. */
    public static final int ERROR_STREAM = 4;
    /** Number of stream types. */
    public static final int STREAM_TYPES_COUNT = 5;

    /** Constructor.
        @param CLI_Cli Cli structure the shell will run the text based interface for. */
    public Shell(Cli CLI_Cli) {
        super(__Shell(CLI_Cli.getNativeRef()));
    }
    private static final native int __Shell(int I_NativeCliRef);

    /** Destructor. */
    protected void finalize() throws Throwable {
        if (getbDoFinalize()) {
            __finalize(this.getNativeRef());
            dontFinalize(); // finalize once.
        }
        super.finalize();
    }
    private static final native void __finalize(int I_NativeShellRef);

    /** Cli accessor.
        @return The CLI instance the shell is running with. */
    public final Cli getCli() {
        return (Cli) NativeObject.getObject(__getCli(this.getNativeRef()));
    }
    private static final native int __getCli(int I_NativeShellRef);

    /** Input stream accessor.
        @return The input stream attached to the shell for its execution. */
    public final IODevice getInput() {
        return (IODevice) NativeObject.getObject(__getInput(this.getNativeRef()));
    }
    private static final native int __getInput(int I_NativeShellRef);

    /** Output stream accessor.
        @param E_StreamType Output stream identifier.
        @return The required stream. null if an error occured. */
    public final OutputDevice getStream(int E_StreamType) {
        return (OutputDevice) NativeObject.getObject(__getStream(this.getNativeRef(), E_StreamType));
    }
    private static final native int __getStream(int I_NativeShellRef, int E_StreamType);

    /** Output stream positionning.
        @param E_StreamType Output stream identifier.
        @param CLI_Stream Stream reference.
        @return true: success, false: failure. */
    public final boolean setStream(int E_StreamType, OutputDevice CLI_Stream) {
        return __setStream(this.getNativeRef(), E_StreamType, CLI_Stream.getNativeRef());
    }
    private static final native boolean __setStream(int I_NativeShellRef, int E_StreamType, int I_NativeDeviceRef);

    /** Enabled/disabled stream status accessor.
        @param E_StreamType Output stream identifier.
        @return true: the stream is enabled, false: the stream is disabled. */
    public final boolean streamEnabled(int E_StreamType) {
        return __streamEnabled(this.getNativeRef(), E_StreamType);
    }
    private static final native boolean __streamEnabled(int I_NativeShellRef, int E_StreamType);

    /** Enabling/disabling output streams.
        @param E_StreamType Output stream identifier.
        @param B_Enable Enable flag.
        @return true: success, false: failure. */
    public final boolean enableStream(int E_StreamType, boolean B_Enable) {
        return __enableStream(this.getNativeRef(), E_StreamType, B_Enable);
    }
    private static final native boolean __enableStream(int I_NativeShellRef, int E_StreamType, boolean B_Enable);

    /** Welcome message setting.
        @param CLI_WelcomeMessage Welcome message. When an empty string is given, the default welcome message is restored. */
    public final void setWelcomeMessage(ResourceString CLI_WelcomeMessage) {
        __setWelcomeMessage(this.getNativeRef(), CLI_WelcomeMessage.getNativeRef());
    }
    private static final native void __setWelcomeMessage(int I_NativeShellRef, int I_NativeWelcomeMessageRef);

    /** Bye message setting.
        @param CLI_ByeMessage Bye message. When an empty string is given, the default bye message is restored. */
    public final void setByeMessage(ResourceString CLI_ByeMessage) {
        __setByeMessage(this.getNativeRef(), CLI_ByeMessage.getNativeRef());
    }
    private static final native void __setByeMessage(int I_NativeShellRef, int I_NativeByeMessageRef);

    /** Prompt message positionning.
        @param CLI_Prompt Prompt string. When an empty string is given, the default prompt (depending on the current menu) is restored. */
    public final void setPrompt(ResourceString CLI_Prompt) {
        __setPrompt(this.getNativeRef(), CLI_Prompt.getNativeRef());
    }
    private static final native void __setPrompt(int I_NativeShellRef, int I_NativePromptRef);

    /** Error formatting.
        @param CLI_LocationPrefix   Very first error prefix string, before the location. No error prefixing when the string given is empty.
        @param CLI_ErrorPrefix      Error prefix string. No error prefixing when the string given is empty.
        @param CLI_ErrorSuffix      Error suffix string. No error suffixing when the string given is empty. */
    public final void setErrorFormatting(ResourceString CLI_LocationPrefix, ResourceString CLI_ErrorPrefix, ResourceString CLI_ErrorSuffix) {
        __setErrorFormatting(this.getNativeRef(), CLI_LocationPrefix.getNativeRef(), CLI_ErrorPrefix.getNativeRef(), CLI_ErrorSuffix.getNativeRef());
    }
    private static final native void __setErrorFormatting(int I_NativeShellRef, int I_NativeLocationPrefixRef, int I_NativeErrorPrefixRef, int I_NativeErrorSuffixRef);

    /** Language setting.
        @param E_Lang New value. */
    public final void setLang(int E_Lang) {
        __setLang(this.getNativeRef(), E_Lang);
    }
    private static final native void __setLang(int I_NativeShellRef, int E_Lang);

    /** Language access.
        @return The language currently set. */
    public final int getLang() {
        return __getLang(this.getNativeRef());
    }
    private static final native int __getLang(int I_NativeShellRef);

    /** Beep configuration setting.
        @param B_Enable New value. */
    public final void setBeep(boolean B_Enable) {
        __setBeep(this.getNativeRef(), B_Enable);
    }
    private static final native void __setBeep(int I_NativeShellRef, boolean B_Enable);

    /** Beep configuration access.
        @return The current beep configuration. true if enabled, false otherwise. */
    public final boolean getBeep() {
        return __getBeep(this.getNativeRef());
    }
    private static final native boolean __getBeep(int I_NativeShellRef);

    /** Main loop.
        Waits for characters from the input device, analyzes command lines, executes them...
        @param CLI_IODevice Basic input/output device to use for input, and all output streams if not already defined.
        @return true: success, false: failure. */
    public final boolean run(IODevice CLI_IODevice) {
        try {
            return __run(this.getNativeRef(), CLI_IODevice.getNativeRef());
        } catch (Exception e) {
            getStream(ERROR_STREAM).printStackTrace(e);
            System.err.println(e.toString());
            return false;
        }
    }
    private static final native boolean __run(int I_NativeShellRef, int I_NativeIODeviceRef);

    /** Tells whether this shell is running or not.
        @return The running status. */
    public final boolean isRunning() {
        return __isRunning(this.getNativeRef());
    }
    private static final native boolean __isRunning(int I_NativeShellRef);

    /** Help margin accessor.
        @return Number of spaces for the help margin. */
    public final int getHelpMargin() {
        return __getHelpMargin(this.getNativeRef());
    }
    private static final native int __getHelpMargin(int I_NativeShellRef);

    /** Help offset accessor.
        @return Number of spaces for the help offset. */
    public final int getHelpOffset() {
        return __getHelpOffset(this.getNativeRef());
    }
    private static final native int __getHelpOffset(int I_NativeShellRef);

    /** Exits the current menu. */
    public final void exitMenu() {
        __exitMenu(this.getNativeRef());
    }
    private static final native void __exitMenu(int I_NativeShellRef);

    /** Terminates the shell.
        Caution! Not thread safe. Call quitThreadSafe() to do so. */
    public final void quit() {
        __quit(this.getNativeRef());
    }
    private static final native void __quit(int I_NativeShellRef);

    /** Terminates the shell.
        The shell will actually quit as soon as the input device return a character. */
    public final void quitThreadSafe() {
        __quitThreadSafe(this.getNativeRef());
    }
    private static final native void __quitThreadSafe(int I_NativeShellRef);

    /** Displays help depending on the context of the current line. */
    public final void displayHelp() {
        __displayHelp(this.getNativeRef());
    }
    private static final native void __displayHelp(int I_NativeShellRef);

    /** Prints the working menu. */
    public final void printWorkingMenu() {
        __printWorkingMenu(this.getNativeRef());
    }
    private static final native void __printWorkingMenu(int I_NativeShellRef);

}
