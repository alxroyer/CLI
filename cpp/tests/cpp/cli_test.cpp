/*
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006-2007 Alexis Royer.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <iostream>

#include "cli/cli.h"
#include "cli/menu.h"
#include "cli/keyword.h"
#include "cli/param_int.h"
#include "cli/endl.h"
#include "cli/syntax_tag.h"
#include "cli/shell.h"
#include "cli/console.h"
#include "cli/telnet.h"


class _Cli : public cli::Cli
{
public:
    _Cli(const std::string& STR_Name, const cli::Help& CLI_Help)
      : Cli(STR_Name, CLI_Help)
    {
    }

    virtual ~_Cli(void)
    {
    }
};

_Cli cli_TestCli("test-cli", cli::Help().AddHelp(cli::Help::LANG_EN, "Test CLI"));
 // show
 cli::Keyword& cli_Show = dynamic_cast<cli::Keyword&>(cli_TestCli.AddElement(new cli::Keyword("show", cli::Help().AddHelp(cli::Help::LANG_EN, "Show parameters"))));
  // all
  cli::Keyword& cli_All = dynamic_cast<cli::Keyword&>(cli_Show.AddElement(new cli::Keyword("all", cli::Help().AddHelp(cli::Help::LANG_EN, "Show all parameters"))));
   // endl
   cli::Endl& cli_Endl1 = dynamic_cast<cli::Endl&>(cli_All.AddElement(new cli::Endl(cli::Help().AddHelp(cli::Help::LANG_EN, "Show all parameters"))));
  cli::SyntaxTag& cli_Tag = dynamic_cast<cli::SyntaxTag&>(cli_Show.AddElement(new cli::SyntaxTag(false)));
   // param
   cli::Keyword& cli_Param = dynamic_cast<cli::Keyword&>(cli_Tag.AddElement(new cli::Keyword("param", cli::Help().AddHelp(cli::Help::LANG_EN, "Parameter <id>"))));
    // <id>
    cli::ParamInt& cli_ParamValue = dynamic_cast<cli::ParamInt&>(cli_Param.AddElement(new cli::ParamInt(cli::Help().AddHelp(cli::Help::LANG_EN, "Parameter id"))));
     // endl
     cli::Endl& cli_Endl2 = dynamic_cast<cli::Endl&>(cli_ParamValue.AddElement(new cli::Endl(cli::Help().AddHelp(cli::Help::LANG_EN, "Show given parameter"))));
     //ref
     cli::SyntaxRef& cli_Ref = dynamic_cast<cli::SyntaxRef&>(cli_ParamValue.AddElement(new cli::SyntaxRef(cli_Tag)));
 // my-cli::Menu
 cli::Keyword& cli_MyMenu = dynamic_cast<cli::Keyword&>(cli_TestCli.AddElement(new cli::Keyword("my-cli::Menu", cli::Help().AddHelp(cli::Help::LANG_EN, "Personal cli::Menu"))));
  // endl
  cli::Endl& cli_Endl3 = dynamic_cast<cli::Endl&>(cli_MyMenu.AddElement(new cli::Endl(cli::Help().AddHelp(cli::Help::LANG_EN, "Enter personal cli::Menu"))));

// my-menu
cli::Menu& cli_MyMenuMenu = dynamic_cast<cli::Menu&>(cli_Endl3.SetMenu(new cli::Menu("my-menu", cli::Help().AddHelp(cli::Help::LANG_EN, "Personal Menu"))));


int main(int I_ArgsCount, char* ARSTR_Args[])
{
    cli::Shell cli_Shell(cli_TestCli);
    if (I_ArgsCount == 1)
    {
        cli::Console cli_Console(false);
        cli_Shell.Run(cli_Console);
    }
    else
    {
        std::cout << "Running telnet server on port " << ARSTR_Args[1] << std::endl;
        cli::TelnetServer cli_Server(cli_Shell, atoi(ARSTR_Args[1]));
        cli_Server.StartServer();
    }
    return 0;
}

