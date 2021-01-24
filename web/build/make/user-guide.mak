# Copyright (c) 2006-2010, Alexis Royer, http://alexis.royer.free.fr/CLI
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


# Default goal
.DEFAULT_GOAL = html
.PHONY: user-guide.default
user-guide.default: $(.DEFAULT_GOAL) ;

# Directories
CLI_DIR ?= ../../..
include $(CLI_DIR)/build/make/_vars.mak
DB_DIR = $(CLI_DIR)/web/user-guide
MISC_DIR = $(DB_DIR)/misc
SAMPLES_DIR = $(DB_DIR)/samples

# Source
DB_USER_GUIDE = $(DB_DIR)/cli-user-guide.xml
HTML_USER_GUIDE = $(patsubst %.xml, %.html, $(DB_USER_GUIDE))
CLI_SAMPLES = $(wildcard $(CLI_DIR)/samples/user-guide/*.xml)
DB_SAMPLES = $(patsubst %.xml,$(SAMPLES_DIR)/%.db,$(notdir $(CLI_SAMPLES))) $(SAMPLES_DIR)/circle-cpp.db $(SAMPLES_DIR)/circle-java.db
MISC_FILES = $(MISC_DIR)/clisample.xml $(MISC_DIR)/clisample.html $(MISC_DIR)/empty.cpp $(MISC_DIR)/Empty.java

# Transformation
XSL_STYLESHEET = $(DOCBOOK_XSL_HTML)

# Configuration
CSS_FLAGS = --param html.stylesheet "'stylesheet.css'"
TOC_FLAGS =
#TOC_FLAGS = --param generate.toc "'article toc,title,figure'"
AUTOLABEL_FLAGS = --param section.autolabel 1

XSLT_OPTIONS = $(CSS_FLAGS) $(TOC_FLAGS) $(AUTOLABEL_FLAGS)

# Rules
.PHONY: html
html: dirs $(HTML_USER_GUIDE) ;

$(HTML_USER_GUIDE): $(wildcard $(DB_DIR)/*.xml $(DB_DIR)/*.css) $(DB_SAMPLES) $(MISC_FILES)
ifneq ($(XSL_STYLESHEET),)
	xsltproc $(XSLT_OPTIONS) "$(XSL_STYLESHEET)" "$(DB_USER_GUIDE)" > $@.tmp && mv $@.tmp $@
else
$(warning Please set DOCBOOK_XSL_HTML to a valid UNIX path to have the docbook user-guide being generated)
endif

.PHONY: dirs
dirs: ;

$(SAMPLES_DIR)/circle-cpp.db: $(CLI_DIR)/samples/user-guide/circle.h
	cat $< | sed -e "s/&/&amp;/g" > $@

$(SAMPLES_DIR)/circle-java.db: $(CLI_DIR)/samples/user-guide/Circle.java
	cat $< | sed -e "s/&/&amp;/g" > $@

$(SAMPLES_DIR)/%.db: $(CLI_DIR)/samples/user-guide/%.xml $(SAMPLES_DIR)/cli2db.xsl
	xsltproc $(SAMPLES_DIR)/cli2db.xsl $< > $@

$(MISC_DIR)/empty.cpp: $(CLI_DIR)/samples/user-guide/empty.xml $(CLI_DIR)/cpp/xsl/cppclic.xsl
	xsltproc $(CLI_DIR)/cpp/xsl/cppclic.xsl $< > $@

$(MISC_DIR)/Empty.java: $(CLI_DIR)/samples/user-guide/empty.xml $(CLI_DIR)/java/xsl/javaclic.xsl
	xsltproc --stringparam STR_CliClassName "Empty" $(CLI_DIR)/java/xsl/javaclic.xsl $< > $@

$(MISC_DIR)/clisample.xml: $(CLI_DIR)/samples/clisample/clisample.xml
	cp $< $@

$(MISC_DIR)/clisample.html: $(MISC_DIR)/clisample.xml $(CLI_DIR)/xsl/cli2help.xsl
	xsltproc $(CLI_DIR)/xsl/cli2help.xsl $< > $@

deps: ;

.PHONY: clean
clean:
ifneq ($(XSL_STYLESHEET),)
	$(RM) $(HTML_USER_GUIDE) $(DB_SAMPLES) $(MISC_FILES)
else
	@echo "Please set DOCBOOK_XSL_HTML to have the docbook user-guide being generated"
endif

# Debug and help
include $(CLI_DIR)/build/make/_help.mak

.PHONY: $(CLI_DIR)/web/user-guide/user-guide.help
help: $(CLI_DIR)/web/user-guide/user-guide.help
$(CLI_DIR)/web/user-guide/user-guide.help:
	$(call PrintHelp, html,      Generate user-guide)
	$(call PrintHelp, clean,     Clean up output and intermediate files)

.PHONY: $(CLI_DIR)/web/user-guide/user-guide.vars
vars: $(CLI_DIR)/web/user-guide/user-guide.vars
$(CLI_DIR)/web/user-guide/user-guide.vars:
	$(call ShowVariables, \
		DB_DIR MISC_DIR SAMPLES_DIR \
		DB_USER_GUIDE HTML_USER_GUIDE CLI_SAMPLES DB_SAMPLES MISC_FILES \
		XSL_STYLESHEET \
		CSS_FLAGS TOC_FLAGS AUTOLABEL_FLAGS XSLT_OPTIONS)
