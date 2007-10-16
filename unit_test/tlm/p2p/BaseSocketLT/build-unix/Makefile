include ../../../build-unix/Makefile.config

GXX = g++
GCC = gcc
FLAGS = -g -Wall

OBJ_NAME = base_socket_LT

INCDIR = -I$(SYSTEMC_INC_DIR) -I$(TLM_INC_DIR) -I../../../common/include/models
SRCDIR = ../src
LIBDIR = -L$(SYSTEMC_LIB_DIR)

LIB_SC = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

.PHONE: clean all

all: $(OBJ_NAME).exe

.c.o:
	$(GCC) $(CFLAGS) -c $<
.cpp.o:
	$(GXX) $(CFLAGS) -c $< -o $@
.cc.o:
	$(GXX) $(CFLAGS) -c $< -o $@

$(OBJ_NAME).exe: $(OBJ_NAME).o
	$(GXX) -o $@ $(OBJ_NAME).o $(LIBS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ_NAME).exe *.o

# DO NOT DELETE

