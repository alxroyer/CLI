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

package cli.test;


public class TestNative {

    public static void main(String ARJ_Args[]) {
        try {
            testJniTraceClass();
            testJniTracesOn();
            testCliShell();
            testSyntaxNodes();
            testIODevices();
            testUI();

            System.exit(0);
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(-1);
        }
    }

    private static void testJniTraceClass() {
        System.out.println("[testJniTraceClass()]");
            // libclijava.mak/jni.mak:
            // JAVA_FILES += $(SRC_DIR)/cli/NativeObject.java => 'Cli.load();' temporarily changed in 'System.loadLibrary("cli");' directly in order not to depend on $(SRC_DIR)/cli/Cli.java
            // JAVA_FILES += $(SRC_DIR)/cli/NativeTraces.java
            // JAVA_FILES += $(SRC_DIR)/cli/TraceClass.java
            // JAVA_FILES += $(SRC_DIR)/cli/Help.java
            // JAVA_FILES += $(SRC_DIR)/cli/ResourceString.java
            // native.mak:
            // CPP_FILES += $(SRC_DIR)/NativeObject.cpp
            // CPP_FILES += $(SRC_DIR)/NativeExec.cpp
            // CPP_FILES += $(SRC_DIR)/NativeTraces.cpp
            // CPP_FILES += $(SRC_DIR)/cli_NativeObject.cpp
            // CPP_FILES += $(SRC_DIR)/cli_NativeTraces.cpp
            // CPP_FILES += $(SRC_DIR)/cli_TraceClass.cpp
            // CPP_FILES += $(SRC_DIR)/cli_Help.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ResourceString.cpp

        cli.TraceClass cli_JniTraceClass = cli.TraceClass.getJniTraceClass();
        System.out.println("cli_JniTraceClass.getClassName() = " + cli_JniTraceClass.getClassName());
        System.out.println("cli_JniTraceClass.getHelp() = " + cli_JniTraceClass.getHelp());
        System.out.println("cli_JniTraceClass.getHelp(LANG_EN) = " + cli_JniTraceClass.getHelp(cli.Help.LANG_EN));
        System.out.println("cli_JniTraceClass.getHelp(LANG_FR) = " + cli_JniTraceClass.getHelp(cli.Help.LANG_FR));
    }

    private static void testJniTracesOn() {
        System.out.println("[testJniTracesOn()]");
            // libclijava.mak/jni.mak:
            // JAVA_FILES += $(SRC_DIR)/cli/Traces.java
            // JAVA_FILES += $(SRC_DIR)/cli/OutputDevice.java
            // JAVA_FILES += $(SRC_DIR)/cli/IODevice.java
            // native.mak:
            // CPP_FILES += $(SRC_DIR)/cli_Traces.cpp
            // CPP_FILES += $(SRC_DIR)/cli_OutputDevice.cpp
            // CPP_FILES += $(SRC_DIR)/cli_IODevice.cpp

        cli.Traces.setFilter(cli.TraceClass.getJniTraceClass(), true);
        cli.Traces.trace(cli.NativeTraces.CLASS, cli.NativeTraces.begin("foo()"));
        cli.Traces.trace(cli.NativeTraces.CLASS, cli.NativeTraces.param("param", "value"));
        cli.Traces.trace(cli.NativeTraces.CLASS, cli.NativeTraces.value("var", "value"));
        cli.Traces.trace(cli.NativeTraces.CLASS, cli.NativeTraces.end("foo()"));
        cli.Traces.trace(cli.NativeTraces.CLASS, cli.NativeTraces.end("foo()", "result"));
    }

    static class TestNativeCli extends cli.Cli {
        public TestNativeCli() {
            super("TestNative", new cli.Help());
        }
        public void populate() {
        }
        public boolean execute(cli.CommandLine CLI_CmdLine) {
            return false;
        }
    }
    private static void testCliShell() {
        System.out.println("[testCliShell()]");
            // libclijava.mak/jni.mak:
            // JAVA_FILES += $(SRC_DIR)/cli/NativeObject.java => 'Cli.load();' call restored
            // JAVA_FILES += $(SRC_DIR)/cli/Cli.java
            // JAVA_FILES += $(SRC_DIR)/cli/Menu.java
            // JAVA_FILES += $(SRC_DIR)/cli/SyntaxNode.java
            // JAVA_FILES += $(SRC_DIR)/cli/Element.java
            // JAVA_FILES += $(SRC_DIR)/cli/CommandLine.java
            // JAVA_FILES += $(SRC_DIR)/cli/Shell.java
            // JAVA_FILES += $(SRC_DIR)/cli/ExecutionContext.java
            // JAVA_FILES += $(SRC_DIR)/cli/ExecutionResult.java
            // native.mak:
            // CPP_FILES += $(SRC_DIR)/cli_Cli.cpp
            // CPP_FILES += $(SRC_DIR)/cli_Menu.cpp
            // CPP_FILES += $(SRC_DIR)/NativeMenu.cpp
            // CPP_FILES += $(SRC_DIR)/cli_SyntaxNode.cpp
            // CPP_FILES += $(SRC_DIR)/cli_Element.cpp
            // CPP_FILES += $(SRC_DIR)/cli_CommandLine.cpp
            // CPP_FILES += $(SRC_DIR)/cli_Shell.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ExecutionContext.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ExecutionResult.cpp

        cli.Cli cli_Cli = new TestNativeCli();
        cli.Shell cli_Shell = new cli.Shell(cli_Cli);
        cli_Cli.enableConfigMenu(true);
    }

    private static void testSyntaxNodes() {
        System.out.println("[testSyntaxNodes()]");
            // libclijava.mak/jni.mak:
            // JAVA_FILES += $(SRC_DIR)/cli/Keyword.java
            // JAVA_FILES += $(SRC_DIR)/cli/Endl.java
            // JAVA_FILES += $(SRC_DIR)/cli/MenuRef.java
            // JAVA_FILES += $(SRC_DIR)/cli/Param.java
            // JAVA_FILES += $(SRC_DIR)/cli/ParamInt.java
            // JAVA_FILES += $(SRC_DIR)/cli/ParamFloat.java
            // JAVA_FILES += $(SRC_DIR)/cli/ParamString.java
            // JAVA_FILES += $(SRC_DIR)/cli/ParamHost.java
            // JAVA_FILES += $(SRC_DIR)/cli/SyntaxTag.java
            // JAVA_FILES += $(SRC_DIR)/cli/SyntaxRef.java
            // native.mak:
            // CPP_FILES += $(SRC_DIR)/cli_Keyword.cpp
            // CPP_FILES += $(SRC_DIR)/cli_Endl.cpp
            // CPP_FILES += $(SRC_DIR)/cli_MenuRef.cpp
            // CPP_FILES += $(SRC_DIR)/cli_Param.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ParamInt.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ParamFloat.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ParamString.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ParamHost.cpp
            // CPP_FILES += $(SRC_DIR)/cli_SyntaxTag.cpp
            // CPP_FILES += $(SRC_DIR)/cli_SyntaxRef.cpp

        cli.Cli cli_Cli = new TestNativeCli();
        cli.Keyword cli_ParamKeyword = (cli.Keyword) cli_Cli.addElement(new cli.Keyword("param", new cli.Help()));
            cli.SyntaxTag cli_Tag = (cli.SyntaxTag) cli_ParamKeyword.addElement(new cli.SyntaxTag(false)); // Not hollow.
                cli.Keyword cli_IntKeyword = (cli.Keyword) cli_Tag.addElement(new cli.Keyword("int", new cli.Help()));
                    cli.Param cli_IntParam = (cli.Param) cli_IntKeyword.addElement(new cli.ParamInt(new cli.Help()));
                        cli_IntParam.addElement(new cli.Endl(new cli.Help()));
                        cli_IntParam.addElement(new cli.SyntaxRef(cli_Tag));
                cli.Keyword cli_FloatKeyword = (cli.Keyword) cli_Tag.addElement(new cli.Keyword("float", new cli.Help()));
                    cli.Param cli_FloatParam = (cli.Param) cli_FloatKeyword.addElement(new cli.ParamFloat(new cli.Help()));
                        cli_FloatParam.addElement(new cli.Endl(new cli.Help()));
                        cli_FloatParam.addElement(new cli.SyntaxRef(cli_Tag));
                cli.Keyword cli_StringKeyword = (cli.Keyword) cli_Tag.addElement(new cli.Keyword("string", new cli.Help()));
                    cli.Param cli_StringParam = (cli.Param) cli_StringKeyword.addElement(new cli.ParamString(new cli.Help()));
                        cli_StringParam.addElement(new cli.Endl(new cli.Help()));
                        cli_StringParam.addElement(new cli.SyntaxRef(cli_Tag));
                cli.Keyword cli_HostKeyword = (cli.Keyword) cli_Tag.addElement(new cli.Keyword("host", new cli.Help()));
                    cli.Param cli_HostParam = (cli.Param) cli_HostKeyword.addElement(new cli.ParamHost(new cli.Help()));
                        cli_HostParam.addElement(new cli.Endl(new cli.Help()));
                        cli_HostParam.addElement(new cli.SyntaxRef(cli_Tag));
    }

    static class TestNativeTelnetServer extends cli.TelnetServer {
        public TestNativeTelnetServer() {
            super(10, 9012, cli.Help.LANG_EN);
        }
        protected cli.ExecutionContext.Interface onNewConnection(cli.TelnetConnection CLI_NewConnection) {
            return null;
        }
        protected void onCloseConnection(cli.TelnetConnection CLI_ConnectionClosed, cli.ExecutionContext.Interface CLI_Context) {
        }
    }
    private static void testIODevices() {
        System.out.println("[testIODevices()]");
            // libclijava.mak/jni.mak:
            // JAVA_FILES += $(SRC_DIR)/cli/Console.java
            // JAVA_FILES += $(SRC_DIR)/cli/StringDevice.java
            // JAVA_FILES += $(SRC_DIR)/cli/SingleCommand.java
            // JAVA_FILES += $(SRC_DIR)/cli/OutputFileDevice.java
            // JAVA_FILES += $(SRC_DIR)/cli/InputFileDevice.java
            // JAVA_FILES += $(SRC_DIR)/cli/IOMux.java
            // JAVA_FILES += $(SRC_DIR)/cli/NonBlockIODevice.java
            // JAVA_FILES += $(SRC_DIR)/cli/TelnetConnection.java
            // JAVA_FILES += $(SRC_DIR)/cli/TelnetServer.java
            // native.mak:
            // CPP_FILES += $(SRC_DIR)/cli_OutputDevice_ScreenInfo.cpp
            // CPP_FILES += $(SRC_DIR)/cli_Console.cpp
            // CPP_FILES += $(SRC_DIR)/cli_StringDevice.cpp
            // CPP_FILES += $(SRC_DIR)/cli_SingleCommand.cpp
            // CPP_FILES += $(SRC_DIR)/cli_OutputFileDevice.cpp
            // CPP_FILES += $(SRC_DIR)/cli_InputFileDevice.cpp
            // CPP_FILES += $(SRC_DIR)/cli_IOMux.cpp
            // CPP_FILES += $(SRC_DIR)/cli_NonBlockingIODevice.cpp
            // CPP_FILES += $(SRC_DIR)/cli_TelnetConnection.cpp
            // CPP_FILES += $(SRC_DIR)/cli_TelnetServer.cpp

        cli.OutputDevice.getNullDevice().put("Hello cli.OutputDevice.getNullDevice()").endl();
        cli.OutputDevice.getStdOut().put("Hello cli.OutputDevice.getStdOut()").endl();
        cli.OutputDevice.getStdErr().put("Hello cli.OutputDevice.getStdErr()").endl();

        cli.Console cli_Console = new cli.Console();
        cli_Console.put("Hello cli.Console").endl(); // Might not work until no OpenUp() call has been done.
        System.out.println("cli_Console.getScreenInfo().getWidth() = " + cli_Console.getScreenInfo().getWidth());
        System.out.println("cli_Console.getScreenInfo().getHeight() = " + cli_Console.getScreenInfo().getHeight());
        System.out.println("cli_Console.getScreenInfo().getbTrueCls() = " + cli_Console.getScreenInfo().getbTrueCls());
        System.out.println("cli_Console.getScreenInfo().getbWrapLines() = " + cli_Console.getScreenInfo().getbWrapLines());

        cli.StringDevice cli_StringDevice = new cli.StringDevice();
        cli.SingleCommand cli_SingleCommand = new cli.SingleCommand("help", cli.OutputDevice.getNullDevice());

        cli.OutputFileDevice cli_OutputFileDevice = new cli.OutputFileDevice("output.txt");
        cli.InputFileDevice cli_InputFileDevice = new cli.InputFileDevice("input.txt", cli.OutputDevice.getNullDevice());

        cli.IOMux cli_IOMux = new cli.IOMux();

        cli.TelnetServer cli_TelnetServer = new TestNativeTelnetServer();
    }

    private static void testUI() {
        System.out.println("[testUI()]");
            // libclijava.mak/jni.mak:
            // JAVA_FILES += $(SRC_DIR)/cli/ui/UI.java
            // JAVA_FILES += $(SRC_DIR)/cli/ui/Line.java
            // JAVA_FILES += $(SRC_DIR)/cli/ui/Password.java
            // JAVA_FILES += $(SRC_DIR)/cli/ui/Int.java
            // JAVA_FILES += $(SRC_DIR)/cli/ui/Float.java
            // JAVA_FILES += $(SRC_DIR)/cli/ui/Choice.java
            // JAVA_FILES += $(SRC_DIR)/cli/ui/YesNo.java
            // JAVA_FILES += $(SRC_DIR)/cli/ui/More.java
            // JAVA_FILES += $(SRC_DIR)/cli/ui/Less.java
            // native.mak:
            // CPP_FILES += $(SRC_DIR)/cli_ui_UI.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ui_Line.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ui_Password.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ui_Int.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ui_Float.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ui_Choice.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ui_YesNo.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ui_More.cpp
            // CPP_FILES += $(SRC_DIR)/cli_ui_Less.cpp

            cli.ui.Line ui_Line =  new cli.ui.Line("", 0, 1024);
            cli.ui.Password ui_Password = new cli.ui.Password(true, 0, 256);
            cli.ui.Int ui_Int = new cli.ui.Int(10, 0, 1000);
            cli.ui.Float ui_Float = new cli.ui.Float(10.0, 0.0, 1000.0);
            java.util.Vector<cli.ResourceString> arcli_Choices = new java.util.Vector<cli.ResourceString>();
            arcli_Choices.add(new cli.ResourceString().setString(cli.ResourceString.LANG_EN, "A"));
            arcli_Choices.add(new cli.ResourceString().setString(cli.ResourceString.LANG_EN, "B"));
            arcli_Choices.add(new cli.ResourceString().setString(cli.ResourceString.LANG_EN, "C"));
            cli.ui.Choice ui_Choice = new cli.ui.Choice(0, arcli_Choices);
            cli.ui.YesNo ui_YesNo =  new cli.ui.YesNo(true);
            cli.ui.More ui_More =  new cli.ui.More();
            ui_More.getText().put("Hello cli.ui.More").endl();
            cli.ui.Less ui_Less =  new cli.ui.Less();
            ui_Less.getText().put("Hello cli.ui.Less").endl();
    }
}
