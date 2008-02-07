include ../../build-unix/Makefile.config

OBJ_NAME = gp_mandatory_extension

INCDIR =    -I../include \
            -I../../common/include \
            -I../../common/include/adapters \
            -I../../common/include/models \
            -I../../common/include/SimpleSocket \
            -I../../common/include/utils \
            -I../../common_tlm_1/include

SRCDIR = ../src
SRCDIRCOMMON = ../../common/src
SRCDIRCOMMONADAPTER = ../../common/src/adapters
SRCDIRCOMMONTLM1 = ../../common_tlm_1/src
 
CFLAGS = $(FLAGS) $(INCDIR)

OBJS   =    $(OBJ_NAME).o \
            gp_mandatory_extension_top.o \
            at_initiator.o \
            at_processor_abstraction.o \
            traffic_generator.o \
            at_target_memory.o \
            lt_target.o \
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

gp_mandatory_extension_top.o: $(SRCDIR)/gp_mandatory_extension_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

at_initiator.o: $(SRCDIR)/at_initiator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

at_processor_abstraction.o: $(SRCDIR)/at_processor_abstraction.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

traffic_generator.o: $(SRCDIR)/traffic_generator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

at_target_memory.o: $(SRCDIR)/at_target_memory.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_target.o: $(SRCDIR)/lt_target.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

mtrand.o: $(SRCDIRCOMMON)/mtrand.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE

