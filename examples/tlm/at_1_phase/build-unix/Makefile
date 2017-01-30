include ../../../build-unix/Makefile.config

PROJECT = at_1_phase

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
	\
	at_target_1_phase.o \
	memory.o \
	report.o \
	select_initiator.o \
	traffic_generator.o

include ../../../build-unix/Makefile.rules

# DO NOT DELETE
