REM Copyright (c) 2006-2018, Alexis Royer, http://alexis.royer.free.fr/CLI
REM
REM All rights reserved.
REM
REM Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
REM
REM     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
REM     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
REM       and/or other materials provided with the distribution.
REM     * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
REM       without specific prior written permission.
REM
REM THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
REM "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
REM LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
REM A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
REM CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
REM EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
REM PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
REM PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
REM LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
REM NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
REM SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

SET SRC_DIR=..\..\..\java\src
SET OUT_DIR=Release
SET JDK_PATH=D:\APP\Oracle\Java\JDK-7u40x86\bin

%JDK_PATH%\javac -Xlint:deprecation -encoding utf-8 -d %OUT_DIR% -classpath %OUT_DIR% %SRC_DIR%\cli\*.java %SRC_DIR%\cli\ui\*.java
cd %OUT_DIR%
jar -cf cli.jar cli/*.class
cd ..
