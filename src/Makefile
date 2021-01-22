
SRCDIR = $(shell pwd)
OUTPUT = ../LinuxGcc

CXXFILES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(CXXFILES))
#CXXFILES = $(patsubst %.cpp, $(SRCDIR)/%.cpp, $(CXXLOCALFILES))
CXXFLAGS = -D_LINUX
INCLUDES = -I/usr/include -I../include
LIBS =
PRODUCT = libclicpp.a

default: $(PRODUCT)
	cp $(PRODUCT) $(OUTPUT)

vars:
	@echo "CXX = $(CXX)"
	@echo "AR = $(AR)"
	@echo "OUTPUT = $(OUTPUT)"
	@echo "SRCDIR = $(SRCDIR)"
	@echo "CXXFILES = $(CXXFILES)"
	@echo "OBJS = $(OBJS)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "INCLUDES = $(INCLUDES)"
	@echo "LIBS = $(LIBS)"
	@echo "PRODUCT = $(PRODUCT)"

help:
	@echo "make help        Get help"
	@echo "make vars        Show Makefile variables"
	@echo "make depend      Update Makefile.depend"
	@echo "make             Generate libclicpp.a"
	@echo "make clean       Clean up output objects and library"

$(PRODUCT): $(OBJS)
	$(AR) -cru $(PRODUCT) $(OBJS) $(LIBS)

%.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

depend:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MM $(CXXFILES) > Makefile.depend

clean:
	rm -f $(OBJS) $(PRODUCT)

include Makefile.depend

