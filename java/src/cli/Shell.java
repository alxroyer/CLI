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


public class Shell extends NativeObject
{
    public Shell(Cli CLI_Cli) {
        super(__Shell(CLI_Cli.getNativeRef()));
    }
    private static final native int __Shell(int I_NativeCliRef);

    protected void finalize() throws Throwable {
        __finalize(getNativeRef());
        super.finalize();
    }
    private static final native void __finalize(int I_NativeShellRef);

    public static final int ALL_STREAMS = -1;       //!< All streams.
    public static final int WELCOME_STREAM = 0;     //!< Welcome stream. Useful for bye too.
    public static final int PROMPT_STREAM = 1;      //!< Prompt stream.
    public static final int ECHO_STREAM = 2;        //!< Echo stream.
    public static final int OUTPUT_STREAM = 3;      //!< Output stream.
    public static final int ERROR_STREAM = 4;       //!< Error stream.
    public static final int STREAM_TYPES_COUNT = 5; //!< Number of streams.

    //! @brief Output stream access.
    //! @param E_StreamType Output stream identifier.
    public final OutputDevice getStream(int E_StreamType) {
        return (OutputDevice) NativeObject.getObject(__getStream(this.getNativeRef(), E_StreamType));
    }
    private static final native int __getStream(int I_NativeShellRef, int E_StreamType);

    //! @brief Output stream positionning.
    //! @param E_StreamType Output stream identifier.
    //! @param CLI_Stream Stream reference.
    public final boolean setStream(int E_StreamType, OutputDevice CLI_Stream) {
        return __setStream(this.getNativeRef(), E_StreamType, CLI_Stream.getNativeRef());
    }
    private static final native boolean __setStream(int I_NativeShellRef, int E_StreamType, int I_NativeDeviceRef);

    //! @brief Enabled/disabled stream accessor.
    //! @param E_StreamType Output stream identifier.
    public final boolean streamEnabled(int E_StreamType) {
        return __streamEnabled(this.getNativeRef(), E_StreamType);
    }
    private static final native boolean __streamEnabled(int I_NativeShellRef, int E_StreamType);

    //! @brief Enable/disable stream.
    //! @param E_StreamType Output stream identifier.
    //! @param B_Enable Enable flag.
    public final boolean enableStream(int E_StreamType, boolean B_Enable) {
        return __enableStream(this.getNativeRef(), E_StreamType, B_Enable);
    }
    private static final native boolean __enableStream(int I_NativeShellRef, int E_StreamType, boolean B_Enable);

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
