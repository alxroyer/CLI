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


#include "cli/keyword.h"
#include "cli/param_string.h"
#include "cli/syntax_tag.h"
#include "cli/endl.h"
#include "cli/command_line.h"
#include "cli/shell.h"
#include "cli/io_device.h"
#include "traces_menu.h"
#include "consistency.h"

using namespace cli;


TracesMenu::TracesMenu(void)
  : Menu("traces", Help()
        .AddHelp(Help::LANG_EN, "Traces")),
    m_pcliShowNode(NULL), m_pcliShowFilterNode(NULL), m_pcliShowClassesNode(NULL),
    m_pcliNoNode(NULL), m_pcliTraceNode(NULL), m_pcliFilterParam(NULL), m_pcliAllFilterNode(NULL)
{
    EnsureTraces();
}

TracesMenu::~TracesMenu(void)
{
}

void TracesMenu::SetCli(Cli& CLI_Cli)
{
    Menu::SetCli(CLI_Cli);
    {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Show traces behavior"));
        m_pcliShowNode = dynamic_cast<Keyword*>(& AddElement(new Keyword("show", cli_Help)));
        {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Show current filter"));
            m_pcliShowFilterNode = dynamic_cast<Keyword*>(& m_pcliShowNode->AddElement(new Keyword("filter", cli_Help)));
            m_pcliShowFilterNode->AddElement(new Endl(cli_Help)); }
        {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Show all trace classes"));
            m_pcliShowClassesNode = dynamic_cast<Keyword*>(& m_pcliShowNode->AddElement(new Keyword("classes", cli_Help)));
            m_pcliShowClassesNode->AddElement(new Endl(cli_Help)); }}
    {   SyntaxTag* pcli_Tag = dynamic_cast<SyntaxTag*>(& AddElement(new SyntaxTag(false)));
        {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Trace setting"));
            m_pcliTraceNode = dynamic_cast<Keyword*>(& pcli_Tag->AddElement(new Keyword("trace", cli_Help)));
            {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Filter selection"));
                m_pcliTraceFilterNode = dynamic_cast<Keyword*>(& m_pcliTraceNode->AddElement(new Keyword("filter", cli_Help)));
                {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "Trace class name"));
                    m_pcliFilterParam = dynamic_cast<ParamString*>(& m_pcliTraceFilterNode->AddElement(new ParamString(cli_Help)));
                    m_pcliFilterParam->AddElement(new Endl(cli_Help)); }}
            {   Help cli_Help(Help().AddHelp(Help::LANG_EN, "All traces"));
                m_pcliAllFilterNode = dynamic_cast<Keyword*>(& m_pcliTraceNode->AddElement(new Keyword("all", cli_Help)));
                m_pcliAllFilterNode->AddElement(new Endl(cli_Help)); }}
        Help cli_Help(Help().AddHelp(Help::LANG_EN, "Remove traces"));
        m_pcliNoNode = dynamic_cast<Keyword*>(& AddElement(new Keyword("no", cli_Help)));
        m_pcliNoNode->AddElement(new SyntaxRef(*pcli_Tag)); }
}

const bool TracesMenu::ExecuteReserved(const CommandLine& CLI_CmdLine) const
{
    bool b_Show = true;

    int i_CmdLineIndex = 0;
    const cli::Element* pcli_Element = NULL;
    #define CLI_STEP_IT() do { if (i_CmdLineIndex >= CLI_CmdLine.GetElementCount()) return false; pcli_Element = & CLI_CmdLine[i_CmdLineIndex++]; } while(0)

    CLI_STEP_IT();
    if (0) {}
    else if (pcli_Element == & GetShowNode())
    {
        CLI_STEP_IT();
        if (0) {}
        else if (pcli_Element == & GetShowFilterNode())
        {
            CLI_STEP_IT();
            if (dynamic_cast<const Endl*>(pcli_Element))
            {
                ShowCurrentFilter();
                return true;
            }
        }
        else if (pcli_Element == & GetShowClassesNode())
        {
            CLI_STEP_IT();
            if (dynamic_cast<const Endl*>(pcli_Element))
            {
                ShowAllClasses();
                return true;
            }
        }
    }
    else if (pcli_Element == & GetNoNode())
    {
        CLI_STEP_IT();
        b_Show = false;
        if (0) {}
        else if (pcli_Element == & GetTraceNode())
        {
            CLI_STEP_IT();
            goto trace_label;
        }
    }
    else if (pcli_Element == & GetTraceNode())
    {
        CLI_STEP_IT();
trace_label:
        if (0) {}
        else if (pcli_Element == & GetFilterNode())
        {
            CLI_STEP_IT();
            if (const cli::Param* pcli_Param = dynamic_cast<const cli::Param*>(pcli_Element))
            if (pcli_Param->GetCloned() == & GetFilterParam()) {
                pcli_Param = dynamic_cast<const cli::Param*>(pcli_Element);
                GetFilterParam().CopyValue(*pcli_Param);
                CLI_STEP_IT();
                if (dynamic_cast<const Endl*>(pcli_Element))
                {
                    SetFilter(GetFilterParam(), b_Show);
                    return true;
                }
            }
        }
        else if (pcli_Element == & GetAllFilterNode())
        {
            CLI_STEP_IT();
            if (dynamic_cast<const Endl*>(pcli_Element))
            {
                SetAllFilter(b_Show);
                return true;
            }
        }
    }

    return Menu::ExecuteReserved(CLI_CmdLine);
}

const bool TracesMenu::Execute(const CommandLine& CLI_CmdLine) const
{
    return false;
}

void TracesMenu::ShowAllClasses(void) const
{
    GetOutputStream() << "All classes:" << endl;
    DisplayClassList(GetTraces().GetAllClasses());
}

void TracesMenu::ShowCurrentFilter(void) const
{
    GetOutputStream() << "Current filter:" << endl;
    DisplayClassList(GetTraces().GetCurrentFilter());
}

void TracesMenu::DisplayClassList(const TraceClass::List& Q_Classes) const
{
    for (   TraceClass::List::const_iterator it = Q_Classes.begin();
            it != Q_Classes.end();
            it ++)
    {
        // Margin.
        unsigned int ui_Len = 0;
        for (; ui_Len < GetShell().GetHelpMargin(); ui_Len ++)
        {
            GetOutputStream() << " ";
        }

        // Display class name.
        GetOutputStream() << "<" << it->GetName() << ">";
        ui_Len += (it->GetName().size() + 2);

        // Offset.
        if (ui_Len >= GetShell().GetHelpOffset())
        {
            GetOutputStream() << " ";
            ui_Len ++;
        }
        for (; ui_Len < GetShell().GetHelpOffset(); ui_Len ++)
        {
            GetOutputStream() << " ";
        }

        // Description.
        GetOutputStream() << it->GetDescription();
        ui_Len += it->GetDescription().size();

        // End of line.
        GetOutputStream() << endl;
    }
}

void TracesMenu::SetFilter(const std::string& STR_ClassName, const bool B_Show) const
{
    TraceClass cli_Class(STR_ClassName, "");
    GetTraces().SetFilter(cli_Class, B_Show);
}

void TracesMenu::SetAllFilter(const bool B_Show) const
{
    GetTraces().SetAllFilter(B_Show);
}

const Keyword& TracesMenu::GetShowNode(void) const
{
    assert(m_pcliShowNode != NULL);
    return *m_pcliShowNode;
}

const Keyword& TracesMenu::GetShowFilterNode(void) const
{
    assert(m_pcliShowFilterNode != NULL);
    return *m_pcliShowFilterNode;
}

const Keyword& TracesMenu::GetShowClassesNode(void) const
{
    assert(m_pcliShowClassesNode != NULL);
    return *m_pcliShowClassesNode;
}

const Keyword& TracesMenu::GetNoNode(void) const
{
    assert(m_pcliNoNode != NULL);
    return *m_pcliNoNode;
}

const Keyword& TracesMenu::GetTraceNode(void) const
{
    assert(m_pcliTraceNode != NULL);
    return *m_pcliTraceNode;
}

const Keyword& TracesMenu::GetFilterNode(void) const
{
    assert(m_pcliTraceFilterNode != NULL);
    return *m_pcliTraceFilterNode;
}

const ParamString& TracesMenu::GetFilterParam(void) const
{
    assert(m_pcliFilterParam != NULL);
    return *m_pcliFilterParam;
}

const Keyword& TracesMenu::GetAllFilterNode(void) const
{
    assert(m_pcliAllFilterNode != NULL);
    return *m_pcliAllFilterNode;
}

