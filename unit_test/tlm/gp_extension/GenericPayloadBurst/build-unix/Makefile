include ../../../build-unix/Makefile.config

OBJ_NAME = generic_payload_burst

INCDIR  = -I$(SYSTEMC_INC_DIR) -I$(TLM_INC_DIR) -I../include -I../../common/include
SRCDIR  = ../src
LIBDIR  = -L$(SYSTEMC_LIB_DIR)

LIB_SC  = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

OBJS   = $(OBJ_NAME).o tlm_target.o


.PHONY: clean all check run announce

all: announce $(OBJ_NAME).exe

announce:
	@if  ( test ! -d $(FLAG_BATCH) ); \
	 	then echo; echo "$(OBJ_NAME):"; echo; \
	 fi
	
check: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe > run.log
	@diff run.log ../results/expected.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

run: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe

$(OBJ_NAME).exe: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) $(CFLAGS) -c $< -o $@

tlm_initiator.o: $(SRCDIR)/tlm_initiator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

tlm_target.o: $(SRCDIR)/tlm_target.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe
