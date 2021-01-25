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


#include "cli/pch.h"

#include "cli/cli.h"
#include "cli/ui.h"
#include "cli/shell.h"


CLI_NS_BEGIN(cli)

    CLI_NS_BEGIN(ui)

        UI::UI(void)
          : ExecutionContext(), m_bExecResult(false)
        {
        }

        UI::UI(ExecutionContext& CLI_ParentContext)
          : ExecutionContext(CLI_ParentContext), m_bExecResult(false)
        {
        }

        UI::~UI(void)
        {
        }

        const bool UI::GetbExecResult(void) const
        {
            return m_bExecResult;
        }

        const bool UI::OnStartExecution(void)
        {
            m_bExecResult = false;
            Reset();
            ResetToDefault();

            return true;
        }

        const bool UI::OnStopExecution(void)
        {
            // If failure, reset the user interface object and manage output.
            if (! m_bExecResult)
            {
                ResetToDefault();
                GetStream(ECHO_STREAM) << endl;
            }

            return true;
        }

        void UI::EndControl(const bool B_ExecResult)
        {
            // Store execution result.
            m_bExecResult = B_ExecResult;

            // Stop execution
            ExecutionContext::StopExecution();
        }

    CLI_NS_END(ui)

CLI_NS_END(cli)

