
# Makefile
# Author : Byunghun Hwang <bh.hwang@iae.re.kr>
# Usage : make ARCH=x86_64

# Makefile

OS := $(shell uname)

#Set Architecutre
ARCH := armhf

#Compilers

#CC := /usr/bin/arm-linux-gnueabihf-g++-8
#GCC := /usr/bin/arm-linux-gnueabihf-gcc-8
CC := g++-8
GCC := gcc-8
LD_LIBRARY_PATH += -L./lib/armhf
OUTDIR		= ./bin
BUILDDIR		= ./bin
INCLUDE_DIR = -I./ -I./include/
LD_LIBRARY_PATH += -L/usr/local/lib -L./lib/armhf


# OS
ifeq ($(OS),Linux) #for Linux
	LDFLAGS = -Wl,--export-dynamic -Wl,-rpath=$(LD_LIBRARY_PATH)
	LDLIBS = -pthread
	GTEST_LDLIBS = -lgtest
endif

$(shell mkdir -p $(BUILDDIR))

#if release(-O3), debug(-O0)
CXXFLAGS = -O3 -fPIC -Wall -std=c++17 -D__cplusplus=201703L

#custom definitions
CXXFLAGS += -D__MAJOR__=0 -D__MINOR__=0 -D__REV__=1
RM	= rm -rf

#directories

INCLUDE_FILES = ./include/
SOURCE_FILES = ./
INSTALL_DIR = /usr/local/bin/

# Make
ppe:	$(BUILDDIR)ppe.o
		$(CC) $(LDFLAGS) $(LD_LIBRARY_PATH) -o $(BUILDDIR)$@ $^ $(LDLIBS)
$(BUILDDIR)ppe.o:	$(SOURCE_FILES)ppe/ppe.cc
						$(CC) $(CXXFLAGS) $(INCLUDE_DIR) -c $^ -o $@

all : ppe libppe2d

deploy : FORCE
	cp $(BINDIR)
clean : FORCE 
		$(RM) $(BUILDDIR)*.o
FORCE : 