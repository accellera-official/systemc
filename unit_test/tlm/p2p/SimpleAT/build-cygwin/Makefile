include ../../../build-cygwin/Makefile.config

OBJ_NAME = simple_AT

MY_INCLUDES = /I../../../common/include/SimpleSocket /I../../../common/include/models /I../../../common/include/utils
SRCDIR = ../src
 
CFLAGS = $(FLAGS) $(MY_INCLUDES)

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

