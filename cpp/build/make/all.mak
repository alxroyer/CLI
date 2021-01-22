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


.PHONY: default
default: $(patsubst %.mak,%.default,$(PROJECTS))
	@#

.PHONY: %.default
%.default:
	@echo "--- $(patsubst %.default,%.mak,$@) ---"
	make -f $(patsubst %.default,%.mak,$@)


.PHONY: vars
vars: $(patsubst %.mak,%.vars,$(PROJECTS))
	@#

.PHONY: %.vars
%.vars:
	@echo "--- $(patsubst %.vars,%.mak,$@) ---"
	make -f $(patsubst %.vars,%.mak,$@) vars


.PHONY: deps
deps: $(patsubst %.mak,%.dep,$(PROJECTS))
	@#

.PHONY: %.dep
%.dep:
	@echo "--- $(patsubst %.dep,%.mak,$@) ---"
	make -f $(patsubst %.dep,%.mak,$@) deps


.PHONY: dirs
dirs: $(patsubst %.mak,%.dirs,$(PROJECTS))
	@#

.PHONY: %.dirs
%.dirs:
	@echo "--- $(patsubst %.dirs,%.mak,$@) ---"
	make -f $(patsubst %.dirs,%.mak,$@) dirs


.PHONY: clean
clean: $(patsubst %.mak,%.clean,$(PROJECTS))
	@#

.PHONY: %.clean
%.clean:
	@echo "--- $(patsubst %.clean,%.mak,$@) ---"
	make -f $(patsubst %.clean,%.mak,$@) clean

