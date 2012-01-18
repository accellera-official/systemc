include ../../build-unix/Makefile.config

OBJ_NAME = update_original

INCDIR = -I../../common/include/models -I../include
SRCDIR = ../src
 
CFLAGS = $(FLAGS) $(INCDIR)

.PHONY: clean all run check announce

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

$(OBJ_NAME).exe: $(OBJ_NAME).o
	$(LD) -o $@ $(OBJ_NAME).o $(LDFLAGS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) -c $(CFLAGS) $< -o $@

# DO NOT DELETE

clean:
	rm -f *.o *.log $(OBJ_NAME).exe
