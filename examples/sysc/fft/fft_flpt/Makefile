SYSTEMC_HOME ?= ../../../..
include ../../../build-unix/Makefile.config

PROJECT = fft_flpt
SRCS    = $(wildcard *.cpp)
OBJS    = $(SRCS:.cpp=.o)
EXTRA_CLEAN = out_real out_imag

include ../../../build-unix/Makefile.rules
