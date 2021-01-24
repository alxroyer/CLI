#!/bin/bash

# Copyright (c) 2006-2009, Alexis Royer, http://alexis.royer.free.fr/CLI
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#     * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# print_result
#   $1 <- item text
#   $2 <- number of lines
print_lines()
{
    TEXT_LEN=20
    LINES_LEN=10

    # Indentation
    echo -n " "

    # Item text
    echo -n "$1:"
    n_Blanks=$[$TEXT_LEN-1-$(echo "$1" | wc -c)]
    while [ $n_Blanks -gt 0 ] ; do
        echo -n " "
        n_Blanks=$[$n_Blanks-1]
    done

    # Number of lines
    n_Blanks=$[$LINES_LEN-$(echo "$2" | wc -c)]
    while [ $n_Blanks -gt 0 ] ; do
        echo -n " "
        n_Blanks=$[$n_Blanks-1]
    done
    echo "$2"
}

echo "MISC:"
    MISC_MAKE=$(cat build/make/Makefile build/make/*.mak | wc -l)
    print_lines "misc/make"     $MISC_MAKE
    MISC_DOC=$(cat *.txt | wc -l)
    print_lines "misc/doc"      $MISC_DOC
    MISC_ALL=$[MISC_MAKE+$MISC_DOC]
    print_lines "TOTAL MISC"    $MISC_ALL
    echo ""

echo "CPP: "
    CPP_SRC=$(cat cpp/src/*.* | wc -l)
    print_lines "cpp/src"       $CPP_SRC
    CPP_INCLUDE=$(cat cpp/include/cli/*.* | wc -l)
    print_lines "cpp/include"   $CPP_INCLUDE
    CPP_MAKE=$(cat cpp/build/make/Makefile cpp/build/make/*.mak | wc -l)
    print_lines "cpp/build"     $CPP_MAKE
    CPP_XSL=$(cat cpp/xsl/*.xsl | wc -l)
    print_lines "cpp/xsl"       $CPP_XSL
    CPP_TESTS=$(cat cpp/tests/*.* cpp/tests/*/*.* | wc -l)
    print_lines "cpp/tests"     $CPP_TESTS
    CPP_ALL=$[$CPP_SRC+$CPP_INCLUDE+$CPP_MAKE+$CPP_XSL+$CPP_TESTS]
    print_lines "TOTAL CPP"     $CPP_ALL
    echo ""

echo "JAVA:"
    JAVA_SRC=$(cat java/src/cli/*.java | wc -l)
    print_lines "java/src"      $JAVA_SRC
    JAVA_NATIVE=$(cat java/native/*.* | wc -l)
    print_lines "java/native"   $JAVA_NATIVE
    JAVA_MAKE=$(cat java/build/make/Makefile java/build/make/*.mak | wc -l)
    print_lines "java/build"    $JAVA_MAKE
    JAVA_XSL=$(cat java/xsl/*.xsl | wc -l)
    print_lines "java/xsl"      $JAVA_XSL
    JAVA_TESTS=$(cat java/tests/*/*.* | wc -l)
    print_lines "java/tests"    $JAVA_TESTS
    JAVA_ALL=$[$JAVA_SRC+$JAVA_NATIVE+$JAVA_MAKE+$JAVA_XSL+$JAVA_TESTS]
    print_lines "TOTAL JAVA"    $JAVA_ALL
    echo ""

echo "SAMPLES:"
    SAMPLES_XML=$(find samples -name "*.xml" -exec cat {} \; | wc -l)
    print_lines "samples/xml"   $SAMPLES_XML
    SAMPLES_TEST=$(find samples -name "*.test" -exec cat {} \; | wc -l)
    print_lines "samples/test"  $SAMPLES_TEST
    SAMPLES_CHECK=$(find samples -name "*.check" -exec cat {} \; | wc -l)
    print_lines "samples/check" $SAMPLES_CHECK
    SAMPLES_SRC=$(find samples -name "*.h" -o -name "*.cpp" -o -name "*.java" -exec cat {} \; | wc -l)
    print_lines "samples/src"   $SAMPLES_SRC
    SAMPLES_ALL=$[$SAMPLES_XML+$SAMPLES_TEST+$SAMPLES_CHECK+$SAMPLES_SRC]
    print_lines "TOTAL SAMPLES" $SAMPLES_ALL
    echo ""

echo "WEB:"
    WEB_HTML=$(cat web/index.html | wc -l)
    print_lines "web/html"      $WEB_HTML
    WEB_DB=$(find web -name "*.xml" -exec cat {} \; | wc -l)
    print_lines "web/docbook"   $WEB_DB
    WEB_CSS=$(find web -name "*.css" -exec cat {} \; | wc -l)
    print_lines "web/css"       $WEB_CSS
    WEB_MAKE=$(find web -name "*.mak" -exec cat {} \; | wc -l)
    print_lines "web/make"      $WEB_MAKE
    WEB_XSL=$(find web -name "*.xsl" -exec cat {} \; | wc -l)
    print_lines "web/xsl"       $WEB_XSL
    WEB_ALL=$[$WEB_HTML+$WEB_DB+$WEB_+$WEB_MAKE+$WEB_XSL]
    print_lines "TOTAL WEB"     $WEB_ALL
    echo ""

ALL_ALL=$[$MISC_ALL+$CPP_ALL+$JAVA_ALL+$SAMPLES_ALL+$WEB_ALL]
print_lines "TOTAL"             $ALL_ALL
