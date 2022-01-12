
# Makefile
# Author : Byunghun Hwang <bh.hwang@iae.re.kr>
# Usage : make ARCH=x86_64

# Makefile

OS := $(shell uname)

#Set Architecutre
ARCH := armhf

OSFLAG	:=
ifeq ($(OS),Windows_NT)
	OSFLAG += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		OSFLAG += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		OSFLAG += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG += -D OSX
		INCLUDE_DIR = -I/opt/homebrew/Cellar/opencv/4.5.4_2/include/opencv4
	endif
		UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		OSFLAG += -D AMD64
	endif
		ifneq ($(filter %86,$(UNAME_P)),)
	OSFLAG += -D IA32
		endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		OSFLAG += -D ARM
	endif
endif



#Compilers

#CC := /usr/bin/arm-linux-gnueabihf-g++-8
#GCC := /usr/bin/arm-linux-gnueabihf-gcc-8
CC := g++
GCC := gcc
OUTDIR		= ./bin/
BUILDDIR		= ./bin/
INCLUDE_DIR += -I./ -I./include/
LD_LIBRARY_PATH += -L/usr/local/lib/ -L./lib/


# OS
ifeq ($(OS),Linux) #for Linux
	LDFLAGS = -Wl,--export-dynamic -Wl,-rpath=$(LD_LIBRARY_PATH)
	LDLIBS = -lpthread  `pkg-config --cflags --libs opencv4` -lusb-1.0
	GTEST_LDLIBS = -lgtest
else

# -lArduCamLib -lusb-1.0  -larducam_config_parser

endif

ifeq ($(OS), Darwin)
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
						$(CC) $(CXXFLAGS) $(INCLUDE_DIR) -c $^ -o $@ $(LDLIBS)

all : ppe libppe2d

deploy : FORCE
	cp $(BINDIR)
clean : FORCE 
		$(RM) $(BUILDDIR)*.o
FORCE : 