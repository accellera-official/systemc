!include <.\Makefile.config>

SUBDIRS = \
	..\at_1_phase \
	..\at_2_phase \
	..\at_4_phase \
	..\at_extension_optional \
	..\at_mixed_targets \
	..\at_ooo \
	..\lt \
	..\lt_dmi \
	..\lt_extension_mandatory \
	..\lt_mixed_endian \
	..\lt_temporal_decouple

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
