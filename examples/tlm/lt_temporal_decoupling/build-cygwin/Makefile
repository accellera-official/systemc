include ../../build-cygwin/Makefile.config

OBJ_NAME = lt_min_system

MY_INCLUDES = /I../include /I../../common/include /I../../common/include/SimpleSocket /I../../common/include/models /I../../common/include/utils
SRCDIR = ../src
SRCCOMMONDIR = ../../common/src

CFLAGS = $(FLAGS) $(MY_INCLUDES)

OBJS   = $(OBJ_NAME).o lt_initiator.o lt_processor_abstraction.o traffic_generator.o lt_target_memory.o lt_top.o

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

lt_top.o: $(SRCDIR)/lt_top.cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"

lt_initiator.o: $(SRCCOMMONDIR)/lt_initiator.cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"

lt_processor_abstraction.o: $(SRCCOMMONDIR)/lt_processor_abstraction.cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"

traffic_generator.o: $(SRCCOMMONDIR)/traffic_generator.cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"

lt_target_memory.o: $(SRCCOMMONDIR)/lt_target_memory.cpp
	$(CC) /c $(CFLAGS) $< /Fo"$@"


clean:
	rm -f $(OBJ_NAME).exe *.o run.log diff.log *.idb *.pdb *ilk

# DO NOT DELETE

