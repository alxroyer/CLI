/*
    Copyright (c) 2006-2010, Alexis Royer, http://alexis.royer.free.fr/CLI

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


/** Generic input/output device. */
public class IODevice extends OutputDevice
{
    /** Constructor.
        @param I_NativeRef Native instance reference. */
    protected IODevice(int I_NativeRef) {
        super(I_NativeRef);
    }

    /** Input key capture. */
    public int getKey() {
        return __getKey(this.getNativeRef());
    }
    private static final native int __getKey(int I_NativeDeviceRef);

    /** Location accessor.
        @return The location as a resource string. */
    public ResourceString getLocation() {
        ResourceString cli_Location = (ResourceString) NativeObject.getObject(__getLocation(this.getNativeRef()));
        NativeObject.forget(cli_Location);
        return cli_Location;
    }
    private static final native int __getLocation(int I_NativeDeviceRef);

    /** Null device singleton.
        @return The null output device. */
    public static final IODevice getNullDevice() {
        if (m_cliNullDevice == null) {
            m_cliNullDevice = new IODevice(__getNullDevice());
        }
        return m_cliNullDevice;
    }
    private static final native int __getNullDevice();
    private static IODevice m_cliNullDevice = null;

    /** Standard input device singleton.
        @return The standard input device. */
    public static final IODevice getStdIn() {
        if (m_cliStdIn == null) {
            m_cliStdIn = new IODevice(__getStdIn());
        }
        return m_cliStdIn;
    }
    private static final native int __getStdIn();
    private static IODevice m_cliStdIn = null;
}
