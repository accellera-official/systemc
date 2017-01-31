!include <.\Makefile.config>

SUBDIRS = \
	..\tlm\at_1_phase \
	..\tlm\at_2_phase \
	..\tlm\at_4_phase \
	..\tlm\at_extension_optional \
	..\tlm\at_mixed_targets \
	..\tlm\at_ooo \
	..\tlm\lt \
	..\tlm\lt_dmi \
	..\tlm\lt_extension_mandatory \
	..\tlm\lt_mixed_endian \
	..\tlm\lt_temporal_decouple

TARGETS = build run check clean

# default target
all: build

# see
#  - Makefile.rules  for actual implementation
#  - Makefile.config for available configuration options
$(TARGETS):
	@for %%i in ($(SUBDIRS)) do \
	  @( chdir $(MAKEDIR)\%%i\build-msvc && \
	    $(MAKE) /nologo FLAG_BATCH=1 \
	      CONFIG=$(CONFIG) PLATFORM=$(PLATFORM) $@ )
