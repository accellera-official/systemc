include ../../build-unix/Makefile.config

OBJ_NAME = b_min_system

INCDIR =    -I$(SYSTEMC_INC_DIR) \
            -I$(TLM_INC_DIR) \
            -I../include \
            -I../example_protocol/include \
            -I../example_bus/include
            
SRCDIR          = ../src
SRCDIRBUS       = ../example_bus/src
SRCDIRPROTOCOL  = ../example_protocol/src

CFLAGS = $(FLAGS) $(INCDIR)

OBJS = $(OBJ_NAME).o \
        top.o \
        simple_memory.o \
        traffic_generator.o \
        rw_slave_base.o \
        rw_master_base.o \
        rw_target_socket.o \
        rw_initiator_socket.o
        
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

top.o: $(SRCDIR)/top.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

simple_memory.o: $(SRCDIR)/simple_memory.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

traffic_generator.o: $(SRCDIR)/traffic_generator.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

rw_slave_base.o: $(SRCDIRPROTOCOL)/rw_slave_base.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

rw_master_base.o: $(SRCDIRPROTOCOL)/rw_master_base.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

rw_target_socket.o: $(SRCDIRPROTOCOL)/rw_target_socket.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

rw_initiator_socket.o: $(SRCDIRPROTOCOL)/rw_initiator_socket.cpp
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.log $(OBJ_NAME).exe

# DO NOT DELETE

