include ../../build-unix/Makefile.config

OBJ_NAME = ut_lt

INCDIR =    -I../include \
            -I../../common/include \
            -I../../common/include/adapters \
            -I../../common/include/models \
            -I../../common/include/SimpleSocket \
            -I../../common/include/utils

SRCDIR = ../src
SRCDIRCOMMON = ../../common/src
SRCDIRCOMMONADAPTER = ../../common/src/adapters
 
CFLAGS = $(FLAGS) $(INCDIR)

OBJS   =    $(OBJ_NAME).o \
            ut_lt_top.o \
            lt_initiator.o \
            lt_processor_abstraction.o \
            traffic_generator.o \
            lt_target_memory.o \
            mtrand.o \
            adapter_lt_ut.o \
            adapter_ut_lt.o \
            ut_initiator.o \
            ut_target.o

.PHONY: clean all check

all: announce $(OBJ_NAME).exe

announce:
	@if  ( test ! -d $(FLAG_BATCH) ); \
	 	then echo; echo "$(OBJ_NAME):"; echo; \
	 fi
	
check: announce $(OBJ_NAME).exe
	@$(VALGRIND) ./$(OBJ_NAME).exe > run.log
	@diff run.log ../results/expected.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

run: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe

$(OBJ_NAME).exe: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) $(CFLAGS) -c $< -o $@

ut_lt_top.o: $(SRCDIR)/ut_lt_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_initiator.o: $(SRCDIR)/lt_initiator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_processor_abstraction.o: $(SRCDIR)/lt_processor_abstraction.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

traffic_generator.o: $(SRCDIR)/traffic_generator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_target_memory.o: $(SRCDIR)/lt_target_memory.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

mtrand.o: $(SRCDIRCOMMON)/mtrand.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

adapter_lt_ut.o: $(SRCDIRCOMMONADAPTER)/adapter_lt_ut.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

adapter_ut_lt.o: $(SRCDIRCOMMONADAPTER)/adapter_ut_lt.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

ut_initiator.o: $(SRCDIR)/ut_initiator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

ut_target.o: $(SRCDIR)/ut_target.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE

