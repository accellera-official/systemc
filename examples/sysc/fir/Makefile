MODULES := fir fir_rtl
COMMON_TARGETS := all run check clean ultraclean

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

# module not set -> all modules
ifeq (,$(strip $(MODULE)))

$(COMMON_TARGETS)::
	@for mod in $(MODULES) ; do  \
	  $(MAKE) $@ FLAG_BATCH=1 MODULE=$$mod ; \
	done

else # module-specific rules

OBJS := $($(MODULE)_SRCS:.cpp=.o)

-include ../Makefile.config
 include ../Makefile.rules

endif # module switch

## Taf!
