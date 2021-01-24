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


public class OutputDevice extends NativeObject
{
    protected OutputDevice(int I_NativeRef) {
        super(I_NativeRef);
    }

    public final OutputDevice put(String J_Text) {
        __print(getNativeRef(), J_Text);
        return this;
    }
    private static final native void __print(int I_NativeOutputDeviceRef, String J_Text);
    public final OutputDevice put(Integer J_Integer) {
        __print(getNativeRef(), J_Integer.intValue());
        return this;
    }
    private static final native void __print(int I_NativeOutputDeviceRef, int I_Integer);
    public final OutputDevice put(Float J_Float) {
        __print(getNativeRef(), J_Float.floatValue());
        return this;
    }
    private static final native void __print(int I_NativeOutputDeviceRef, float F_Float);
    public final OutputDevice put(Double J_Double) {
        __print(getNativeRef(), J_Double.doubleValue());
        return this;
    }
    private static final native void __print(int I_NativeOutputDeviceRef, double D_Double);
    public final OutputDevice endl() {
        __endl(getNativeRef());
        return this;
    }
    private static final native void __endl(int I_NativeOutputDeviceRef);

    //! @brief Null device.
    public static final OutputDevice getNullDevice() {
        return new OutputDevice(__getNullDevice());
    }
    private static final native int __getNullDevice();

    //! @brief Standard output device.
    public static final OutputDevice getStdOut() {
        return new OutputDevice(__getStdOut());
    }
    private static final native int __getStdOut();

    //! @brief Standard error device.
    public static final OutputDevice getStdErr() {
        return new OutputDevice(__getStdErr());
    }
    private static final native int __getStdErr();

}
