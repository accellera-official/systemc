include ../../build-unix/Makefile.config

TARGET_ARCH = linux

GXX = g++
GCC = gcc
FLAGS = -g -Wall

OBJ_NAME = transactor

INCDIR = -I$(SYSTEMC_HOME)/include -I$(TLM_HOME)/include -I../../common/include/SimpleSocket -I../../common/include/models -I../../common/include/utils -I../include/transactors -I../include/tlm_bus
SRCDIR = ../src
LIBDIR = -L$(SYSTEMC_HOME)/lib-$(TARGET_ARCH)

LIB_SC = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

.PHONE: clean all

all: $(OBJ_NAME)

.c.o:
	$(GCC) $(CFLAGS) -c $<
.cpp.o:
	$(GXX) $(CFLAGS) -c $< -o $@
.cc.o:
	$(GXX) $(CFLAGS) -c $< -o $@

$(OBJ_NAME).exe: $(OBJ_NAME).o
	$(GXX) -o $@ $(OBJ_NAME).o $(LIBS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) -c $(CFLAGS) $(SRCDIR)/$(OBJ_NAME).cpp

clean:
	rm -f $(OBJ_NAME).exe *.o

# DO NOT DELETE

