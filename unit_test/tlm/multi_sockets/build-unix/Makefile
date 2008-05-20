include ../../build-unix/Makefile.config

OBJ_NAME = multi_sockets

INCDIR = -I../../common/include/models -I../include -I$(BOOST_HOME)
SRCDIR = ../src
 
CFLAGS = $(FLAGS) $(INCDIR)

ifdef BOOST_HOME

.PHONY: clean all run check announce

all: announce $(OBJ_NAME).exe

announce:
	@if  ( test ! -d $(FLAG_BATCH) ); \
	 	then echo; echo "$(OBJ_NAME):"; echo; \
	 fi
	
check: announce $(OBJ_NAME).exe
	@$(VALGRIND) ./$(OBJ_NAME).exe > runtemp.log
	@cat runtemp.log | grep -v "stopped by user" > run.log
	@cat ../results/expected.log | grep -v "stopped by user" > expected.log
	@diff -B run.log expected.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

run: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe

$(OBJ_NAME).exe: $(OBJ_NAME).o
	$(LD) -o $@ $(OBJ_NAME).o $(LDFLAGS)

$(OBJ_NAME).o: $(SRCDIR)/$(OBJ_NAME).cpp
	$(GXX) -c $(CFLAGS) $< -o $@

# DO NOT DELETE

else
all:
	@echo Variable BOOST_HOME not specified. Set it so that it points to your boost installation \(one level above /boost\).
endif

clean:
	rm -f *.o *.log $(OBJ_NAME).exe
