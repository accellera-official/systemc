include ../../build-unix/Makefile.config

PROJECTS := fir fir_rtl
COMMON_TARGETS := all build run check clean ultraclean

COMMON_SRCS = \
	stimulus.cpp \
	display.cpp 

fir_SRCS = \
	$(COMMON_SRCS) \
	fir.cpp \
	main.cpp 

fir_rtl_SRCS = \
	$(COMMON_SRCS) \
	fir_fsm.cpp \
	fir_data.cpp \
	main_rtl.cpp 

# project not set -> all projects
ifeq (,$(strip $(PROJECT)))

$(COMMON_TARGETS)::
	@for prj in $(PROJECTS) ; do  \
	  $(MAKE) $@ FLAG_BATCH=1 PROJECT=$$prj ; \
	done

else # module-specific rules

OBJS := $($(PROJECT)_SRCS:.cpp=.o)

include ../../build-unix/Makefile.rules

endif # project switch

## Taf!
