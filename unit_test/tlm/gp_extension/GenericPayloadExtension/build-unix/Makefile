include ../../../build-unix/Makefile.config

GXX = g++
GCC = gcc
FLAGS = -g -Wall

OBJ_NAME = generic_payload_extension

INCDIR  = -I$(SYSTEMC_INC_DIR) -I$(TLM_INC_DIR) -I../include -I../../common/include
SRCDIR  = ../src
LIBDIR  = -L$(SYSTEMC_LIB_DIR)

LIB_SC  = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

OBJS   = $(OBJ_NAME).o tlm_master.o tlm_slave.o

#-------------------------------------------------------------------------------
# Rules
#-------------------------------------------------------------------------------

$(OBJ_NAME).exe: $(OBJS)
	$(GXX) -o $@ $(OBJS) $(LIBS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) $(CFLAGS) -c $< -o $@

tlm_master.o: $(SRCDIR)/tlm_master.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

tlm_slave.o: $(SRCDIR)/tlm_slave.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_NAME).exe *.o 
