
TECSGEN := tecsgen

TCWDIR := ../../..
SRCDIR := ..
COMMONDIR := $(SRCDIR)/../common
GENDIR := ./gen

CC = clang
CXX = clang++
LD = clang++

INCLUDES := -I$(TCWDIR)/include -I$(GENDIR) -I$(COMMONDIR)
INCLUDES_TECS := -I$(TCWDIR)/src -I$(SRCDIR) -I$(COMMONDIR)

CFLAGS = $(INCLUDES) -g "-DInline=static inline"
CXXFLAGS = $(CFLAGS) -std=c++1z

# FIXME: this needs to be customized for other platforms
LDFLAGS = -framework SDL2 -framework Cocoa

TARGET := test

# make.inc defines the list of non-TECS object files as $(TCWTECS_OBJS)
include $(TCWDIR)/src/Makefile.tcw

# source file directories
vpath %.c $(TCWDIR)/src
vpath %.c $(GENDIR)
vpath %.c $(SRCDIR)
vpath %.c $(COMMONDIR)
vpath %.cpp $(TCWDIR)/src

# default targets
default:		all
all:			$(TARGET)

clean:
	rm -rf gen $(OBJS)

# TECS interface code generation
tecs:
	$(TECSGEN) $(INCLUDES) $(INCLUDES_TECS) $(SRCDIR)/test.cdl

-include gen/Makefile.tecsgen

OBJS = $(TCWTECS_OBJS) $(TECS_COBJS)

$(TARGET):		$(OBJS)
	$(LD) $(OBJS) -o $(TARGET) $(LDFLAGS)

