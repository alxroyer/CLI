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
//! @brief Trace classes definition.

#ifndef _CLI_TRACES_H_
#define _CLI_TRACES_H_

#include <string>
#include <deque>
#include <map>


namespace cli {

    // Forward declarations.
    class Element;
    class OutputDevice;


    //! @brief Trace class object.
    class TraceClass
    {
    public:
        //! @brief Trace class list typedef.
        typedef std::deque<TraceClass> List;

    private:
        //! @brief Default constructor forbidden.
        TraceClass(void);

    public:
        //! @brief Copy constructor.
        TraceClass(
            const TraceClass& CLI_Class         //!< Source class object.
            );

        //! @brief Constructor.
        TraceClass(
            const std::string& STR_ClassName,   //!< Class name.
            const std::string& STR_Description  //!< Description.
            );

        //! @brief Destructor.
        virtual ~TraceClass(void);

    public:
        //! @brief Class name accessor.
        const std::string& GetName(void) const;

        //! @brief Description accessor.
        const std::string& GetDescription(void) const;

    protected:
        //! @brief Assignment operator.
        //! @note Not available from outside.
        TraceClass& operator=(const TraceClass&);

    private:
        //! Class name.
        std::string m_strName;

        //! Description.
        std::string m_strDescription;
    };

    //! @brief Classes equivalence operator.
    const bool operator==(
        const TraceClass& CLI_Class1,   //!< First member.
        const TraceClass& CLI_Class2    //!< Second member.
        );


    //! @brief Traces service.
    class Traces
    {
    public:
        //! @brief Singleton.
        static Traces& GetInstance(void);

    protected:
        //! @brief Default constructor.
        Traces(void);

    public:
        //! @brief Destructor.
        virtual ~Traces(void);

    public:
        //! @brief Stream access.
        const OutputDevice& GetStream(void) const;
        //! @brief Stream positionning.
        //! @warning Please ensure one of the following conditions regarding the given device:
        //!     - Either the device is an auto-deleted device,
        //!     - or it will be destroyed after the traces system,
        //!     - or another call to this method with the null device is done on termination.
        //! Otherwise you could experience consistency troubles.
        //! The null device and standard devices are not subject to this remark.
        const bool SetStream(
            OutputDevice& CLI_Stream                //!< Stream reference.
            );

    public:
        //! @brief All classes accessor.
        const TraceClass::List GetAllClasses(void) const;

        //! @brief Current filter retrieval.
        const TraceClass::List GetCurrentFilter(void) const;

        //! @brief Current filter modification.
        //! @return true: Filter is set.
        //! @return false: Trace class not found.
        const bool SetFilter(
            const TraceClass& CLI_Class,    //!< Trace class.
            const bool B_ShowTraces         //!< Show traces flag.
            );

        //! @brief All filter management.
        //!
        //! Same as above but for all filters in one operation.
        const bool SetAllFilter(
            const bool B_ShowTraces         //!< Show traces flag.
            );

    public:
        //! @brief Trace routine.
        //! @return Trace output stream prepared to receive the trace
        //! @note If enabled, the trace is directed to the output stream of the shell corresponding to context element.
        const OutputDevice& Trace(
            const TraceClass& CLI_Class     //!< Trace class.
            );

    private:
        //! @brief Internal trace routine.
        //! @return Trace output stream prepared to receive the trace
        //!
        //! Does not check whether the filter matches.
        const OutputDevice& BeginTrace(
            const TraceClass& CLI_Class     //!< Trace class.
            );

        //! @brief Trace filter states.
        const bool TraceFilterState(
            const TraceClass& CLI_Class,    //!< Trace class.
            const bool B_ShowTraces         //!< Show traces flag.
            );

    private:
        //! @brief Trace class + show flag aggregation.
        class TraceClassFlag : public TraceClass
        {
        public:
            TraceClassFlag(void);
            TraceClassFlag(const TraceClass& CLI_Source, const bool B_Show);
            TraceClassFlag(const TraceClassFlag& CLI_Source);
        public:
            TraceClassFlag& operator=(const TraceClassFlag& CLI_Class);
        public:
            const bool IsVisible(void) const;
        private:
            bool m_bShow;
        };

        //! @brief Trace class map typedef.
        typedef std::map<std::string, TraceClassFlag> ClassMap;

        //! List of available classes.
        ClassMap m_mapClasses;

        //! Trace all flag.
        bool m_bTraceAll;

        //! Traces output stream.
        OutputDevice* m_pcliStream;
    };

    //! @brief Singleton.
    Traces& GetTraces(void);

};

#endif // _CLI_TRACES_H_
