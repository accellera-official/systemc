MODULE = at_1_phase

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

OBJS = \
	$(MODULE).o \
	$(MODULE)_top.o \
	initiator_top.o \
	traffic_generator.o \
	memory.o \
	report.o \
	at_target_1_phase.o \
	select_initiator.o

-include ../../build-unix/Makefile.config
 include ../../build-unix/Makefile.rules

# DO NOT DELETE
