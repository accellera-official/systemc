include ../../../build-unix/Makefile.config

PROJECT = lt_extension_mandatory

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

OBJS = \
	$(PROJECT).o \
	$(PROJECT)_top.o \
	lt_initiator_extension_mandatory.o \
	lt_target_extension_mandatory.o \
	\
	extension_initiator_id.o \
	report.o

include ../../../build-unix/Makefile.rules

# DO NOT DELETE
