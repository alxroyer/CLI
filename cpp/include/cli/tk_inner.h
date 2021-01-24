/*
    Copyright (c) 2006-2009, Alexis Royer

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
//! @brief Inner cli toolkit definition.

#ifndef _CLI_TK_INNER_H_
#define _CLI_TK_INNER_H_

#include <string.h>
#include <stdio.h>

#include "cli/object.h"


CLI_NS_BEGIN(cli)

    //! @brief CLI classes toolkit.
    CLI_NS_BEGIN(tk)

        //! @brief Basic string object.
        class String : public cli::Object
        {
        public:
            //! @brief Concatenation.
            static const tk::String Concat(
                    const unsigned int UI_MaxLength,    //!< Maximum length.
                    const char* const STR_1,            //!< First string.
                    const char* const STR_2             //!< Second string.
                    )
            {
                tk::String str_Result(UI_MaxLength);
                str_Result.Append(STR_1);
                str_Result.Append(STR_2);
                return str_Result;
            }

            //! @brief Concatenation.
            static const tk::String Concat(
                    const unsigned int UI_MaxLength,    //!< Maximum length.
                    const char* const STR_1,            //!< First string.
                    const char* const STR_2,            //!< Second string.
                    const char* const STR_3             //!< Third string.
                    )
            {
                tk::String str_Result(Concat(UI_MaxLength, STR_1, STR_2));
                str_Result.Append(STR_3);
                return str_Result;
            }

            //! @brief Concatenation.
            static const tk::String Concat(
                    const unsigned int UI_MaxLength,    //!< Maximum length.
                    const char* const STR_1,            //!< First string.
                    const char* const STR_2,            //!< Second string.
                    const char* const STR_3,            //!< Third string.
                    const char* const STR_4             //!< Fourth string.
                    )
            {
                tk::String str_Result(Concat(UI_MaxLength, STR_1, STR_2, STR_3));
                str_Result.Append(STR_4);
                return str_Result;
            }

            //! @brief Concatenation.
            static const tk::String Concat(
                    const unsigned int UI_MaxLength,    //!< Maximum length.
                    const char* const STR_1,            //!< First string.
                    const char* const STR_2,            //!< Second string.
                    const char* const STR_3,            //!< Third string.
                    const char* const STR_4,            //!< Fourth string.
                    const char* const STR_5             //!< Fifth string.
                    )
            {
                tk::String str_Result(Concat(UI_MaxLength, STR_1, STR_2, STR_3, STR_4));
                str_Result.Append(STR_5);
                return str_Result;
            }

        private:
            //! @brief No default constructor.
            String(void);

        public:
            //! @brief Constructor.
            String(
                    const unsigned int UI_MaxLen    //!< Maximum string length.
                    )
              : m_uiMaxLen(UI_MaxLen),
                m_strString(new char[UI_MaxLen + 1])
            {
                Reset();
            }

            //! @brief Initial value constructor.
            //! @warning Maximum length inherits the initial string value.
            //!         Objects initialized by this constructor might be constants in general.
            String(
                    const unsigned int UI_MaxLen,   //!< Maximum string length.
                    const char* const STR_String    //!< Initial value.
                    )
              : m_uiMaxLen(UI_MaxLen),
                m_strString(new char[UI_MaxLen + 1])
            {
                Reset();
                Set(STR_String);
            }

            //! @brief Copy constructor.
            String(
                    const String& TK_String //!< Source object.
                    )
              : m_uiMaxLen(TK_String.m_uiMaxLen),
                m_strString(new char[TK_String.m_uiMaxLen + 1])
            {
                Reset();
                Set(TK_String);
            }

            //! @brief Destructor.
            virtual ~String(void)
            {
                if (m_strString != NULL)
                {
                    delete [] m_strString;
                    m_strString = NULL;
                }
            }

        public:
            //! @brief String length accessor.
            const unsigned int GetLength(void) const
            {
                if (m_strString != NULL)
                {
                    return (unsigned int) strlen(m_strString);
                }
                else
                {
                    return 0;
                }
            }

            //! @brief Checks whether the string is empty or not.
            const bool IsEmpty(void) const
            {
                return (GetLength() <= 0);
            }

            //! @brief Conversion operator.
            operator const char* const(void) const
            {
                if (m_strString != NULL)
                {
                    return m_strString;
                }
                else
                {
                    static const char str_Null[] = { '\0' };
                    return str_Null;
                }
            }

            //! @brief Single character accessor.
            const char GetChar(const unsigned int UI_Pos) const
            {
                char c_Char = '\0';
                if (UI_Pos < GetLength())
                {
                    if (m_strString != NULL)
                    {
                        c_Char = m_strString[UI_Pos];
                    }
                }
                return c_Char;
            }

            //! @brief Sub-string computation.
            const tk::String SubString(
                    const unsigned int UI_FirstCharacter,   //!< First character position.
                    const int I_SubStringLength             //!< Sub-string length.
                                                            //!< negative values mean 0.
                    ) const
            {
                tk::String str_SubString(m_uiMaxLen);

                // Determine copy length.
                unsigned int ui_CopyLen = I_SubStringLength;
                if (str_SubString.m_uiMaxLen < ui_CopyLen)
                    ui_CopyLen = str_SubString.m_uiMaxLen;
                if (GetLength() - UI_FirstCharacter < ui_CopyLen)
                    ui_CopyLen = GetLength() - UI_FirstCharacter;

                // Copy.
                if ((m_strString != NULL) && (UI_FirstCharacter < GetLength())
                    && (str_SubString.m_strString != NULL) && (ui_CopyLen > 0))
                {
                    strncpy(str_SubString.m_strString, & m_strString[UI_FirstCharacter], ui_CopyLen);
                }

                return str_SubString;
            }

        public:
            //! @brief String resetting.
            const bool Reset(void)
            {
                bool b_Result = false;
                if (m_strString != NULL)
                {
                    memset(m_strString, '\0', m_uiMaxLen + 1);
                    b_Result = true;
                }
                return b_Result;
            }

            //! @brief String setting.
            const bool Set(const char* const STR_String)
            {
                return (Reset() && Append(STR_String));
            }

            //! @brief String appending.
            const bool Append(const char* const STR_String)
            {
                bool b_Result = false, b_FullAppend = true;

                if (STR_String != NULL)
                {
                    // Determine copy length.
                    unsigned int ui_CopyLen = (unsigned int) strlen(STR_String);
                    if (ui_CopyLen + GetLength() > m_uiMaxLen)
                    {
                        ui_CopyLen = m_uiMaxLen - GetLength();
                        b_FullAppend = false;
                    }

                    // Copy.
                    if ((m_strString != NULL) && (GetLength() < m_uiMaxLen))
                    {
                        if (ui_CopyLen > 0)
                        {
                            strncpy(& m_strString[GetLength()], STR_String, ui_CopyLen);
                        }
                        b_Result = true;
                    }
                }

                return (b_Result && b_FullAppend);
            }

            //! @brief String appending.
            const bool Append(const char C_Character)
            {
                const char str_Buffer[] = { C_Character, '\0' };
                return Append(str_Buffer);
            }

            //! @brief Assignment operator.
            //! @warning Prefer Set() method since this method indicates failures.
            tk::String& operator=(const tk::String& STR_String)
            {
                Set(STR_String);
                return *this;
            }

        private:
            //! String maximum length.
            const unsigned int m_uiMaxLen;

            //! String buffer.
            char* m_strString;
        };


        //! @brief Basic queue object.
        template <class T> class Queue : public cli::Object
        {
        private:
            //! @brief No default constructor.
            Queue(void);

        public:
            //! @brief Main constructor.
            Queue(
                    const unsigned int UI_MaxCount  //!< Maximum item count.
                    )
              : m_uiMaxCount(UI_MaxCount),
                m_arptQueue(new T*[UI_MaxCount]),
                m_uiCount(0)
            {
                if (m_arptQueue != NULL)
                {
                    memset(m_arptQueue, '\0', sizeof(T*) * UI_MaxCount);
                }
            }

            //! @brief Copy constructor.
            Queue(
                    const Queue<T>& TK_Queue    //!< Source queue object.
                    )
              : m_uiMaxCount(TK_Queue.m_uiMaxCount),
                m_arptQueue(new T*[TK_Queue.m_uiMaxCount]),
                m_uiCount(0)
            {
                if (m_arptQueue != NULL)
                {
                    memset(m_arptQueue, '\0', sizeof(T*) * TK_Queue.m_uiMaxCount);
                }
                if ((m_arptQueue != NULL) && (TK_Queue.m_arptQueue != NULL))
                {
                    for (   m_uiCount = 0;
                            (m_uiCount < TK_Queue.m_uiCount) && (m_uiCount < TK_Queue.m_uiMaxCount);
                            m_uiCount ++)
                    {
                        if (T* const pt_Element = TK_Queue.m_arptQueue[m_uiCount])
                        {
                            m_arptQueue[m_uiCount] = new T(*pt_Element);
                        }
                    }
                }
            }

            //! @brief Destructor.
            virtual ~Queue(void)
            {
                while (! IsEmpty())
                {
                    RemoveTail();
                }
                if (m_arptQueue != NULL)
                {
                    delete [] m_arptQueue;
                    m_arptQueue = NULL;
                }
            }

        private:
            //! @brief No assignment operator.
            Queue& operator=(const Queue&);

        public:
            //! @brief Determines whether the queue is empty.
            const bool IsEmpty(void) const
            {
                return (m_uiCount == 0);
            }

            //! @brief Item count.
            const unsigned int GetCount(void) const
            {
                return m_uiCount;
            }

        public:
            //! @brief Iterator object.
            class Iterator : public cli::Object
            {
            private:
                //! @brief Default constructor.
                Iterator(void) : m_uiIndex(0) {}

            public:
                //! @brief Copy constructor.
                Iterator(const Iterator& CLI_Iterator) : m_uiIndex(CLI_Iterator.m_uiIndex) {}

            private:
                //! Internal data.
                unsigned int m_uiIndex;

                friend class Queue;
            };

            //! @brief Iterator retrieval.
            Iterator GetIterator(void) const
            {
                return Iterator();
            }

            //! @brief Checks the element at the given position is valid.
            const bool IsValid(const Iterator& it) const
            {
                return (it.m_uiIndex < m_uiCount);
            }

            //! @brief Iterates the iterator.
            const bool MoveNext(Iterator& it) const
            {
                it.m_uiIndex ++;
                return IsValid(it);
            }

            //! @brief Read-only item retrieval.
            const T& GetAt(const Iterator& it) const
            {
                const T* pt_Element = NULL;
                if ((m_arptQueue != NULL)
                    && (it.m_uiIndex < m_uiCount) && (it.m_uiIndex < m_uiMaxCount))
                {
                    pt_Element = m_arptQueue[it.m_uiIndex];
                }
                return *pt_Element;
            }

            //! @brief Modifiable item retrieval.
            T& GetAt(const Iterator& it)
            {
                return const_cast<T&>(
                    const_cast<const Queue<T>*>(this)->GetAt(it)
                );
            }

            //! @brief Item removal.
            //! @param it Position. Set to next item.
            //! @return The removed element.
            const T Remove(Iterator& it)
            {
                const T* pt_Element = NULL;
                if (m_arptQueue != NULL)
                {
                    if ((it.m_uiIndex < m_uiCount) && (it.m_uiIndex < m_uiMaxCount))
                    {
                        pt_Element = m_arptQueue[it.m_uiIndex];
                    }
                }
                if (pt_Element != NULL)
                {
                    // Switch elements after on the left.
                    for (   unsigned int ui_Index = it.m_uiIndex;
                            (ui_Index < m_uiCount - 1) && (ui_Index < m_uiMaxCount - 1);
                            ui_Index ++)
                    {
                        m_arptQueue[ui_Index] = m_arptQueue[ui_Index + 1];
                        m_arptQueue[ui_Index + 1] = NULL;
                    }

                    // Decrement element count.
                    m_uiCount --;
                }

                T t_Element(*pt_Element);
                if (pt_Element != NULL)
                {
                    delete pt_Element;
                }
                return t_Element;
            }

        public:
            //! @brief Add a new element at the head of the queue.
            const bool AddHead(
                    const T& T_Element          //!< New element.
                    )
            {
                bool b_Result = false;
                if ((m_arptQueue != NULL) && (m_uiCount < m_uiMaxCount))
                {
                    for (unsigned int ui_Index = m_uiCount; ui_Index > 0; ui_Index --)
                    {
                        m_arptQueue[ui_Index] = m_arptQueue[ui_Index - 1];
                    }
                    m_arptQueue[0] = new T(T_Element);
                    m_uiCount ++;
                    b_Result = true;
                }
                return b_Result;
            }

            //! @brief Add a new element at the tail of the queue.
            const bool AddTail(
                    const T& T_Element          //!< New element.
                    )
            {
                bool b_Result = false;
                if ((m_arptQueue != NULL) && (m_uiCount < m_uiMaxCount))
                {
                    m_arptQueue[m_uiCount] = new T(T_Element);
                    m_uiCount ++;
                    b_Result = true;
                }
                return b_Result;
            }

            //! @brief First item accessor of the read-only queue.
            const T& GetHead(void) const
            {
                const T* pt_Element = NULL;
                if ((m_arptQueue != NULL) && (m_uiCount > 0) && (m_uiMaxCount > 0))
                {
                    pt_Element = m_arptQueue[0];
                }
                return *pt_Element;
            }

            //! @brief First item accessor of the modifiable queue.
            T& GetHead(void)
            {
                return const_cast<T&>(
                    const_cast<const Queue<T>*>(this)->GetHead()
                );
            }

            //! @brief Last item accessor of the read-only queue.
            const T& GetTail(void) const
            {
                const T* pt_Element = NULL;
                if ((m_arptQueue != NULL) && (m_uiCount > 0) && (m_uiCount <= m_uiMaxCount))
                {
                    pt_Element = m_arptQueue[m_uiCount - 1];
                }
                return *pt_Element;
            }

            //! @brief Last item accessor of the modifiable queue.
            T& GetTail(void)
            {
                return const_cast<T&>(
                    const_cast<const Queue<T>*>(this)->GetTail()
                );
            }

            //! @brief Add a new element at the head of the queue.
            const T RemoveHead(void)
            {
                Iterator it;
                it.m_uiIndex = 0;
                return Remove(it);
            }

            //! @brief Add a new element at the tail of the queue.
            const T RemoveTail(void)
            {
                Iterator it;
                it.m_uiIndex = m_uiCount - 1;
                return Remove(it);
            }

        public:
            //! @brief Sort the list according to the given comparison function.
            const bool Sort(
                    const int (*cmp)(const T&, const T&)    //!< Comparison function.
                                                                //!< Return positive value when then second argument should follow first one.
                    )
            {
                bool b_Result = true;
                if (GetCount() < 2)
                {
                    b_Result = true;
                }
                else
                {
                    // Get a reference element.
                    T t_Ref = RemoveTail();

                    // Dispatch elements in q_1 and q_2.
                    Queue<T> q_1(m_uiMaxCount), q_2(m_uiMaxCount);
                    while (! IsEmpty())
                    {
                        T t_Sort = RemoveTail();
                        if (cmp(t_Sort, t_Ref) > 0)
                        {
                            q_1.AddTail(t_Sort);
                        }
                        else
                        {
                            q_2.AddTail(t_Sort);
                        }
                    }

                    // Sort each list q_1 and q_2
                    if (q_1.Sort(cmp) && q_2.Sort(cmp))
                    {
                        // Eventually restore q_1...
                        b_Result = true;
                        for (   Iterator it_1 = q_1.GetIterator();
                                q_1.IsValid(it_1);
                                q_1.MoveNext(it_1))
                        {
                            if (! AddTail(q_1.GetAt(it_1)))
                            {
                                b_Result = false;
                            }
                        }
                        // ... then the reference element...
                        if (! AddTail(t_Ref))
                        {
                            b_Result = false;
                        }
                        // ... and then q_2.
                        for (   Iterator it_2 = q_2.GetIterator();
                                q_2.IsValid(it_2);
                                q_2.MoveNext(it_2))
                        {
                            if (! AddTail(q_2.GetAt(it_2)))
                            {
                                b_Result = false;
                            }
                        }
                    }
                }
                return b_Result;
            }

        protected:
            //! Maximum queue count.
            unsigned int m_uiMaxCount;

            //! Internal queue buffer.
            T** m_arptQueue;

            //! Queue count.
            unsigned int m_uiCount;
        };

        //! @brief Basic map object.
        template <class K, class T> class Map : public cli::Object
        {
        private:
            //! @brief Key-value association class.
            class Pair
            {
            public:
                //! @brief Constructor.
                Pair(const K& K_Key, const T& T_Value) : m_kKey(K_Key), m_tValue(T_Value) {}
                Pair(const Pair& p) : m_kKey(p.m_kKey), m_tValue(p.m_tValue) {}
                virtual ~Pair(void) {}
            public:
                const K m_kKey;
                T m_tValue;
            };

        private:
            //! @brief No default constructor.
            Map(void);

        public:
            //! @brief Main constructor.
            Map(
                    const unsigned int UI_MaxCount  //!< Maximum item count.
                    )
              : m_qPairs(UI_MaxCount)
            {
            }

            //! @brief Copy constructor.
            Map(
                    const Map<K,T>& TK_Map      //!< Source map object.
                    )
              : m_qPairs(TK_Map.m_qPairs)
            {
            }

            //! @brief Destructor.
            virtual ~Map(void)
            {
            }

        private:
            //! @brief No assignment operator.
            Map& operator=(const Map&);

        public:
            //! @brief Set a new item.
            //! @return true if the element has been set.
            //! @return false if the element has not been set.
            const bool SetAt(const K& K_Key, const T& T_Value)
            {
                for (   Iterator it = m_qPairs.GetIterator();
                        m_qPairs.IsValid(it);
                        m_qPairs.MoveNext(it))
                {
                    if (m_qPairs.GetAt(it).m_kKey == K_Key)
                    {
                        Remove(it);
                        break;
                    }
                }

                return m_qPairs.AddTail(Pair(K_Key, T_Value));
            }

            //! @brief Unset an item.
            //! @return true if the element has been unset correctly, or if the element was not set.
            //! @return false if an error occured.
            const bool Unset(const K& K_Key)
            {
                for (   Iterator it = m_qPairs.GetIterator();
                        m_qPairs.IsValid(it);
                        m_qPairs.MoveNext(it))
                {
                    if (m_qPairs.GetAt(it).m_kKey == K_Key)
                    {
                        m_qPairs.Remove(it);
                        return true;
                    }
                }

                return true;
            }

            //! @brief Checks whether an element is set for this key.
            const bool IsSet(const K& K_Key) const
            {
                for (   Iterator it = m_qPairs.GetIterator();
                        m_qPairs.IsValid(it);
                        m_qPairs.MoveNext(it))
                {
                    if (m_qPairs.GetAt(it).m_kKey == K_Key)
                    {
                        return true;
                    }
                }

                return false;
            }

            //! @brief Element accessor.
            //! @return NULL if no element is set for this key.
            const T* const GetAt(const K& K_Key) const
            {
                for (   Iterator it = m_qPairs.GetIterator();
                        m_qPairs.IsValid(it);
                        m_qPairs.MoveNext(it))
                {
                    if (m_qPairs.GetAt(it).m_kKey == K_Key)
                    {
                        return & m_qPairs.GetAt(it).m_tValue;
                    }
                }

                return NULL;
            }


        public:
            //! @brief Iterator object.
            typedef typename Queue<Pair>::Iterator Iterator;

            //! @brief Iterator retrieval.
            Iterator GetIterator(void) const
            {
                return m_qPairs.GetIterator();
            }

            //! @brief Checks the element at the given position is valid.
            const bool IsValid(const Iterator& it) const
            {
                return m_qPairs.IsValid(it);
            }

            //! @brief Iterates the iterator.
            const bool MoveNext(Iterator& it) const
            {
                return m_qPairs.MoveNext(it);
            }

            //! @brief Key retrieval.
            const K& GetKey(const Iterator& it) const
            {
                return m_qPairs.GetAt(it).m_kKey;
            }

            //! @brief Read-only item retrieval.
            const T& GetAt(const Iterator& it) const
            {
                return m_qPairs.GetAt(it).m_tValue;
            }

            //! @brief Modifiable item retrieval.
            T& GetAt(const Iterator& it)
            {
                return m_qPairs.GetAt(it).m_tValue;
            }

            //! @brief Item removal.
            //! @param it Position. Set to next item.
            //! @return The remove element.
            const T Remove(Iterator& it)
            {
                return m_qPairs.Remove(it).m_tValue;
            }

            //! @brief Determines whether the map is empty.
            const bool IsEmpty(void) const
            {
                return m_qPairs.IsEmpty();
            }

            //! @brief Item count.
            const unsigned int GetCount(void) const
            {
                return m_qPairs.GetCount();
            }

        private:
            //! Internal pair list.
            Queue<Pair> m_qPairs;
        };

    CLI_NS_END(tk)

CLI_NS_END(cli)

#endif // _CLI_TK_INNER_H_

