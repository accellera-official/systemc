MODULE = at_extension_optional

VPATH = \
	../src \
	../../common/src

INCDIR = \
	-I../include \
	-I../../common/include

CXXFLAGS += -DUSING_EXTENSION_OPTIONAL

OBJS = \
	$(MODULE).o \
	$(MODULE)_top.o \
	initiator_top.o \
	traffic_generator.o \
	memory.o \
	report.o \
	at_target_4_phase.o \
	at_target_2_phase.o \
	select_initiator.o \
	extension_initiator_id.o

-include ../../build-unix/Makefile.config
 include ../../build-unix/Makefile.rules

# DO NOT DELETE
