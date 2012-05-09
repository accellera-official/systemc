include ../../build-unix/Makefile.config

OBJ_NAME         = lt_extension_mandatory

SRCDIR           = ../src
SRCDIRCOMMON     = ../../common/src

INCDIR           = -I../include \
                   -I../../common/include

CFLAGS           = $(FLAGS) $(INCDIR)

OBJS             = $(OBJ_NAME).o \
                   $(OBJ_NAME)_top.o \
                   lt_initiator_extension_mandatory.o \
                   lt_target_extension_mandatory.o \
                   extension_initiator_id.o \
                   report.o

.PHONY: clean all check run announce

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

$(OBJ_NAME)_top.o: $(SRCDIR)/$(OBJ_NAME)_top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) -c $(CFLAGS) $< -o $@

lt_initiator_extension_mandatory.o: $(SRCDIR)/lt_initiator_extension_mandatory.cpp
	$(GXX) -c $(CFLAGS) $< -o $@

lt_target_extension_mandatory.o: $(SRCDIR)/lt_target_extension_mandatory.cpp
	$(GXX) -c $(CFLAGS) $< -o $@

extension_initiator_id.o: $(SRCDIRCOMMON)/extension_initiator_id.cpp
	$(GXX) -c $(CFLAGS) $< -o $@

report.o: $(SRCDIRCOMMON)/report.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE
