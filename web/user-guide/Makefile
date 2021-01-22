# This file is part of the CLI library.  The CLI library aims to provide
# facilities in making text-based user interfaces.
# Copyright (C) 2006-2007 Alexis Royer.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


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
