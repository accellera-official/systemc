include ../../../build-unix/Makefile.config

OBJ_NAME = register_socket_process_LT

INCDIR = -I$(SYSTEMC_INC_DIR) -I$(TLM_INC_DIR) -I../../../common/include/SimpleSocket -I../../../common/include/models
SRCDIR = ../src
LIBDIR = -L$(SYSTEMC_LIB_DIR)

LIB_SC = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

.PHONY: clean all run

all: $(OBJ_NAME).exe

run:  $(OBJ_NAME).exe
	./$(OBJ_NAME).exe > run.log
	@diff run.log ../golden.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

.c.o:
	$(GCC) $(CFLAGS) -c $<
.cpp.o:
	$(GXX) $(CFLAGS) -c $< -o $@
.cc.o:
	$(GXX) $(CFLAGS) -c $< -o $@

$(OBJ_NAME).exe: $(OBJ_NAME).o
	$(LD) -o $@ $(OBJ_NAME).o $(LIBS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) -c $(CFLAGS) $(SRCDIR)/$(OBJ_NAME).cpp

clean:
	rm -f $(OBJ_NAME).exe *.o run.log diff.log

# DO NOT DELETE

