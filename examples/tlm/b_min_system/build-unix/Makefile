include ../../build-unix/Makefile.config

OBJ_NAME = b_min_system

INCDIR = -I$(SYSTEMC_INC_DIR) -I$(TLM_INC_DIR) -I../include -I../example_protocol/include -I../example_bus/include
SRCDIR = ../src

LIBDIR = -L$(SYSTEMC_LIB_DIR)

LIB_SC = -lsystemc

LIBS = $(LIBDIR) $(LIB_SC)
 
CFLAGS = $(FLAGS) $(INCDIR)

SRCSORIG = example_protocol/src/rw_initiator_socket.cpp    \
           example_protocol/src/rw_target_socket.cpp       \
           example_protocol/src/rw_master_base.cpp         \
           example_protocol/src/rw_slave_base.cpp          \
           src/traffic_generator.cpp                       \
           src/simple_memory.cpp                           \
           src/top.cpp                                     \
           src/$(OBJ_NAME).cpp 

SRCS = $(SRCSORIG:%=../%)
OBJS = $(SRCS:%.cpp=%.o)
DEPS = $(SRCS:%.cpp=%.d)

.PHONY: clean all check

all: announce $(OBJ_NAME).exe

announce:
	@if  ( test ! -d $(FLAG_BATCH) ); \
	 	then echo; echo "$(OBJ_NAME):"; echo; \
	 fi
	
check: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe > run.log
	@diff run.log ../results/expected.log > diff.log 2>&1;\
	if [ -s diff.log ]; then echo "***ERROR:"; cat diff.log; else echo "OK"; fi

run: announce $(OBJ_NAME).exe
	@./$(OBJ_NAME).exe

$(OBJ_NAME).exe: $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)

%.d: %.cpp
	@ echo Making dependencies for $<
	@ $(GXX) -E -c -MM $(CFLAGS) $< -o $@ >/dev/null
	@ cat $@ | sed 's#.*:# $@ :#1' > $@.tmp
	@ mv -f $@.tmp $@
        
%.o: %.cpp %.d Makefile
	$(GXX) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(DEPS) $(OBJS) *.log $(OBJ_NAME).exe

# include dependency file
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
endif


