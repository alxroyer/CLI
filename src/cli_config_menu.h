/*
    This file is part of the CLI library.  The CLI library aims to provide
    facilities in making text-based user interfaces.
    Copyright (C) 2006 Alexis Royer.

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
//! @brief CcliConfigMenu class definition.

#ifndef _CLI_CONFIG_MENU_H_
#define _CLI_CONFIG_MENU_H_

#include "cli_menu.h"


// Forward declarations.
class CcliKeyword;
class CcliOutputDevice;


//! @brief Menu definition.
class CcliConfigMenu : public CcliMenu
{
public:
    //! @brief Default constructor.
    CcliConfigMenu(void);

    //! @brief Destructor.
    virtual ~CcliConfigMenu(void);

public:
    //! @brief CLI reference setting.
    virtual void SetCli(CcliCli& CLI_Cli);
    //! @brief Reserved commands execution.
    virtual const bool ExecuteReserved(const CcliCommandLine& CLI_CommandLine) const;
    //! @brief User-defined commands execution.
    virtual const bool Execute(const CcliCommandLine& CLI_CommandLine) const;

private:
    //! @brief 'echo on' command.
    void EchoOn(void) const;

    //! @brief 'echo off' command.
    void EchoOff(void) const;

    //! @brief 'beep on' command.
    void BeepOn(void) const;

    //! @brief 'beep off' command.
    void BeepOff(void) const;

public:
    //! @brief Initial configuration restoration.
    void RestoreInitialConfig(void) const;

private:
    //! @brief Echo node access.
    CcliKeyword& GetEchoNode(void);
    //! @brief Echo node access (read-only).
    const CcliKeyword& GetEchoNode(void) const;
    //! @brief Echo on node access.
    CcliKeyword& GetEchoOnNode(void);
    //! @brief Echo on node access (read-only).
    const CcliKeyword& GetEchoOnNode(void) const;
    //! @brief Echo off node access.
    CcliKeyword& GetEchoOffNode(void);
    //! @brief Echo off node access (read-only).
    const CcliKeyword& GetEchoOffNode(void) const;

    //! @brief Beep node access.
    CcliKeyword& GetBeepNode(void);
    //! @brief Beep node access (read-only).
    const CcliKeyword& GetBeepNode(void) const;
    //! @brief Beep on node access.
    CcliKeyword& GetBeepOnNode(void);
    //! @brief Beep on node access (read-only).
    const CcliKeyword& GetBeepOnNode(void) const;
    //! @brief Beep off node access.
    CcliKeyword& GetBeepOffNode(void);
    //! @brief Beep off node access (read-only).
    const CcliKeyword& GetBeepOffNode(void) const;

private:
    //! Echo node.
    CcliKeyword* m_pcliEcho;
    //! Echo on node.
    CcliKeyword* m_pcliEchoOn;
    //! Echo off node.
    CcliKeyword* m_pcliEchoOff;
    //! Tmp echo reference.
    mutable const CcliOutputDevice* m_pcliEchoDevice;

    //! Beep node.
    CcliKeyword* m_pcliBeep;
    //! Beep on node.
    CcliKeyword* m_pcliBeepOn;
    //! Beep off node.
    CcliKeyword* m_pcliBeepOff;
    //! Initial beep configuration.
    mutable int m_iInitialBeepConfiguration;
};

#endif // _CLI_CONFIG_MENU_H_
