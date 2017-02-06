include ../../../build-unix/Makefile.config

PROJECT = lt

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

OBJS = \
	$(PROJECT).o \
	$(PROJECT)_top.o \
	lt_initiator.o \
	\
	at_target_1_phase.o \
	lt_target.o \
	initiator_top.o \
	memory.o \
	report.o \
	traffic_generator.o

include ../../../build-unix/Makefile.rules

# DO NOT DELETE
