include ../../build-unix/Makefile.config

OBJ_NAME = lt_min_system

INCDIR =    -I../include \
            -I../../common/include \
            -I../../common/include/models \
            -I../../common/include/SimpleSocket \
            -I../../common/include/utils
            
SRCDIR          = ../src
SRCCOMMONDIR    = ../../common/src
 
CFLAGS = $(FLAGS) $(INCDIR)

OBJS   =    $(OBJ_NAME).o \
            lt_initiator.o \
            lt_processor_abstraction.o \
            traffic_generator.o \
            lt_target_memory.o \
            lt_top.o \
            mtrand.o

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

lt_top.o: $(SRCDIR)/lt_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_initiator.o: $(SRCCOMMONDIR)/lt_initiator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_processor_abstraction.o: $(SRCCOMMONDIR)/lt_processor_abstraction.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

traffic_generator.o: $(SRCCOMMONDIR)/traffic_generator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_target_memory.o: $(SRCCOMMONDIR)/lt_target_memory.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

mtrand.o: $(SRCCOMMONDIR)/mtrand.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE

