########################################################################
#
# required: set the env variables 
#	    SYSTEMC_HOME -> your SystemC 2.2 installation
#	    TLM_HOME     -> your TLM 2.0 installation
#
########################################################################
# FIXME: In the final release we should not set TLM_HOME here.
TLM_HOME=..

############ Linux ##########
ifeq ($(shell uname -s), Linux)

CC = g++
LD = g++
OPT    = -O2
DEBUG  = -g
OTHER  = -Wall -MMD -I. -I$(TLM_HOME) -I$(SYSTEMC_HOME)/include

ifeq (64, $(NUM_BITS))
   LDFLAGS = -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc
   EXTRA_CFLAGS=-m64
else
   LDFLAGS = -L$(SYSTEMC_HOME)/lib-linux -lsystemc
   EXTRA_CFLAGS=-m32
endif

endif

############ Solaris ##########
ifeq ($(shell uname -s), SunOS)

CC = g++
LD = g++
OPT    = -O2
DEBUG  = -g
OTHER  = -Wall -MMD -I. -I$(TLM_HOME) -I$(SYSTEMC_HOME)/include

ifeq (64, $(NUM_BITS))
   LDFLAGS = -L$(SYSTEMC_HOME)/lib-gccsparcOS5 -lsystemc
   EXTRA_CFLAGS=-m64
else
   LDFLAGS = -L$(SYSTEMC_HOME)/lib-gccsparcOS5 -lsystemc
   EXTRA_CFLAGS=-m32
endif

endif

########## Windows w/ cygwin #########

ifeq ($(shell uname -o), Cygwin)

CC = cl.exe
LD = link.exe /SUBSYSTEM:CONSOLE /nologo
OPT    = /O2
DEBUG  = /Od /ZI
OTHER  =  /GR /vmg /MD /EHsc /nologo /I$(TLM_HOME) /I"$(SYSTEMC_HOME)/include"

LDFLAGS = /LIBPATH:"$(SYSTEMC_HOME)/msvc71/SystemC/Release" systemc.lib
EXTRA_CFLAGS=

endif


ifeq ($(MODE), debug)
   CFLAGS = $(DEBUG) $(OTHER) $(EXTRA_CFLAGS)
else
   CFLAGS = $(OPT) $(OTHER) $(EXTRA_CFLAGS)
endif

###############################################################

EXE       =  tlm_gp_extensions

SRCS      := $(wildcard ./*.cpp)

OBJS      = $(SRCS:%.cpp=%.o) 

#------------------------------------------------------------------------------
# Rules
#------------------------------------------------------------------------------

all: $(EXE)

debug:
	$(MAKE) MODE=debug

clean:
	rm -rf *.o *.d $(EXE)


ifeq ($(shell uname -o), Cygwin)
$(EXE): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) /OUT:$@
else
$(EXE): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@
endif

ifeq ($(shell uname -o), Cygwin)
%.o : %.cpp
	$(CC) $(CFLAGS) -c -Fo"$@" $<
else
%.o : %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
endif

-include $(ODIR)/*.d
