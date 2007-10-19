include ../../../build-unix/Makefile.config

OBJ_NAME = generic_payload

INCDIR = -I$(SYSTEMC_INC_DIR) -I$(TLM_INC_DIR) -I../include -I../../common/include
SRCDIR = ../src
LIBDIR = -L$(SYSTEMC_LIB_DIR)

LIB_SC = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

OBJS   = $(OBJ_NAME).o tlm_slave.o

.PHONY: clean all run

all: $(OBJ_NAME).exe

run:  $(OBJ_NAME).exe
	./$(OBJ_NAME).exe > run.log
	@diff run.log ../golden.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

$(OBJ_NAME).exe: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) $(CFLAGS) -c $< -o $@

tlm_slave.o: $(SRCDIR)/tlm_slave.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_NAME).exe  *.o run.log diff.log
