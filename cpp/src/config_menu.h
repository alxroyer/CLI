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
//! @brief ConfigMenu class definition.

#ifndef _CLI_CONFIG_MENU_H_
#define _CLI_CONFIG_MENU_H_

#include "cli/menu.h"


namespace cli {

    // Forward declarations.
    class Keyword;
    class OutputDevice;


    //! @brief CLI configuration menu definition.
    class ConfigMenu : public Menu
    {
    public:
        //! @brief Default constructor.
        ConfigMenu(void);

        //! @brief Destructor.
        virtual ~ConfigMenu(void);

    public:
        //! @brief CLI reference setting.
        virtual void SetCli(Cli& CLI_Cli);
        //! @brief Reserved commands execution.
        virtual const bool ExecuteReserved(const CommandLine& CLI_CmdLine) const;
        //! @brief User-defined commands execution.
        virtual const bool Execute(const CommandLine& CLI_CmdLine) const;

    private:
        //! @brief 'echo on' command.
        void EchoOn(void) const;

        //! @brief 'echo off' command.
        void EchoOff(void) const;

        //! @brief 'beep on' command.
        void BeepOn(void) const;

        //! @brief 'beep off' command.
        void BeepOff(void) const;

    private:
        //! @brief Echo node access.
        Keyword& GetEchoNode(void);
        //! @brief Echo node access (read-only).
        const Keyword& GetEchoNode(void) const;
        //! @brief Echo on node access.
        Keyword& GetEchoOnNode(void);
        //! @brief Echo on node access (read-only).
        const Keyword& GetEchoOnNode(void) const;
        //! @brief Echo off node access.
        Keyword& GetEchoOffNode(void);
        //! @brief Echo off node access (read-only).
        const Keyword& GetEchoOffNode(void) const;

        //! @brief Beep node access.
        Keyword& GetBeepNode(void);
        //! @brief Beep node access (read-only).
        const Keyword& GetBeepNode(void) const;
        //! @brief Beep on node access.
        Keyword& GetBeepOnNode(void);
        //! @brief Beep on node access (read-only).
        const Keyword& GetBeepOnNode(void) const;
        //! @brief Beep off node access.
        Keyword& GetBeepOffNode(void);
        //! @brief Beep off node access (read-only).
        const Keyword& GetBeepOffNode(void) const;

    private:
        //! Echo node.
        Keyword* m_pcliEcho;
        //! Echo on node.
        Keyword* m_pcliEchoOn;
        //! Echo off node.
        Keyword* m_pcliEchoOff;

        //! Beep node.
        Keyword* m_pcliBeep;
        //! Beep on node.
        Keyword* m_pcliBeepOn;
        //! Beep off node.
        Keyword* m_pcliBeepOff;
    };

};

#endif // _CLI_CONFIG_MENU_H_
