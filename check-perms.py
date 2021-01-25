#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (c) 2006-2018, Alexis Royer, http://alexis.royer.free.fr/CLI
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#
#     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
#       and/or other materials provided with the distribution.
#     * Neither the name of the CLI library project nor the names of its contributors may be used to endorse or promote products derived from this software
#       without specific prior written permission.
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

import sys, os


RW = 644
RWX = 755
l_map_extensions = {
    ".a":               [RW, 0],
    ".awk":             [RW, 0],
    ".bat":             [RW, 0],
    ".check":           [RW, 0],
    ".class":           [RW, 0],
    ".cli":             [RW, 0],
    ".cli2help.diff":   [RW, 0],
    ".conf":            [RW, 0],
    ".cpp":             [RW, 0],
    ".css":             [RW, 0],
    ".db":              [RW, 0],
    ".deps":            [RW, 0],
    ".diff":            [RW, 0],
    ".dll":             [RWX, 0],
    ".dot":             [RW, 0],
    ".doxygen":         [RW, 0],
    ".exe":             [RWX, 0],
    ".gif":             [RW, 0],
    ".gitignore":       [RW, 0],
    ".h":               [RW, 0],
    ".h.lite":          [RW, 0],
    ".htm":             [RW, 0],
    ".html":            [RW, 0],
    ".html.lite":       [RW, 0],
    ".jar":             [RW, 0],
    ".java":            [RW, 0],
    ".java.lite":       [RW, 0],
    ".jpg":             [RW, 0],
    ".js":              [RW, 0],
    ".log":             [RW, 0],
    ".log.1":           [RW, 0],
    ".log.2":           [RW, 0],
    ".log.3":           [RW, 0],
    ".log.lite":        [RW, 0],
    ".mak":             [RW, 0],
    ".map":             [RW, 0],
    ".md5":             [RW, 0],
    ".o":               [RW, 0],
    # ".obj":             [RW, 0],
    # ".pdb":             [RW, 0],
    ".png":             [RW, 0],
    ".py":              [RW, 0],
    ".pyc":             None,
    ".rng":             [RW, 0],
    ".sh":              [RWX, 0],
    ".sln":             [RW, 0],
    ".test":            [RW, 0],
    ".txt":             [RW, 0],
    ".vcxproj":         [RW, 0],
    ".vcxproj.filters": [RW, 0],
    ".vcxproj.user":    [RW, 0],
    ".xml":             [RW, 0],
    ".xsd":             [RW, 0],
    ".xsl":             [RW, 0],
    "Makefile":         [RW, 0],
    "package-list":     [RW, 0]
}
def extensions_summary():
    print("")
    print("Extension summary")
    for l_str_ext in sorted(l_map_extensions.keys()):
        if l_map_extensions[l_str_ext] is not None:
            print("  %s: %d files" % (l_str_ext, l_map_extensions[l_str_ext][1]))

l_arstr_errors = []
def info(p_str_path, p_str_info):
    print("%s: %s" % (p_str_path, p_str_info))
def error(p_str_path, p_str_error):
    l_arstr_errors.append("%s: %s" % (p_str_path, p_str_error))
    print("ERROR! %s" % (l_arstr_errors[-1]))
def error_summary():
    print("")
    print("Error summary:")
    for l_str_error in l_arstr_errors:
        print("  %s" % (l_str_error))
    print("%d errors" % (len(l_arstr_errors)))

def get_ext(p_str_path):
    l_str_ext = ""
    l_t_ext = os.path.splitext(p_str_path)
    if l_t_ext[1] == "":
        return os.path.basename(p_str_path)
    else:
        l_str_ext = l_t_ext[1] + l_str_ext
        while (l_str_ext in [".user", ".filters", ".lite", ".1", ".2", ".3"]) and (l_t_ext[1] != ""):
            l_t_ext = os.path.splitext(l_t_ext[0])
            l_str_ext = l_t_ext[1] + l_str_ext
        return l_str_ext

def set_perms(p_str_path, p_i_perms):
    import subprocess
    p = subprocess.Popen(["chmod", "%s" % p_i_perms, p_str_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    l_i_res = p.wait()
    l_str_stdout = p.stdout.read()
    p.stdout.close()
    l_str_stderr = p.stderr.read()
    p.stderr.close()

    if l_str_stderr == "":
        info(p_str_path, "chmod %d" % (p_i_perms))
    else:
        error(p_str_path, l_str_stderr)

def check_perms(p_str_path):
    if p_str_path != "":
        if not os.path.exists(p_str_path):
            error(p_str_path, "not a valid file/directory")
            return

    l_arstr_skipped_items = [
        ".git/",
        # "cpp/build/make/Cygwing++/",
        "cpp/build/VisualStudio/.vs/",
        "cpp/build/VisualStudio/Debug/",
        "cpp/build/VisualStudio/Release/",
        "cpp/build/NetBeans/",
        "java/build/NetBeans/",
    ]
    for l_str_skipped_item in l_arstr_skipped_items:
        if (
            os.path.abspath(p_str_path).startswith(os.path.abspath(l_str_skipped_item))
            and not (l_str_skipped_item.endswith("/") and os.path.isfile(p_str_path))
        ):
            info(p_str_path, "skipped file/directory '%s'" % (l_str_skipped_item))
            return

    if (p_str_path == "") or os.path.isdir(p_str_path):
        set_perms("." if p_str_path == "" else p_str_path, RWX)
        for l_str_path in os.listdir("." if p_str_path == "" else p_str_path):
            if p_str_path != "":
                l_str_path = os.path.join(p_str_path, l_str_path)
            try:
                check_perms(l_str_path)
            except Exception, e:
                error(l_str_path, "exception: %s" % (str(e)))
    elif os.path.isfile(p_str_path):
        l_str_ext = get_ext(p_str_path)
        l_i_perms = None
        if (l_str_ext == ".py") and (os.path.dirname(p_str_path) == ""):
            l_i_perms = RWX
        elif (l_str_ext == ".py") and p_str_path.startswith("tools/cli2"):
            l_i_perms = RWX
        elif l_map_extensions.has_key(l_str_ext):
            if l_map_extensions[l_str_ext] is not None:
                l_i_perms = l_map_extensions[l_str_ext][0]
        else:
            error(p_str_path, "unknown extension '%s'" % (l_str_ext))
        if l_i_perms is not None:
            l_map_extensions[l_str_ext][1] += 1
            set_perms(p_str_path, l_i_perms)

# Find out the files.
if len(sys.argv) > 1:
    for l_str_path in sys.argv[1:]:
        try:
            check_perms(l_str_path)
        except Exception, e:
            error(l_str_path, "exception: %s" % (str(e)))
else:
    try:
        check_perms("")
    except Exception, e:
        error(".", "exception: %s" % (str(e)))

if len(l_arstr_errors) > 0:
    error_summary()
else:
    extensions_summary()
