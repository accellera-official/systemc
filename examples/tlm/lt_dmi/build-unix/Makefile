include ../../../build-unix/Makefile.config

PROJECT = lt_dmi

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
	dmi_memory.o \
	lt_dmi_initiator.o \
	lt_dmi_target.o \
	memory.o \
	report.o \
	traffic_generator.o

include ../../../build-unix/Makefile.rules

# DO NOT DELETE
