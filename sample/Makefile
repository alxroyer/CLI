
SRCDIR = $(shell pwd)
OUTPUT = ../LinuxGcc

CXXFILES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(CXXFILES))
#CXXFILES = $(patsubst %.cpp, $(SRCDIR)/%.cpp, $(CXXLOCALFILES))
CXXFLAGS = -D_LINUX
INCLUDES = -I/usr/include -I../include
PROJ_LIBS = ../LinuxGcc/libclicpp.a
LIB_OPTS = -ldl /usr/lib/libncurses.a
PRODUCT = clisample

default: $(PRODUCT)
	cp $(PRODUCT) $(OUTPUT)

vars: climaker.cpp
	@echo "CXX = $(CXX)"
	@echo "AR = $(AR)"
	@echo "OUTPUT = $(OUTPUT)"
	@echo "SRCDIR = $(SRCDIR)"
	@echo "CXXFILES = $(CXXFILES)"
	@echo "OBJS = $(OBJS)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "INCLUDES = $(INCLUDES)"
	@echo "PROJ_LIBS = $(PROJ_LIBS)"
	@echo "LIB_OPTS = $(LIB_OPTS)"
	@echo "PRODUCT = $(PRODUCT)"

help:
	@echo "make help        Get help"
	@echo "make vars        Show Makefile variables"
	@echo "make depend      Update Makefile.depend"
	@echo "make             Generate libclicpp.a"
	@echo "make clean       Clean up output objects and library"

$(PRODUCT): clisample.cpp $(OBJS) $(PROJ_LIBS)
	$(CXX) -o $(PRODUCT) $(OBJS) $(PROJ_LIBS) $(LIB_OPTS)
%.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@
../LinuxGcc/libclicpp.a:
	cd ../src && make

clisample.cpp: clisample.xml ../xsl/cppclic.xsl
	xsltproc ../xsl/cppclic.xsl clisample.xml > clisample.cpp

depend: clisample.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MM $(CXXFILES) > Makefile.depend

clean:
	rm -f $(OBJS) $(PRODUCT)
	echo "" > clisample.cpp
	sleep 1 && touch clisample.xml

include Makefile.depend

