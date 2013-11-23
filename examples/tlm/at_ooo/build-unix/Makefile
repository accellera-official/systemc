MODULE = at_ooo

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
	at_target_2_phase.o \
	at_target_ooo_2_phase.o \
	memory.o \
	report.o \
	select_initiator.o \
	traffic_generator.o

-include ../../build-unix/Makefile.config
 include ../../build-unix/Makefile.rules

# DO NOT DELETE
