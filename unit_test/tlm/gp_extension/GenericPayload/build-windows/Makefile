include ../../../build-windows/Makefile.config

OBJ_NAME = generic_payload

MY_INCLUDES = /I../include /I../../common/include
SRCDIR = ../src
 
CFLAGS = $(FLAGS) $(MY_INCLUDES)

OBJS = $(OBJ_NAME).o tlm_slave.o

.PHONY: clean all run

all: $(OBJ_NAME).exe

run:  $(OBJ_NAME).exe
	./$(OBJ_NAME).exe > run.log
	@diff run.log ../golden.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi


$(OBJ_NAME).exe: $(OBJS)
	$(LD) /OUT:"$@" $(OBJS) $(LDFLAGS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"

tlm_slave.o: $(SRCDIR)/tlm_slave.cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"


clean:
	rm -f $(OBJ_NAME).exe *.o run.log diff.log *.idb *.pdb *ilk

# DO NOT DELETE

