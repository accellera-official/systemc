include ../../../build-unix/Makefile.config

TARGET_ARCH = linux

GXX = g++
GCC = gcc
FLAGS = -g -Wall

OBJ_NAME = generic_packet

INCDIR  = -I../include -I../../common/include -I$(SYSTEMC_HOME)/include
SRCDIR  = ../src
LIBDIR  = -L$(SYSTEMC_HOME)/lib-$(TARGET_ARCH)

LIB_SC  = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

OBJS   = $(OBJ_NAME).o tlm_slave.o

#-------------------------------------------------------------------------------
# Rules
#-------------------------------------------------------------------------------

$(OBJ_NAME).exe: $(OBJS)
	$(GXX) -o $@ $(OBJS) $(LIBS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) $(CFLAGS) -c $< -o $@

tlm_slave.o: $(SRCDIR)/tlm_slave.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_NAME).exe  *.o
