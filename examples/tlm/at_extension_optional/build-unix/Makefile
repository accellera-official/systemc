include ../../../build-unix/Makefile.config

PROJECT = at_extension_optional

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

CXXFLAGS += -DUSING_EXTENSION_OPTIONAL

OBJS = \
	$(PROJECT).o \
	$(PROJECT)_top.o \
	initiator_top.o \
	\
	at_target_2_phase.o \
	at_target_4_phase.o \
	extension_initiator_id.o \
	memory.o \
	report.o \
	select_initiator.o \
	traffic_generator.o

include ../../../build-unix/Makefile.rules

# DO NOT DELETE
