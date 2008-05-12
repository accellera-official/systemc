include ../../build-unix/Makefile.config

OBJ_NAME = tlm_1_integration

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
            tlm_1_integration_top.o \
            lt_initiator.o \
            lt_processor_abstraction.o \
            traffic_generator.o \
            lt_target_memory.o \
            mtrand.o \
            adapter_tlm_1_2.o \
            adapter_tlm_2_1.o \
            tlm_1_master.o \
            tlm_1_slave_memory.o

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

tlm_1_integration_top.o: $(SRCDIR)/tlm_1_integration_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_initiator.o: $(SRCDIRCOMMON)/lt_initiator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_processor_abstraction.o: $(SRCDIRCOMMON)/lt_processor_abstraction.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

traffic_generator.o: $(SRCDIRCOMMON)/traffic_generator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_target_memory.o: $(SRCDIRCOMMON)/lt_target_memory.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

mtrand.o: $(SRCDIRCOMMON)/mtrand.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

adapter_tlm_1_2.o: $(SRCDIRCOMMONADAPTER)/adapter_tlm_1_2.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

adapter_tlm_2_1.o: $(SRCDIRCOMMONADAPTER)/adapter_tlm_2_1.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

tlm_1_master.o: $(SRCDIRCOMMONTLM1)/tlm_1_master.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

tlm_1_slave_memory.o: $(SRCDIRCOMMONTLM1)/tlm_1_slave_memory.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE

