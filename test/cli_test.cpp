/*
    Copyright 2006 Alexis Royer

    This file is part of the CLI library.

    The CLI library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <iostream>

#include "cli_cli.h"
#include "cli_menu.h"
#include "cli_keyword.h"
#include "cli_param_int.h"
#include "cli_endl.h"
#include "cli_syntax_tag.h"
#include "cli_shell.h"


#define CLI_HELP(__strhelp) CcliHelp().AddHelp(CcliHelp::CLI_LANG_EN, __strhelp)


CcliCli cli_TestCli("test-cli", CLI_HELP("Test CLI"));
    // show
    CcliKeyword& cli_Show = dynamic_cast<CcliKeyword&>(cli_TestCli.AddElement(new CcliKeyword("show", CLI_HELP("Show parameters"))));
        // all
        CcliKeyword& cli_All = dynamic_cast<CcliKeyword&>(cli_Show.AddElement(new CcliKeyword("all", CLI_HELP("Show all parameters"))));
            // endl
            CcliEndl& cli_Endl1 = dynamic_cast<CcliEndl&>(cli_All.AddElement(new CcliEndl(CLI_HELP("Show all parameters"))));
        CcliSyntaxTag& cli_Tag = dynamic_cast<CcliSyntaxTag&>(cli_Show.AddElement(new CcliSyntaxTag()));
            // param
            CcliKeyword& cli_Param = dynamic_cast<CcliKeyword&>(cli_Tag.AddElement(new CcliKeyword("param", CLI_HELP("Parameter <id>"))));
                // <id>
                CcliParamInt& cli_ParamValue = dynamic_cast<CcliParamInt&>(cli_Param.AddElement(new CcliParamInt(CLI_HELP("Parameter id"))));
                    // endl
                    CcliEndl& cli_Endl2 = dynamic_cast<CcliEndl&>(cli_ParamValue.AddElement(new CcliEndl(CLI_HELP("Show given parameter"))));
                    //ref
                    CcliSyntaxRef& cli_Ref = dynamic_cast<CcliSyntaxRef&>(cli_ParamValue.AddElement(new CcliSyntaxRef(cli_Tag)));
    // my-menu
    CcliKeyword& cli_MyMenu = dynamic_cast<CcliKeyword&>(cli_TestCli.AddElement(new CcliKeyword("my-menu", CLI_HELP("Personal menu"))));
        // endl
        CcliEndl& cli_Endl3 = dynamic_cast<CcliEndl&>(cli_MyMenu.AddElement(new CcliEndl(CLI_HELP("Enter personal menu"))));

// my-menu
CcliMenu& cli_MyMenuMenu = dynamic_cast<CcliMenu&>(cli_Endl3.SetMenu(new CcliMenu("my-menu", CLI_HELP("Personal Menu"))));

int main(int I_ArgsCount, char** ARSTR_Args)
{
    CcliShell cli_Shell(cli_TestCli);
    if (I_ArgsCount == 1)
    {
        cli_Shell.RunConsole();
    }
    else
    {
        std::cout << "Running telnet server on port " << ARSTR_Args[1] << std::endl;
        cli_Shell.RunTelnet(atoi(ARSTR_Args[1]));
    }
    return 0;
}

