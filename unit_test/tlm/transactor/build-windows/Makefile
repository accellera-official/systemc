include ../../build-windows/Makefile.config

OBJ_NAME = transactor

MY_INCLUDES = /I../../common/include/SimpleSocket /I../../common/include/models -I../../common/include/utils /I../include/transactors /I../include/tlm_bus
SRCDIR = ../src
 
CFLAGS = $(FLAGS) $(MY_INCLUDES)

.PHONY: clean all run

all: $(OBJ_NAME).exe

run:  $(OBJ_NAME).exe
	./$(OBJ_NAME).exe > run.log
	@diff run.log ../golden.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

.c.o:
	$(CC) $(CFLAGS) /c $< /Fo"$@"
.cpp.o:
	$(CC) $(CFLAGS) /c $< /Fo"$@"
.cc.o:
	$(CC) $(CFLAGS) /c $< /Fo"$@"

$(OBJ_NAME).exe: $(OBJ_NAME).o
	$(LD) /OUT:"$@" $(OBJ_NAME).o $(LDFLAGS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"


clean:
	rm -f $(OBJ_NAME).exe *.o run.log diff.log *.idb *.pdb *ilk

# DO NOT DELETE

