include ../../build-unix/Makefile.config

OBJ_NAME = at_ooo

INCDIR =    -I../include \
            -I../../common/include \
            -I../../common/include/models \
            -I../../common/include/utils \
            -I../../common/include/SimpleSocket\
            -I../../common/include/adapters\

SRCDIR = ../src
SRCDIRCOMMON = ../../common/src
SRCDIRCOMMONADAPTER = ../../common/src/adapters
SRCDIRCOMMONTLM1 = ../../common_tlm_1/src
 
CFLAGS = $(FLAGS) $(INCDIR)

OBJS   =    $(OBJ_NAME).o \
            $(OBJ_NAME)_top.o \
            initiator_top.o \
            traffic_generator.o \
            at_initiator_4_phase.o \
            at_target_4_phase.o \

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

$(OBJ_NAME)_top.o: $(SRCDIR)/$(OBJ_NAME)_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

initiator_top.o: $(SRCDIR)/initiator_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

traffic_generator.o: $(SRCDIRCOMMON)/traffic_generator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

at_initiator_4_phase.o: $(SRCDIRCOMMON)/at_initiator_4_phase.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

at_target_4_phase.o: $(SRCDIRCOMMON)/at_target_4_phase.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE

