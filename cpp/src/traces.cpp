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


#include "cli/element.h"
#include "cli/io_device.h"
#include "cli/traces.h"
#include "consistency.h"

using namespace cli;


static const TraceClass TRACE_TRACES("TRACES", "Traces about traces.");


TraceClass::TraceClass(const TraceClass& CLI_Class)
  : m_strName(CLI_Class.GetName()), m_strDescription(CLI_Class.GetDescription())
{
}

TraceClass::TraceClass(const std::string& STR_ClassName, const std::string& STR_Description)
  : m_strName(STR_ClassName), m_strDescription(STR_Description)
{
}

TraceClass::~TraceClass(void)
{
}

const std::string& TraceClass::GetName(void) const
{
    return m_strName;
}

const std::string& TraceClass::GetDescription(void) const
{
    return m_strDescription;
}

TraceClass& TraceClass::operator=(const TraceClass& CLI_Class)
{
    m_strName = CLI_Class.GetName();
    m_strDescription = CLI_Class.GetDescription();
    return *this;
}

const bool cli::operator==(const TraceClass& CLI_Class1, const TraceClass& CLI_Class2)
{
    return (CLI_Class1.GetName() == CLI_Class2.GetName());
}



Traces& cli::GetTraces(void)
{
    return Traces::GetInstance();
}

Traces& Traces::GetInstance(void)
{
    static Traces cli_Instance;
    return cli_Instance;
}


Traces::Traces(void)
  : m_bTraceAll(false),
    m_pcliStream(NULL)
{
    EnsureCommonDevices();

    // Register TRACE_TRACES.
    m_mapClasses[TRACE_TRACES.GetName()] = TraceClassFlag(TRACE_TRACES, m_bTraceAll);
}

Traces::~Traces(void)
{
    if (cli::OutputDevice* const pcli_Stream = m_pcliStream)
    {
        m_pcliStream = NULL;
        pcli_Stream->CloseDown(__CALL_INFO__);
        pcli_Stream->FreeInstance(__CALL_INFO__);
    }
}

const OutputDevice& Traces::GetStream(void) const
{
    if (m_pcliStream != NULL)
    {
        return *m_pcliStream;
    }
    else
    {
        // Default to stderr.
        return OutputDevice::GetStdErr();
    }
}

const bool Traces::SetStream(OutputDevice& CLI_Stream)
{
    // Free previous reference.
    if (cli::OutputDevice* const pcli_Stream = m_pcliStream)
    {
        bool b_Res = true;

        // Unreference the device right now.
        m_pcliStream = NULL;
        if (! pcli_Stream->CloseDown(__CALL_INFO__))
        {
            b_Res = false;
        }
        pcli_Stream->FreeInstance(__CALL_INFO__);

        if (! b_Res)
        {
            // Abort on error
            return false;
        }
    }

    // Store next reference.
    {
        CLI_Stream.UseInstance(__CALL_INFO__);
        if (! CLI_Stream.OpenUp(__CALL_INFO__))
        {
            // Store nothing on error.
            CLI_Stream.FreeInstance(__CALL_INFO__);
            return false;
        }
        // Do not store the reference until opening is done.
        m_pcliStream = & CLI_Stream;
    }

    return true;
}

const TraceClass::List Traces::GetAllClasses(void) const
{
    // Retrieve current filter.
    TraceClass::List q_Classes;
    for (   ClassMap::const_iterator it = m_mapClasses.begin();
            it != m_mapClasses.end();
            it ++)
    {
        q_Classes.push_back(it->second);
    }
    return q_Classes;
}

const TraceClass::List Traces::GetCurrentFilter(void) const
{
    // Retrieve current filter.
    TraceClass::List q_CurrentFilter;
    for (   ClassMap::const_iterator it = m_mapClasses.begin();
            it != m_mapClasses.end();
            it ++)
    {
        if (it->second.IsVisible())
        {
            q_CurrentFilter.push_back(it->second);
        }
    }
    return q_CurrentFilter;
}

const bool Traces::SetFilter(const TraceClass& CLI_Class, const bool B_ShowTraces)
{
    // Check the class exists.
    ClassMap::const_iterator it = m_mapClasses.find(CLI_Class.GetName());
    if (it != m_mapClasses.end())
    {
        if (it->second.IsVisible() != B_ShowTraces)
        {
            // Set the flag.
            m_mapClasses[CLI_Class.GetName()] = TraceClassFlag(it->second, B_ShowTraces);
            TraceFilterState(CLI_Class, B_ShowTraces);
        }
        return true;
    }

    return false;
}

const bool Traces::SetAllFilter(const bool B_ShowTraces)
{
    // Remember 'all filter configuration'.
    m_bTraceAll = B_ShowTraces;
    BeginTrace(TRACE_TRACES) << "All traces " << (B_ShowTraces ? "enabled" : "disabled") << endl;

    // Trace traces modifications.
    SetFilter(TRACE_TRACES, true);

    // Then adapt every filter configuration.
    for (   ClassMap::const_iterator it = m_mapClasses.begin();
            it != m_mapClasses.end();
            it ++)
    {
        if (! SetFilter(it->second, B_ShowTraces))
        {
            return false;
        }
    }

    // Set final traces settings.
    SetFilter(TRACE_TRACES, B_ShowTraces);

    return true;
}

const OutputDevice& Traces::Trace(const TraceClass& CLI_Class)
{
    bool b_ShowTrace = false;

    ClassMap::const_iterator it = m_mapClasses.find(CLI_Class.GetName());
    if (it != m_mapClasses.end())
    {
        b_ShowTrace = it->second.IsVisible();
    }
    else
    {
        // Show this new trace class depending on the trace all configuration.
        b_ShowTrace = m_bTraceAll;

        // Remember this new class.
        m_mapClasses[CLI_Class.GetName()] = TraceClassFlag(CLI_Class, b_ShowTrace);
        TraceFilterState(CLI_Class, b_ShowTrace);
    }

    if (b_ShowTrace)
    {
        return BeginTrace(CLI_Class);
    }
    else
    {
        return OutputDevice::GetNullDevice();
    }
}

const OutputDevice& Traces::BeginTrace(const TraceClass& CLI_Class)
{
    return (GetStream() << "<" << CLI_Class.GetName() << "> ");
}

const bool Traces::TraceFilterState(const TraceClass& CLI_Class, const bool B_ShowTraces)
{
    // Trace this new class state.
    (   (CLI_Class == TRACE_TRACES) ? BeginTrace(TRACE_TRACES) : Trace(TRACE_TRACES))
        << "Trace <" << CLI_Class.GetName() << "> "
        << "-> " << (B_ShowTraces ? "on" : "off") << endl;

    return true;
}


Traces::TraceClassFlag::TraceClassFlag(void)
  : TraceClass("", ""), m_bShow(false)
{
}

Traces::TraceClassFlag::TraceClassFlag(const TraceClass& CLI_Source, const bool B_Show)
  : TraceClass(CLI_Source), m_bShow(B_Show)
{
}

Traces::TraceClassFlag::TraceClassFlag(const TraceClassFlag& CLI_Source)
  : TraceClass(CLI_Source), m_bShow(CLI_Source.IsVisible())
{
}

Traces::TraceClassFlag& Traces::TraceClassFlag::operator=(const Traces::TraceClassFlag& CLI_Class)
{
    TraceClass::operator=(CLI_Class);
    m_bShow = CLI_Class.IsVisible();
    return *this;
}

const bool Traces::TraceClassFlag::IsVisible(void) const
{
    return m_bShow;
}
