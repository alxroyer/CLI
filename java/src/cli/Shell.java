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


/** Shell class.
    A shell basically runs a command line interface with the following arguments at the minimum:

        # a Cli object which defines a set of command lines and the execution for each of them;

        # an IODevice used to receive the input characters and to print out characters. */
public class Shell extends NativeObject {

    /** Constructor.
        @param CLI_Cli Cli structure the shell will run the text based interface for. */
    public Shell(Cli CLI_Cli) {
        super(__Shell(CLI_Cli.getNativeRef()));
    }
    private static final native int __Shell(int I_NativeCliRef);

    /** Destructor. */
    protected void finalize() throws Throwable {
        if (getbDoFinalize()) {
            __finalize(getNativeRef());
        }
        super.finalize();
    }
    private static final native void __finalize(int I_NativeShellRef);

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

    /** Main loop.
        Waits for characters from the input device, analyzes command lines, executes them...
        @param CLI_IODevice Basic input/output device to use for input, and all output streams if not already defined.
        @return true: success, false: failure. */
    public final boolean run(IODevice CLI_IODevice) {
        try {
            return __run(this.getNativeRef(), CLI_IODevice.getNativeRef());
        } catch (Exception e) {
            e.printStackTrace();
            System.err.println(e.toString());
            return false;
        }
    }
    private static final native boolean __run(int I_NativeShellRef, int I_NativeIODeviceRef);

}
