/*
    Copyright (c) 2006-2008, Alexis Royer

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


#include "cli/pch.h"

#include <stdlib.h>

#include "cli/keyword.h"
#include "cli/endl.h"
#include "cli/command_line.h"
#include "cli/shell.h"
#include "config_menu.h"

CLI_NS_USE(cli)


ConfigMenu::ConfigMenu(void)
  : Menu("cli-config", Help()
        .AddHelp(Help::LANG_EN, "CLI settings menu")
        .AddHelp(Help::LANG_FR, "Menu de configuration du CLI")),
    m_pcliEcho(NULL), m_pcliEchoOn(NULL), m_pcliEchoOff(NULL),
    m_pcliBeep(NULL), m_pcliBeepOn(NULL), m_pcliBeepOff(NULL),
    m_pcliLang(NULL), m_pcliEnglishLang(NULL), m_pcliFrenchLang(NULL)
{
}

ConfigMenu::~ConfigMenu(void)
{
}

void ConfigMenu::SetCli(Cli& CLI_Cli)
{
    Menu::SetCli(CLI_Cli);
    {   Help cli_Help(Help()
            .AddHelp(Help::LANG_EN, "Modify echo behavior")
            .AddHelp(Help::LANG_FR, "Configuration de l'écho"));
        m_pcliEcho = dynamic_cast<Keyword*>(& AddElement(new Keyword("echo", cli_Help)));
        {   Help cli_Help(Help()
                .AddHelp(Help::LANG_EN, "Set echo on")
                .AddHelp(Help::LANG_FR, "Activation de l'écho"));
            m_pcliEchoOn = dynamic_cast<Keyword*>(& m_pcliEcho->AddElement(new Keyword("on", cli_Help)));
            m_pcliEchoOn->AddElement(new Endl(cli_Help)); }
        {   Help cli_Help(Help()
                .AddHelp(Help::LANG_EN, "Set echo off")
                .AddHelp(Help::LANG_FR, "Désactivation de l'écho"));
            m_pcliEchoOff = dynamic_cast<Keyword*>(& m_pcliEcho->AddElement(new Keyword("off", cli_Help)));
            m_pcliEchoOff->AddElement(new Endl(cli_Help)); }}
    {   Help cli_Help(Help()
            .AddHelp(Help::LANG_EN, "Modify beep behavior")
            .AddHelp(Help::LANG_FR, "Configuration du bip"));
        m_pcliBeep = dynamic_cast<Keyword*>(& AddElement(new Keyword("beep", cli_Help)));
        {   Help cli_Help(Help()
                .AddHelp(Help::LANG_EN, "Set beep on")
                .AddHelp(Help::LANG_FR, "Activation du bip"));
            m_pcliBeepOn = dynamic_cast<Keyword*>(& m_pcliBeep->AddElement(new Keyword("on", cli_Help)));
            m_pcliBeepOn->AddElement(new Endl(cli_Help)); }
        {   Help cli_Help(Help()
                .AddHelp(Help::LANG_EN, "Set beep off")
                .AddHelp(Help::LANG_FR, "Désactivation du bip"));
            m_pcliBeepOff = dynamic_cast<Keyword*>(& m_pcliBeep->AddElement(new Keyword("off", cli_Help)));
            m_pcliBeepOff->AddElement(new Endl(cli_Help)); }}
    {   Help cli_Help(Help()
            .AddHelp(Help::LANG_EN, "Language setting")
            .AddHelp(Help::LANG_FR, "Modification de la langue"));
        m_pcliLang = dynamic_cast<Keyword*>(& AddElement(new Keyword("lang", cli_Help)));
        {   Help cli_Help(Help()
                .AddHelp(Help::LANG_EN, "English language")
                .AddHelp(Help::LANG_FR, "Langue anglaise"));
            m_pcliEnglishLang = dynamic_cast<Keyword*>(& m_pcliLang->AddElement(new Keyword("english", cli_Help)));
            m_pcliEnglishLang->AddElement(new Endl(cli_Help)); }
        {   Help cli_Help(Help()
                .AddHelp(Help::LANG_EN, "French language")
                .AddHelp(Help::LANG_FR, "Langue française"));
            m_pcliFrenchLang = dynamic_cast<Keyword*>(& m_pcliLang->AddElement(new Keyword("french", cli_Help)));
            m_pcliFrenchLang->AddElement(new Endl(cli_Help)); }}
}

const bool ConfigMenu::ExecuteReserved(const CommandLine& CLI_CmdLine) const
{
    CommandLineIterator it(CLI_CmdLine);

    if (! it.StepIt()) { return false; }
    else if (it == GetEchoNode())
    {
        if (! it.StepIt()) { return false; }
        else if (it == GetEchoOnNode())
        {
            if (! it.StepIt()) { return false; }
            if (dynamic_cast<const Endl*>(*it))
            {
                EchoOn();
                return true;
            }
        }
        else if (it == GetEchoOffNode())
        {
            if (! it.StepIt()) { return false; }
            if (dynamic_cast<const Endl*>(*it))
            {
                EchoOff();
                return true;
            }
        }
    }
    else if (it == GetBeepNode())
    {
        if (! it.StepIt()) { return false; }
        else if (it == GetBeepOnNode())
        {
            if (! it.StepIt()) { return false; }
            if (dynamic_cast<const Endl*>(*it))
            {
                BeepOn();
                return true;
            }
        }
        else if (it == GetBeepOffNode())
        {
            if (! it.StepIt()) { return false; }
            if (dynamic_cast<const Endl*>(*it))
            {
                BeepOff();
                return true;
            }
        }
    }
    else if (it == GetLangNode())
    {
        if (! it.StepIt()) { return false; }
        else if (it == GetEnglishLangNode())
        {
            if (! it.StepIt()) { return false; }
            if (dynamic_cast<const Endl*>(*it))
            {
                SetLang(ResourceString::LANG_EN);
                return true;
            }
        }
        else if (it == GetFrenchLangNode())
        {
            if (! it.StepIt()) { return false; }
            if (dynamic_cast<const Endl*>(*it))
            {
                SetLang(ResourceString::LANG_FR);
                return true;
            }
        }
    }

    return Menu::ExecuteReserved(CLI_CmdLine);
}

const bool ConfigMenu::Execute(const CommandLine& CLI_CmdLine) const
{
    return false;
}

void ConfigMenu::EchoOn(void) const
{
    GetShell().EnableStream(ECHO_STREAM, true);
    const ResourceString cli_EchoOn = ResourceString()
        .SetString(ResourceString::LANG_EN, "echo is on")
        .SetString(ResourceString::LANG_FR, "écho activé");
    GetOutputStream() << cli_EchoOn.GetString(GetShell().GetLang()) << endl;
}

void ConfigMenu::EchoOff(void) const
{
    GetShell().EnableStream(ECHO_STREAM, false);
    const ResourceString cli_EchoOff = ResourceString()
        .SetString(ResourceString::LANG_EN, "echo is off")
        .SetString(ResourceString::LANG_FR, "écho désactivé");
    GetOutputStream() << cli_EchoOff.GetString(GetShell().GetLang()) << endl;
}

void ConfigMenu::BeepOn(void) const
{
    GetShell().SetBeep(true);
    const ResourceString cli_BeepOn = ResourceString()
        .SetString(ResourceString::LANG_EN, "beep is on")
        .SetString(ResourceString::LANG_FR, "bip activé");
    GetOutputStream() << cli_BeepOn.GetString(GetShell().GetLang()) << endl;
}

void ConfigMenu::BeepOff(void) const
{
    GetShell().SetBeep(false);
    const ResourceString cli_BeepOff = ResourceString()
        .SetString(ResourceString::LANG_EN, "beep is off")
        .SetString(ResourceString::LANG_FR, "bip désactivé");
    GetOutputStream() << cli_BeepOff.GetString(GetShell().GetLang()) << endl;
}

void ConfigMenu::SetLang(const ResourceString::LANG E_Lang) const
{
    GetShell().SetLang(E_Lang);
    const ResourceString cli_SelectedLanguage = ResourceString()
        .SetString(ResourceString::LANG_EN, "English language")
        .SetString(ResourceString::LANG_FR, "Langue française");
    GetOutputStream() << cli_SelectedLanguage.GetString(GetShell().GetLang()) << endl;
}

Keyword& ConfigMenu::GetEchoNode(void)
{
    return *m_pcliEcho;
}

const Keyword& ConfigMenu::GetEchoNode(void) const
{
    return *m_pcliEcho;
}

Keyword& ConfigMenu::GetEchoOnNode(void)
{
    return *m_pcliEchoOn;
}

const Keyword& ConfigMenu::GetEchoOnNode(void) const
{
    return *m_pcliEchoOn;
}

Keyword& ConfigMenu::GetEchoOffNode(void)
{
    return *m_pcliEchoOff;
}

const Keyword& ConfigMenu::GetEchoOffNode(void) const
{
    return *m_pcliEchoOff;
}

Keyword& ConfigMenu::GetBeepNode(void)
{
    return *m_pcliBeep;
}

const Keyword& ConfigMenu::GetBeepNode(void) const
{
    return *m_pcliBeep;
}

Keyword& ConfigMenu::GetBeepOnNode(void)
{
    return *m_pcliBeepOn;
}

const Keyword& ConfigMenu::GetBeepOnNode(void) const
{
    return *m_pcliBeepOn;
}

Keyword& ConfigMenu::GetBeepOffNode(void)
{
    return *m_pcliBeepOff;
}

const Keyword& ConfigMenu::GetBeepOffNode(void) const
{
    return *m_pcliBeepOff;
}

Keyword& ConfigMenu::GetLangNode(void)
{
    return *m_pcliLang;
}

const Keyword& ConfigMenu::GetLangNode(void) const
{
    return *m_pcliLang;
}

Keyword& ConfigMenu::GetEnglishLangNode(void)
{
    return *m_pcliEnglishLang;
}

const Keyword& ConfigMenu::GetEnglishLangNode(void) const
{
    return *m_pcliEnglishLang;
}

Keyword& ConfigMenu::GetFrenchLangNode(void)
{
    return *m_pcliFrenchLang;
}

const Keyword& ConfigMenu::GetFrenchLangNode(void) const
{
    return *m_pcliFrenchLang;
}
