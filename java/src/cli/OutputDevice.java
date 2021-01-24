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


/** Generic output device. */
public class OutputDevice extends NativeObject
{
    /** Java output stream attached to a CLI output device. */
    public static class OutputStream extends java.io.OutputStream {
        /** Constructor. */
        public OutputStream(OutputDevice CLI_OutputDevice) {
            m_cliOutputDevice = CLI_OutputDevice;
        }

        /** Output handler. */
        public void write(int b) {
            if (m_cliOutputDevice != null) {
                byte[] arb_Bytes = { (byte) b };
                m_cliOutputDevice.put(new String(arb_Bytes));
            }
        }

        /** CLI output device attached to this java output stream. */
        private final OutputDevice m_cliOutputDevice;
    }

    /** Constructor.
        @param I_NativeRef Native instance reference. */
    protected OutputDevice(int I_NativeRef) {
        super(I_NativeRef);
    }

    /** Pushes characters to the output device.
        @param J_Text String to be displayed by the device.
        @return The output device itself. */
    public final OutputDevice put(String J_Text) {
        __print(getNativeRef(), J_Text);
        return this;
    }
    private static final native void __print(int I_NativeOutputDeviceRef, String J_Text);
    /** Pushes an integer value to be displayed by the output device.
        @param J_Integer Integer value to be displayed by the device.
        @return The output device itself. */
    public final OutputDevice put(Integer J_Integer) {
        __print(getNativeRef(), J_Integer.intValue());
        return this;
    }
    private static final native void __print(int I_NativeOutputDeviceRef, int I_Integer);
    /** Pushes a float value to be displayed by the output device.
        @param J_Float Float value to be displayed by the device.
        @return The output device itself. */
    public final OutputDevice put(Float J_Float) {
        __print(getNativeRef(), J_Float.floatValue());
        return this;
    }
    private static final native void __print(int I_NativeOutputDeviceRef, float F_Float);
    /** Pushes a double value to be displayed by the output device.
        @param J_Double Double value to be displayed by the device.
        @return The output device itself. */
    public final OutputDevice put(Double J_Double) {
        __print(getNativeRef(), J_Double.doubleValue());
        return this;
    }
    private static final native void __print(int I_NativeOutputDeviceRef, double D_Double);
    /** Pushes an end of line to be displayed by the output device.
        @return The output device itself. */
    public final OutputDevice endl() {
        __endl(getNativeRef());
        return this;
    }
    private static final native void __endl(int I_NativeOutputDeviceRef);

    /** Null device singleton.
        @return The null output device. */
    public static final OutputDevice getNullDevice() {
        if (m_cliNullDevice == null) {
            m_cliNullDevice = new OutputDevice(__getNullDevice());
        }
        return m_cliNullDevice;
    }
    private static final native int __getNullDevice();
    private static OutputDevice m_cliNullDevice = null;

    /** Standard output device singleton.
        @return The standard output device. */
    public static final OutputDevice getStdOut() {
        if (m_cliStdOut == null) {
            m_cliStdOut = new OutputDevice(__getStdOut());
        }
        return m_cliStdOut;
    }
    private static final native int __getStdOut();
    private static OutputDevice m_cliStdOut = null;

    /** Standard error device singleton.
        @return The standard error device. */
    public static final OutputDevice getStdErr() {
        if (m_cliStdErr == null) {
            m_cliStdErr = new OutputDevice(__getStdErr());
        }
        return m_cliStdErr;
    }
    private static final native int __getStdErr();
    private static OutputDevice m_cliStdErr = null;

}
