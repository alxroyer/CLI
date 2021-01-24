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


/** Input/output device multiplexer.
    Manages a list of input devices, and output streams can be specialized. */
public class IOMux extends IODevice
{
    /** Default constructor. */
    public IOMux() {
        super(__IOMux());
    }
    private static final native int __IOMux();

    /** Destructor. */
    protected void finalize() throws Throwable {
        if (getbDoFinalize()) {
            __finalize(this.getNativeRef());
            dontFinalize(); // finalize once.
        }
        super.finalize();
    }
    private static final native void __finalize(int I_NativeIOMuxRef);

    /** Input device addition.
        @param CLI_Input Input device to add.
        @return true if the input device has been correctly added, false otherwise. */
    public boolean addInput(IODevice CLI_Input) {
        return __addInput(this.getNativeRef(), CLI_Input.getNativeRef());
    }
    private static final native boolean __addInput(int I_NativeIOMuxRef, int I_NativeInputRef);

    /** Output device setting.
        @param E_StreamType Stream type selector. Must be one of
                    cli.Shell.ALL_STREAMS,
                    cli.Shell.WELCOME_STREAM,
                    cli.Shell.PROMPT_STREAM,
                    cli.Shell.ECHO_STREAM,
                    cli.Shell.OUTPUT_STREAM or
                    cli.Shell.ERROR_STREAM.
        @param CLI_Stream Output device to be set.
        @return true if the output device has been correctly set, false otherwise. */
    public boolean setOutput(int E_StreamType, OutputDevice CLI_Stream) {
        return __setOutput(this.getNativeRef(), E_StreamType, CLI_Stream.getNativeRef());
    }
    private static final native boolean __setOutput(int I_NativeIOMuxRef, int E_StreamType, int I_NativeStreamRef);
}
