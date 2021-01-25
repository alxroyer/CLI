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

#ifndef CLI_NO_STL
#include <string>
#endif

#include "cli/string_device.h"


static const bool CheckOutput(const char* const STR_Method, const cli::OutputDevice& CLI_Out, const char* const STR_Expected)
{
    if (const cli::StringDevice* const pcli_Out = dynamic_cast<const cli::StringDevice*>(& CLI_Out))
    {
        cli::StringDevice& cli_Out = const_cast<cli::StringDevice&>(*pcli_Out);
        if (cli_Out.GetString() != STR_Expected)
        {
            cli::OutputDevice::GetStdErr()
                << STR_Method << ": "
                << "Bad output: '" << cli_Out.GetString() << "', "
                << "Expected: '" << STR_Expected << "'"
                << cli::endl;
            return false;
        }
        cli_Out.Reset();
        return true;
    }
    return false;
}

int main(int I_Args, const char* ARSTR_Args[])
{
    cli::StringDevice cli_Out(256, false);

    #ifndef CLI_NO_STL
    // STL string
    if (! CheckOutput(  "operator<<(const stl::string&)",       cli_Out << std::string("Hello"),            "Hello"         )) return -1;
    #endif

    // cli::tk::String
    if (! CheckOutput(  "operator<<(const cli::tk::String&)",   cli_Out << cli::tk::String(10, "Bonjour"),  "Bonjour"       )) return -1;
    // const char* const
    if (! CheckOutput(  "operator<<(const char* const)",        cli_Out << "Hola",                          "Hola"          )) return -1;
    // Single char
    if (! CheckOutput(  "operator<<(const char)",               cli_Out << 'W',                             "W"             )) return -1;
    // cli::KEY
    if (! CheckOutput(  "operator<<(const cli::KEY)",           cli_Out << cli::TAB,                        "\t"            )) return -1;

    // uint8_t
    if (! CheckOutput(  "operator<<(const uint8_t)",            cli_Out << (uint8_t) 8,                     "8"             )) return -1;

    // int16_t
    if (! CheckOutput(  "operator<<(const int16_t)",            cli_Out << (int16_t) -16,                   "-16"           )) return -1;
    // uint16_t
    if (! CheckOutput(  "operator<<(const uint16_t)",           cli_Out << (uint16_t) 0x0100,               "256"           )) return -1;

    // int32_t
    if (! CheckOutput(  "operator<<(const int32_t)",            cli_Out << (int32_t) -32,                   "-32"           )) return -1;
    // uint32_t
    if (! CheckOutput(  "operator<<(const uint32_t)",           cli_Out << (uint32_t) 0x10000,              "65536"         )) return -1;

    // int64_t
    if (! CheckOutput(  "operator<<(const int64_t)",            cli_Out << (int64_t) -64,                   "-64"           )) return -1;
    // uint64_t
    if (! CheckOutput(  "operator<<(const uint64_t)",           cli_Out << (uint64_t) 0x100000000,          "4294967296"    )) return -1;

    // float
    if (! CheckOutput(  "operator<<(const float)",              cli_Out << (float) 6.55957,                 "6.55957"       )) return -1;
    // double
    if (! CheckOutput(  "operator<<(const double)",             cli_Out << 3.14,                            "3.14"          )) return -1;
    if (! CheckOutput(  "operator<<(const double)",             cli_Out << .1230,                           "0.123"         )) return -1;
    if (! CheckOutput(  "operator<<(const double)",             cli_Out << (-2.5 * 1000.0 * 1000.0),        "-2.5e6"        )) return -1;
    if (! CheckOutput(  "operator<<(const double)",             cli_Out << 0.025e-6,                        "25.0e-9"       )) return -1;

    // void*
    if (! CheckOutput(  "operator<<(const void* const)",        cli_Out << (void*) 0x00012345,              "0x12345"       )) return -1;

    // cli::endl
    if (! CheckOutput(  "operator<<(cli::endl)",                cli_Out << cli::endl,                       "\n"            )) return -1;

    return 0;
}
