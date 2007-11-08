include ../../../build-windows/Makefile.config

OBJ_NAME = generic_payload_burst

MY_INCLUDES = /I../include /I../../common/include
SRCDIR = ../src
 
CFLAGS = $(FLAGS) $(MY_INCLUDES)

OBJS = $(OBJ_NAME).o tlm_target.o

.PHONY: clean all run check announce

all: announce $(OBJ_NAME).exe

announce:
	@if  ( test ! -d $(FLAG_BATCH) ); \
	 	then echo; echo "$(OBJ_NAME):"; echo; \
	 fi
	
check: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe > run.log
	@diff -w run.log ../results/expected.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

run: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe


$(OBJ_NAME).exe: $(OBJS)
	$(LD) /OUT:"$@" $(OBJS) $(LDFLAGS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"

tlm_target.o: $(SRCDIR)/tlm_target.cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"


clean:
	rm -f $(OBJ_NAME).exe *.o run.log diff.log *.idb *.pdb *ilk

# DO NOT DELETE

