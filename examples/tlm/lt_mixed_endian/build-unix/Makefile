include ../../../build-unix/Makefile.config

PROJECT = lt_mixed_endian

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

OBJS = \
	lt.o \
	lt_top.o \
	initiator_top.o \
	me_traffic_generator.o \
	\
	at_target_1_phase.o \
	at_target_4_phase.o \
	lt_initiator.o \
	memory.o \
	report.o

INPUT  = ../results/input.txt
FILTER = grep "::"

include ../../../build-unix/Makefile.rules

# DO NOT DELETE
