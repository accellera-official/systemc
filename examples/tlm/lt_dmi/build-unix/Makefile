MODULE = lt_dmi

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

OBJS = \
	$(MODULE).o \
	$(MODULE)_top.o \
	lt_dmi_initiator.o \
	lt_dmi_target.o \
	initiator_top.o \
	memory.o \
	dmi_memory.o \
	report.o \
	traffic_generator.o

-include ../../build-unix/Makefile.config
 include ../../build-unix/Makefile.rules

# DO NOT DELETE
