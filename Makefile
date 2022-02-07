
# Makefile
# Author : Byunghun Hwang <bh.hwang@iae.re.kr>
# Usage : make ARCH=x86_64

# Makefile

OS := $(shell uname)


#Compilers


CC := g++
GCC := gcc
OUTDIR		= ./bin/
BUILDDIR		= ./bin/
INCLUDE_DIR += -I./ -I./include/
LD_LIBRARY_PATH += -L./lib/x86/Arducam_SDK/


# OS
ifeq ($(OS),Linux) #for Linux
	LDFLAGS = -Wl,--export-dynamic
#LDLIBS =  -lArduCamLib -lpthread -larducam_config_parser `pkg-config --cflags --libs opencv4` -lusb-1.0
	LDLIBS =  -lpthread `pkg-config --cflags --libs opencv4`
	GTEST_LDLIBS = -lgtest
else

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
SOURCE_FILES = ./ppe/
INSTALL_DIR = /usr/local/bin/

# Make
ppe:	$(BUILDDIR)ppe.o \
		$(BUILDDIR)ov2311_uc762c.o \
		$(BUILDDIR)task_wafer.o \
		$(BUILDDIR)task_tool.o
		$(CC) $(LDFLAGS) $(LD_LIBRARY_PATH) -o $(BUILDDIR)$@ $^ $(LDLIBS)
$(BUILDDIR)ppe.o:	$(SOURCE_FILES)ppe.cc
						$(CC) $(CXXFLAGS) $(INCLUDE_DIR) -c $^ -o $@ $(LDLIBS)
#$(BUILDDIR)ov2311_uc593c.o:	$(INCLUDE_FILES)module/ov2311_uc593c.cc
#						$(CC) $(CXXFLAGS) $(INCLUDE_DIR) -c $^ -o $@ $(LDLIBS)
$(BUILDDIR)ov2311_uc762c.o:	$(INCLUDE_FILES)module/ov2311_uc762c.cc
						$(CC) $(CXXFLAGS) $(INCLUDE_DIR) -c $^ -o $@ $(LDLIBS)
$(BUILDDIR)task_wafer.o:	$(SOURCE_FILES)task_wafer.cc
						$(CC) $(CXXFLAGS) $(INCLUDE_DIR) -c $^ -o $@ $(LDLIBS)
$(BUILDDIR)task_tool.o:	$(SOURCE_FILES)task_tool.cc
						$(CC) $(CXXFLAGS) $(INCLUDE_DIR) -c $^ -o $@ $(LDLIBS)


all : ppe libppe2d

deploy : FORCE
	cp $(BINDIR)
clean : FORCE 
		$(RM) $(BUILDDIR)*.o
FORCE : 