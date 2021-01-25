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
SET NATIVE_DIR=..\..\..\java\native
SET OUT_DIR=Release
SET JDK_PATH=D:\APP\Oracle\Java\JDK-7u40x86\bin

%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Cli
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.CommandLine
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Console
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Element
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Endl
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ExecutionContext
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ExecutionResult
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Help
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.InputFileDevice
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.IODevice
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.IOMux
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Keyword
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Menu
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.MenuRef
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.NativeObject
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.NativeTraces
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.NonBlockingIODevice
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.OutputDevice
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.OutputFileDevice
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Param
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ParamFloat
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ParamHost
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ParamInt
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ParamString
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ResourceString
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Shell
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.SingleCommand
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.StringDevice
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.SyntaxNode
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.SyntaxRef
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.SyntaxTag
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.TelnetConnection
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.TelnetServer
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.TraceClass
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.Traces
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.Choice
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.Float
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.Int
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.Less
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.Line
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.More
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.Password
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.UI
%JDK_PATH%\javah -classpath %OUT_DIR% -d %NATIVE_DIR% cli.ui.YesNo
