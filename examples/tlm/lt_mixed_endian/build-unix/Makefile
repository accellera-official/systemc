MODULE = lt_mixed_endian

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

OBJS = \
	lt.o \
	lt_top.o \
	lt_initiator.o \
	memory.o \
	at_target_1_phase.o \
	at_target_4_phase.o \
	initiator_top.o \
	report.o \
	me_traffic_generator.o

INPUT  = ../results/input.txt
FILTER = grep "::"

-include ../../build-unix/Makefile.config
 include ../../build-unix/Makefile.rules

# DO NOT DELETE
