/*
    Copyright (c) 2006-2009, Alexis Royer, http://alexis.royer.free.fr/CLI

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


//! @file
//! @author Alexis Royer
//! @brief Traces management system definition.
//!
//! Defined by Traces and TraceClass classes.
//! Use the singleton cli::GetTraces.

#ifndef _CLI_TRACES_H_
#define _CLI_TRACES_H_

#include "cli/namespace.h"
#include "cli/object.h"
#include "cli/help.h"
#include "cli/tk.h"


CLI_NS_BEGIN(cli)

    // Forward declarations.
    class OutputDevice;


    //! @brief Trace class object.
    class TraceClass : public Object
    {
    public:
        //! @brief Trace class list typedef.
        typedef tk::Queue<TraceClass> List;

    private:
        //! @brief No default constructor.
        TraceClass(void);

    public:
        //! @brief Copy constructor.
        TraceClass(
            const TraceClass& CLI_Class         //!< Source class object.
            );

        //! @brief Constructor.
        TraceClass(
            const char* const STR_ClassName,    //!< Class name.
            const Help& CLI_Help                //!< Help description.
            );

        //! @brief Destructor.
        virtual ~TraceClass(void);

    public:
        //! @brief Class name accessor.
        const tk::String GetName(void) const;

        //! @brief Description accessor.
        const Help& GetHelp(void) const;

    public:
        //! @brief Assignment operator.
        TraceClass& operator=(const TraceClass&);

    private:
        //! Class name.
        tk::String m_strName;

        //! Description.
        Help m_cliHelp;
    };

    //! @brief Classes equivalence operator.
    const bool operator==(
        const TraceClass& CLI_Class1,   //!< First member.
        const TraceClass& CLI_Class2    //!< Second member.
        );

    //! @brief Internal error common trace class singleton redirection.
    #define INTERNAL_ERROR GetInternalErrorTraceClass()
    //! @brief Internal error common trace class singleton.
    const TraceClass& GetInternalErrorTraceClass();


    //! @brief Traces service.
    class Traces : public Object
    {
    public:
        //! @brief Singleton.
        static Traces& GetInstance(void);

    protected:
        //! @brief Default constructor.
        Traces(void);

    private:
        //! @brief No copy constructor.
        Traces(const Traces&);

    public:
        //! @brief Destructor.
        virtual ~Traces(void);

    private:
        //! @brief No assignment operator.
        Traces& operator=(const Traces&);

    public:
        //! @brief Stream access.
        const OutputDevice& GetStream(void) const;

        //! @brief Stream positionning.
        //! @warning For consistency reasons, if you use this method,
        //!          you should better call UnsetStream() before program termination.
        const bool SetStream(
            OutputDevice& CLI_Stream                //!< Stream reference.
            );

        //! @brief Stream dereferencing.
        //!
        //! This method should be called if you have previously called SetStream().
        //! You may also call UnsetStream() even if SetStream() has been been previously called.
        const bool UnsetStream(void);

        //! @brief Determines whether the trace stream has already been set.
        //! @return true: The stream has already been set.
        //! @return false: No stream has been set yet.
        const bool IsStreamSet(void) const;

    public:
        //! @brief Trace class declaration.
        //! @return true if the class has been declared successfully or the class was already declared.
        //! @return false if an error as occured.
        const bool Declare(
            const TraceClass& CLI_Class     //!< Trace class to declare.
            );

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
        //! @brief Trace status.
        //! @return true if the trace is active, false otherwise.
        const bool IsTraceOn(
            const TraceClass& CLI_Class     //!< Trace class.
            ) const;

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

        //! @brief Trace filter state display.
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
            virtual ~TraceClassFlag(void);
        public:
            TraceClassFlag& operator=(const TraceClassFlag& CLI_Class);
        public:
            const bool IsVisible(void) const;
        private:
            bool m_bShow;
        };

        //! @brief Trace class map typedef.
        typedef tk::Map<tk::String, TraceClassFlag> ClassMap;

        //! List of available classes.
        ClassMap m_mapClasses;

        //! Trace all flag.
        bool m_bTraceAll;

        //! Traces output stream.
        OutputDevice* m_pcliStream;
    };

    //! @brief Singleton.
    //! @return The only one Traces instance.
    Traces& GetTraces(void);

CLI_NS_END(cli)

#endif // _CLI_TRACES_H_
