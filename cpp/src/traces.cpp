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
#include <stdio.h>

#include "cli/element.h"
#include "cli/io_device.h"
#include "cli/traces.h"
#include "cli/assert.h"
#include "consistency.h"
#include "constraints.h"

CLI_NS_USE(cli)


#ifndef CLI_NO_NAMESPACE
const TraceClass& cli::GetInternalErrorTraceClass(void)
#else
const TraceClass& GetInternalErrorTraceClass(void)
#endif
{
    static const TraceClass cli_InternalErrorTraceClass("CLI_INTERNAL_ERROR", Help()
        .AddHelp(Help::LANG_EN, "Internal error traces")
        .AddHelp(Help::LANG_FR, "Traces d'erreurs internes"));
    return cli_InternalErrorTraceClass;
}

//! @brief TRACE_TRACES singleton redirection.
#define TRACE_TRACES GetTracesTraceClass()
//! @brief TRACE_TRACES singleton.
static const TraceClass& GetTracesTraceClass(void)
{
    static const TraceClass cli_TracesTraceClass("CLI_TRACES", Help()
        .AddHelp(Help::LANG_EN, "Traces about traces")
        .AddHelp(Help::LANG_FR, "Traces du syst�me de traces"));
    return cli_TracesTraceClass;
}


TraceClass::TraceClass(const TraceClass& CLI_Class)
  : m_strName(CLI_Class.GetName()), m_cliHelp(CLI_Class.GetHelp())
{
}

TraceClass::TraceClass(const char* const STR_ClassName, const Help& CLI_Help)
  : m_strName(MAX_TRACE_CLASS_NAME_LENGTH, STR_ClassName), m_cliHelp(CLI_Help)
{
}

TraceClass::~TraceClass(void)
{
}

const tk::String TraceClass::GetName(void) const
{
    return m_strName;
}

const Help& TraceClass::GetHelp(void) const
{
    return m_cliHelp;
}

TraceClass& TraceClass::operator=(const TraceClass& CLI_Class)
{
    m_strName.Set(CLI_Class.GetName());
    m_cliHelp = CLI_Class.GetHelp();
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
  : m_mapClasses(MAX_TRACE_CLASS_COUNT),
    m_bTraceAll(false),
    m_pcliStream(NULL)
{
    EnsureCommonDevices();

    // Register TRACE_TRACES.
    m_mapClasses.SetAt(TRACE_TRACES.GetName(), TraceClassFlag(TRACE_TRACES, m_bTraceAll));

    // Register INTERNAL_ERROR
    Declare(INTERNAL_ERROR); SetFilter(INTERNAL_ERROR, true);
}

Traces::~Traces(void)
{
    // If the assertion below occures, you might call UnsetStream() before program termination.
    CLI_ASSERT(m_pcliStream == NULL);
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

const bool Traces::UnsetStream(void)
{
    if (cli::OutputDevice* const pcli_Stream = m_pcliStream)
    {
        bool b_Res = true;

        // Unreference the device right now.
        m_pcliStream = NULL;
        if (! pcli_Stream->CloseDown(__CALL_INFO__))
        {
            OutputDevice::GetStdErr() << pcli_Stream->GetLastError().GetString(ResourceString::LANG_DEFAULT) << endl;
            b_Res = false;
        }
        pcli_Stream->FreeInstance(__CALL_INFO__);

        if (! b_Res)
        {
            // Abort on error
            return false;
        }
    }

    return true;
}

const bool Traces::SetStream(OutputDevice& CLI_Stream)
{
    // Free previous reference.
    if (! UnsetStream())
    {
        return false;
    }

    // Store next reference.
    {
        CLI_Stream.UseInstance(__CALL_INFO__);
        if (! CLI_Stream.OpenUp(__CALL_INFO__))
        {
            OutputDevice::GetStdErr() << CLI_Stream.GetLastError().GetString(ResourceString::LANG_DEFAULT) << endl;

            // Store nothing on error.
            CLI_Stream.FreeInstance(__CALL_INFO__);
            return false;
        }
        // Do not store the reference until opening is done.
        m_pcliStream = & CLI_Stream;
    }

    return true;
}

const bool Traces::Declare(const TraceClass& CLI_Class)
{
    if (m_mapClasses.GetAt(CLI_Class.GetName()) != NULL)
    {
        // Already declared.
        // Nothing else to do.
        return true;
    }
    else
    {
        // Show this new trace class depending on the trace all configuration.
        const bool b_ShowTrace = m_bTraceAll;

        // Remember this new class.
        if (m_mapClasses.SetAt(CLI_Class.GetName(), TraceClassFlag(CLI_Class, b_ShowTrace)))
        {
            TraceFilterState(CLI_Class, b_ShowTrace);
            return true;
        }
        else
        {
            return false;
        }
    }
}

const TraceClass::List Traces::GetAllClasses(void) const
{
    // Retrieve current filter.
    TraceClass::List q_Classes(MAX_TRACE_CLASS_COUNT);
    for (   ClassMap::Iterator it = m_mapClasses.GetIterator();
            m_mapClasses.IsValid(it);
            m_mapClasses.MoveNext(it))
    {
        if (! q_Classes.AddTail(m_mapClasses.GetAt(it)))
        {
            GetTraces().Trace(INTERNAL_ERROR)
                << "Traces::GetAllClasses(): "
                << "Could not had '" << m_mapClasses.GetAt(it).GetName() << "' trace class "
                << "to q_Classes"
                << endl;
        }
    }
    return q_Classes;
}

const TraceClass::List Traces::GetCurrentFilter(void) const
{
    // Retrieve current filter.
    TraceClass::List q_CurrentFilter(MAX_TRACE_CLASS_COUNT);
    for (   ClassMap::Iterator it = m_mapClasses.GetIterator();
            m_mapClasses.IsValid(it);
            m_mapClasses.MoveNext(it))
    {
        if (m_mapClasses.GetAt(it).IsVisible())
        {
            if (! q_CurrentFilter.AddTail(m_mapClasses.GetAt(it)))
            {
                GetTraces().Trace(INTERNAL_ERROR)
                    << "Traces::GetCurrentFilter(): "
                    << "Could not had '" << m_mapClasses.GetAt(it).GetName() << "' trace class "
                    << "to q_CurrentFilter"
                    << endl;
            }
        }
    }
    return q_CurrentFilter;
}

const bool Traces::SetFilter(const TraceClass& CLI_Class, const bool B_ShowTraces)
{
    // Check the class exists.
    if (const TraceClassFlag* const pcli_Flags = m_mapClasses.GetAt(CLI_Class.GetName()))
    {
        if (pcli_Flags->IsVisible() != B_ShowTraces)
        {
            // Set the flag.
            m_mapClasses.SetAt(CLI_Class.GetName(), TraceClassFlag(*pcli_Flags, B_ShowTraces));
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
    for (   ClassMap::Iterator it = m_mapClasses.GetIterator();
            m_mapClasses.IsValid(it);
            m_mapClasses.MoveNext(it))
    {
        if (! SetFilter(m_mapClasses.GetAt(it), B_ShowTraces))
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
    Declare(CLI_Class);
    bool b_ShowTrace = false;

    if (const TraceClassFlag* const pcli_Flags = m_mapClasses.GetAt(CLI_Class.GetName()))
    {
        b_ShowTrace = pcli_Flags->IsVisible();
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
  : TraceClass("", Help()), m_bShow(false)
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

Traces::TraceClassFlag::~TraceClassFlag(void)
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
