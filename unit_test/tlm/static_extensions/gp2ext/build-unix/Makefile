include ../../../build-unix/Makefile.config

OBJ_NAME = gp2ext

INCDIR = -I$(SYSTEMC_INC_DIR) -I$(TLM_INC_DIR) -I../../../common/include/SimpleSocket -I../../../common/include/models -I../../../common/include/utils -I../../include
SRCDIR = ../src
LIBDIR = -L$(SYSTEMC_LIB_DIR)

LIB_SC = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

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

.c.o:
	$(GCC) $(CFLAGS) -c $<
.cpp.o:
	$(GXX) $(CFLAGS) -c $< -o $@
.cc.o:
	$(GXX) $(CFLAGS) -c $< -o $@

$(OBJ_NAME).exe: $(OBJ_NAME).o
	$(LD) -o $@ $(OBJ_NAME).o $(LIBS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE

