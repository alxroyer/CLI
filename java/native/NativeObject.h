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

#ifndef _CLI_NATIVE_OBJECT_H_
#define _CLI_NATIVE_OBJECT_H_

#include <string>
#include <map>

#include "cli/object.h"
#include "cli/io_device.h"

class NativeObject
{
public:
    static const jobject GetJavaObject(JNIEnv* const PJ_Env, int I_NativeObjectRef);

public:
    template <class T> static void Use(
        const T* const PT_Object            //!< Object in use.
        )
    {
        if (cli::OutputDevice* const pcli_OutputDevice = dynamic_cast<cli::OutputDevice*>(
                const_cast<T*>(PT_Object)))
        {
            pcli_OutputDevice->UseInstance(__CALL_INFO__);
        }
        Use((int) PT_Object);
    }

    template <class T> static void Free(
        const T* const PT_Object            //!< Object which reference is released.
        )
    {
        const bool b_Delete = Free((int) PT_Object);
        if (cli::OutputDevice* const pcli_OutputDevice = dynamic_cast<cli::OutputDevice*>(
                const_cast<T*>(PT_Object)))
        {
            pcli_OutputDevice->FreeInstance(__CALL_INFO__);
        }
        else if ((b_Delete) && (PT_Object != NULL))
        {
            delete PT_Object;
        }
    }

    template <class T1, class T2> static void Delegate(
        const T1* const PT_What,            //!< Object which deletion is now delegeted.
        const T2* const PT_Who              //!< Object the deletion of the previous object is delegated to.
        )
    {
        Delegate((int) PT_What, (int) PT_Who);
    }

public:
    static const bool CreateFromNative(
        JNIEnv* const PJ_Env,               //!< Java environment.
        const cli::Object& CLI_Object       //!< CLI element to create from native source.
        );

    static const bool DeleteFromNative(
        JNIEnv* const PJ_Env,               //!< Java environment.
        const cli::Object& CLI_Object       //!< CLI element to delete from native source.
        );

private:
    static void Use(
        const int I_ObjectRef               //!< Object in use.
        );

    //! @return true if the object should be deleted.
    static const bool Free(
        const int I_ObjectRef               //!< Object which reference is released.
        );

    static void Delegate(
        const int I_WhatRef,                //!< Object which deletion is now delegeted.
        const int I_WhoRef                  //!< Object the deletion of the previous object is delegated to.
        );

private:
    static const std::string GetJavaClassName(
        const cli::Object& CLI_Object       //!< CLI element.
        );

private:
    //! Object information structure.
    typedef struct _ObjectInfo
    {
        int iTokens;        //!< Token number. When this number falls down 0, perform deletion.
        bool bAutoDelete;   //!< Auto-deletion flag. When not set, do not delete the object on unregistration.
    } ObjectInfo;

    //! Objet map typedef.
    typedef std::map<int, struct _ObjectInfo> ObjectMap;

    //! Object map.
    static ObjectMap m_mapTokens;
};

#endif // _CLI_NATIVE_OBJECT_H_
