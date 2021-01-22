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


//! @file
//! @author Alexis Royer
//! @brief Traces menu definition.

#ifndef _CLI_TRACES_MENU_H_
#define _CLI_TRACES_MENU_H_

#include "cli/menu.h"
#include "cli/traces.h"


namespace cli {

    // Forward declarations.
    class ParamString;

    //! @brief Traces menu definition.
    class TracesMenu : public Menu
    {
    public:
        //! @brief Default constructor.
        TracesMenu(void);

        //! @brief Destructor.
        virtual ~TracesMenu(void);

    public:
        //! @brief CLI reference setting.
        virtual void SetCli(Cli& CLI_Cli);
        //! @brief Reserved commands execution.
        virtual const bool ExecuteReserved(const CommandLine& CLI_CmdLine) const;
        //! @brief User-defined commands execution.
        virtual const bool Execute(const CommandLine& CLI_CmdLine) const;

    private:
        //! @brief Show all classes.
        void ShowAllClasses(void) const;

        //! @brief Show current filter.
        void ShowCurrentFilter(void) const;

        //! @brief Display a class list.
        void DisplayClassList(
            const TraceClass::List& Q_Classes   //!< Class list to display.
            ) const;

        //! @brief Filter modification.
        void SetFilter(
            const std::string& STR_ClassName,
            const bool B_Show
            ) const;

        //! @brief Global filter modification.
        void SetAllFilter(
            const bool B_Show           //!< Show flag.
            ) const;

    private:
        //! @brief 'show' node access.
        const Keyword& GetShowNode(void) const;
        //! @brief 'show filter' node access.
        const Keyword& GetShowFilterNode(void) const;
        //! @brief 'show classes' node access.
        const Keyword& GetShowClassesNode(void) const;
        //! @brief 'no' node access.
        const Keyword& GetNoNode(void) const;
        //! @brief 'trace' node access.
        const Keyword& GetTraceNode(void) const;
        //! @brief 'trace filter' node access.
        const Keyword& GetFilterNode(void) const;
        //! @brief 'trace filter <filter>' node access.
        const ParamString& GetFilterParam(void) const;
        //! @brief 'trace all' node access.
        const Keyword& GetAllFilterNode(void) const;

    private:
        //! 'show' node.
        Keyword* m_pcliShowNode;
        //! 'show filter' node.
        Keyword* m_pcliShowFilterNode;
        //! 'show classes' node.
        Keyword* m_pcliShowClassesNode;

        //! 'no' node.
        Keyword* m_pcliNoNode;
        //! 'trace' node.
        Keyword* m_pcliTraceNode;
        //! 'trace filter' node.
        Keyword* m_pcliTraceFilterNode;
        //! 'trace filter <filter>' node.
        ParamString* m_pcliFilterParam;
        //! 'trace all' node.
        Keyword* m_pcliAllFilterNode;
    };

};

#endif // _CLI_TRACES_MENU_H_
