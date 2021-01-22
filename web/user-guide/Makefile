# Copyright (c) 2006-2007, Alexis Royer
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


# Source
DB_USER_GUIDE = cli-user-guide.xml
HTML_USER_GUIDE = $(patsubst %.xml, %.html, $(DB_USER_GUIDE))
CLI_SAMPLES = $(wildcard samples/*.xml)
DB_SAMPLES = $(patsubst %.xml, %.db, $(CLI_SAMPLES)) samples/circle.db

# Transformation
# Please set DOCBOOK_XSL_HTML to a valid UNIX path.
XSL_STYLESHEET = $(DOCBOOK_XSL_HTML)

# Configuration
CSS_FLAGS = --param html.stylesheet "'stylesheet.css'"
TOC_FLAGS =
#TOC_FLAGS = --param generate.toc "'article toc,title,figure'"
AUTOLABEL_FLAGS = --param section.autolabel 1

XSLT_OPTIONS = $(CSS_FLAGS) $(TOC_FLAGS) $(AUTOLABEL_FLAGS)

# Rules
$(HTML_USER_GUIDE): $(wildcard *.xml *.css) $(DB_SAMPLES)
	xsltproc $(XSLT_OPTIONS) $(XSL_STYLESHEET) $(DB_USER_GUIDE) > $@

samples/circle.db: samples/circle.h
	cat $< | sed -e "s/&/&amp;/g" > $@

samples/%.db: samples/%.xml
	xsltproc samples/cli2db.xsl $< > $@

.PHONY: clean
clean:
	rm -f $(HTML_USER_GUIDE) $(DB_SAMPLES)
