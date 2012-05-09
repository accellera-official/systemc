include ../../build-unix/Makefile.config

OBJ_NAME = lt

SRCDIR           = ../src
SRCDIRCOMMON     = ../../common/src

INCDIR =    -I../include \
            -I../../common/include

CFLAGS = $(FLAGS) $(INCDIR)

OBJS = $(OBJ_NAME).o \
        $(OBJ_NAME)_top.o \
        lt_initiator.o \
        at_target_1_phase.o \
        lt_target.o \
        initiator_top.o \
        memory.o \
        report.o \
        traffic_generator.o

.PHONY: clean all check

all: announce $(OBJ_NAME).exe

announce:
	@if  ( test ! -d $(FLAG_BATCH) ); \
		then echo; echo "$(OBJ_NAME):"; echo; \
	fi

check: announce $(OBJ_NAME).exe
	@$(VALGRIND) ./$(OBJ_NAME).exe > run.log
	@cat run.log | grep -v "stopped by user" | awk '{if($$0!="") print $$0}' > run_trimmed.log
	@cat ../results/expected.log | grep -v "stopped by user" | awk '{if($$0!="") print $$0}' > ./expected_trimmed.log
	@diff ./run_trimmed.log ./expected_trimmed.log > diff.log 2>&1;\
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

at_target_1_phase.o: $(SRCDIRCOMMON)/at_target_1_phase.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_target.o: $(SRCDIRCOMMON)/lt_target.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

lt_initiator.o: $(SRCDIRCOMMON)/lt_initiator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

memory.o: $(SRCDIRCOMMON)/memory.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

report.o: $(SRCDIRCOMMON)/report.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

traffic_generator.o: $(SRCDIRCOMMON)/traffic_generator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE
