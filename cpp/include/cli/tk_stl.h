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


//! @file
//! @author Alexis Royer
//! @brief STL toolkit definition.

#ifndef _CLI_TK_STL_H_
#define _CLI_TK_STL_H_

#include <string>
#include <deque>
#include <map>

#include <cli/object.h>


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
            {
            }

            //! @brief Initial value constructor.
            //! @warning Maximum length inherits the initial string value.
            //!         Objects initialized by this constructor might be constants in general.
            String(
                    const unsigned int UI_MaxLen,   //!< Maximum string length.
                    const char* const STR_String    //!< Initial value.
                    )
              : m_stlString(STR_String)
            {
            }

            //! @brief Copy constructor.
            String(
                    const String& TK_String //!< Source object.
                    )
              : m_stlString((const char* const) TK_String)
            {
            }

            //! @brief Destructor.
            virtual ~String(void)
            {
            }

        public:
            //! @brief String length accessor.
            const unsigned int GetLength(void) const
            {
                return m_stlString.size();
            }

            //! @brief Checks whether the string is empty or not.
            const bool IsEmpty(void) const
            {
                return (GetLength() <= 0);
            }

            //! @brief Conversion operator.
            operator const char* const(void) const
            {
                return m_stlString.c_str();
            }

            //! @brief Single character accessor.
            const char GetChar(const unsigned int UI_Pos) const
            {
                char c_Char = '\0';
                if (UI_Pos < m_stlString.size())
                {
                    c_Char = m_stlString[UI_Pos];
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
                tk::String str_SubString(0);

                // Determine copy length.
                unsigned int ui_CopyLen = I_SubStringLength;
                if (GetLength() - UI_FirstCharacter < ui_CopyLen)
                    ui_CopyLen = GetLength() - UI_FirstCharacter;

                // Copy.
                if ((UI_FirstCharacter < GetLength()) && (ui_CopyLen > 0))
                {
                    str_SubString.Append(m_stlString.substr(UI_FirstCharacter, ui_CopyLen).c_str());
                }

                return str_SubString;
            }

        public:
            //! @brief String resetting.
            const bool Reset(void)
            {
                m_stlString.erase();
                return true;
            }

            //! @brief String setting.
            const bool Set(const char* const STR_String)
            {
                m_stlString = STR_String;
                return true;
            }

            //! @brief String appending.
            const bool Append(const char* const STR_String)
            {
                m_stlString += STR_String;
                return true;
            }

            //! @brief String appending.
            const bool Append(const char C_Character)
            {
                m_stlString += C_Character;
                return true;
            }

            //! @brief Assignment operator.
            //! @warning Prefer Set() method since this method indicates failures.
            tk::String& operator=(const tk::String& STR_String)
            {
                m_stlString = STR_String.m_stlString;
                return *this;
            }

        private:
            //! String buffer.
            std::string m_stlString;
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
            {
            }

            //! @brief Copy constructor.
            Queue(
                    const Queue<T>& TK_Queue    //!< Source queue object.
                    )
              : m_stlQueue(TK_Queue.m_stlQueue)
            {
            }

            //! @brief Destructor.
            virtual ~Queue(void)
            {
            }

        private:
            //! @brief No assignment operator.
            Queue& operator=(const Queue&);

        public:
            //! @brief Determines whether the queue is empty.
            const bool IsEmpty(void) const
            {
                return m_stlQueue.empty();
            }

            //! @brief Item count.
            const unsigned int GetCount(void) const
            {
                return m_stlQueue.size();
            }

        public:
            //! @brief Iterator object.
            typedef class std::deque<T>::iterator Iterator;

            //! @brief Iterator retrieval.
            Iterator GetIterator(void) const
            {
                return const_cast<std::deque<T>&>(m_stlQueue).begin();
            }

            //! @brief Checks the element at the given position is valid.
            const bool IsValid(const Iterator& it) const
            {
                return (it != m_stlQueue.end());
            }

            //! @brief Iterates the iterator.
            const bool MoveNext(Iterator& it) const
            {
                it ++;
                return IsValid(it);
            }

            //! @brief Read-only item retrieval.
            const T& GetAt(const Iterator& it) const
            {
                return *it;
            }

            //! @brief Modifiable item retrieval.
            T& GetAt(const Iterator& it)
            {
                return *it;
            }

            //! @brief Item removal.
            //! @param it Position. Set to next item.
            //! @return The removed element.
            const T Remove(Iterator& it)
            {
                const T t_Element = *it;
                m_stlQueue.erase(it);
                return t_Element;
            }

        public:
            //! @brief Add a new element at the head of the queue.
            const bool AddHead(
                    const T& T_Element          //!< New element.
                    )
            {
                m_stlQueue.push_front(T_Element);
                return true;
            }

            //! @brief Add a new element at the tail of the queue.
            const bool AddTail(
                    const T& T_Element          //!< New element.
                    )
            {
                m_stlQueue.push_back(T_Element);
                return true;
            }

            //! @brief First item accessor of the read-only queue.
            const T& GetHead(void) const
            {
                return m_stlQueue.front();
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
                return m_stlQueue.back();
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
                const T t_Element = m_stlQueue.front();
                m_stlQueue.pop_front();
                return t_Element;
            }

            //! @brief Add a new element at the tail of the queue.
            const T RemoveTail(void)
            {
                const T t_Element = m_stlQueue.back();
                m_stlQueue.pop_back();
                return t_Element;
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
                    Queue<T> q_1(0), q_2(0);
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
            //! Internal queue buffer.
            std::deque<T> m_stlQueue;
        };

        //! @brief Basic map object.
        template <class K, class T> class Map : public cli::Object
        {
        private:
            //! @brief No default constructor.
            Map(void);

        public:
            //! @brief Main constructor.
            Map(
                    const unsigned int UI_MaxCount  //!< Maximum item count.
                    )
            {
            }

            //! @brief Copy constructor.
            Map(
                    const Map<K,T>& TK_Map      //!< Source map object.
                    )
              : m_stlMap(TK_Map.m_stlMap)
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
                if (m_stlMap.count(K_Key) > 0)
                {
                    m_stlMap.erase(K_Key);
                }
                m_stlMap.insert(std::make_pair(K_Key, T_Value));
                return true;
            }

            //! @brief Unset an item.
            //! @return true if the element has been unset correctly, or if the element was not set.
            //! @return false if an error occured.
            const bool Unset(const K& K_Key)
            {
                m_stlMap.erase(K_Key);
                return true;
            }

            //! @brief Checks whether an element is set for this key.
            const bool IsSet(const K& K_Key) const
            {
                return (m_stlMap.count(K_Key) ? true : false);
            }

            //! @brief Element accessor.
            //! @return NULL if no element is set for this key.
            const T* const GetAt(const K& K_Key) const
            {
                typename std::map<K,T>::const_iterator it = m_stlMap.find(K_Key);
                if (it != m_stlMap.end())
                {
                    return & it->second;
                }
                else
                {
                    return NULL;
                }
            }

        public:
            //! @brief Iterator object.
            typedef class std::map<K,T>::iterator Iterator;

            //! @brief Iterator retrieval.
            Iterator GetIterator(void) const
            {
                return const_cast<std::map<K,T>&>(m_stlMap).begin();
            }

            //! @brief Checks the element at the given position is valid.
            const bool IsValid(const Iterator& it) const
            {
                return (it != m_stlMap.end());
            }

            //! @brief Iterates the iterator.
            const bool MoveNext(Iterator& it) const
            {
                it ++;
                return IsValid(it);
            }

            //! @brief Key retrieval.
            const K& GetKey(const Iterator& it) const
            {
                return it->first;
            }

            //! @brief Read-only item retrieval.
            const T& GetAt(const Iterator& it) const
            {
                return it->second;
            }

            //! @brief Modifiable item retrieval.
            T& GetAt(const Iterator& it)
            {
                return it->second;
            }

            //! @brief Item removal.
            //! @param it Position. Set to next item.
            //! @return The remove element.
            const T Remove(Iterator& it)
            {
                const T t_Element = GetAt(it);
                m_stlMap.erase(it);
                return t_Element;
            }

            //! @brief Determines whether the map is empty.
            const bool IsEmpty(void) const
            {
                return m_stlMap.empty();
            }

            //! @brief Item count.
            const unsigned int GetCount(void) const
            {
                return m_stlMap.size();
            }

        private:
            //! Internal pair list.
            std::map<K,T> m_stlMap;
        };

    CLI_NS_END(tk)

CLI_NS_END(cli)

#endif // _CLI_TK_STL_H_

