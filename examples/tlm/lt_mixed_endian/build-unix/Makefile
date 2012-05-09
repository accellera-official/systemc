include ../../build-unix/Makefile.config

OBJ_NAME = lt
EXAMPLE_NAME = lt_mixed_endian

SRCDIR           = ../src
SRCDIRCOMMON     = ../../common/src

INCDIR =    -I../include \
            -I../../common/include

CFLAGS = $(FLAGS) $(INCDIR)

OBJS = $(OBJ_NAME).o \
        $(OBJ_NAME)_top.o \
        lt_initiator.o \
        at_target_1_phase.o \
        at_target_4_phase.o \
        initiator_top.o \
        memory.o \
        report.o \
        me_traffic_generator.o

.PHONY: clean all check

all: announce $(OBJ_NAME).exe

announce:
	@if  ( test ! -d $(FLAG_BATCH) ); \
		then echo; echo "$(EXAMPLE_NAME):"; echo; \
	fi

check: announce $(OBJ_NAME).exe
	@$(VALGRIND) ./$(OBJ_NAME).exe < ../results/input.txt | grep "::" > run.log
	@diff run.log ../results/expected.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

run: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe < ../results/input.txt

$(OBJ_NAME).exe: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LDFLAGS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) $(CFLAGS) -c $< -o $@

$(OBJ_NAME)_top.o: $(SRCDIR)/$(OBJ_NAME)_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

initiator_top.o: $(SRCDIR)/initiator_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

at_target_1_phase.o: $(SRCDIRCOMMON)/at_target_1_phase.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

at_target_4_phase.o: $(SRCDIRCOMMON)/at_target_4_phase.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_initiator.o: $(SRCDIRCOMMON)/lt_initiator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

memory.o: $(SRCDIRCOMMON)/memory.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

report.o: $(SRCDIRCOMMON)/report.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

me_traffic_generator.o: $(SRCDIR)/me_traffic_generator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE
