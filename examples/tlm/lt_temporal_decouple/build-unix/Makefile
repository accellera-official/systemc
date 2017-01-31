include ../../../build-unix/Makefile.config

PROJECT = lt_temporal_decouple

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

OBJS = \
	$(PROJECT).o \
	$(PROJECT)_top.o \
	initiator_top.o \
	td_initiator_top.o \
	\
	lt_initiator.o \
	lt_synch_target.o \
	lt_target.o \
	lt_td_initiator.o \
	memory.o \
	report.o \
	traffic_generator.o

include ../../../build-unix/Makefile.rules

# DO NOT DELETE
