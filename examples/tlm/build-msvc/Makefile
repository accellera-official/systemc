!include <.\Makefile.config>

SUBDIRS = \
          ../at_1_phase \
          ../at_2_phase \
          ../at_4_phase \
          ../at_extension_optional \
          ../at_mixed_targets \
          ../at_ooo \
          ../lt \
          ../lt_dmi \
          ../lt_extension_mandatory \
          ../lt_mixed_endian \
          ../lt_temporal_decouple

#.PHONY: all clean run check

all: dummy
	@for %%i in ($(SUBDIRS)) do \
		@(chdir %%i/build-msvc && $(MAKE) && chdir .. )

check: dummy
	@for %%i in ($(SUBDIRS)) do \
		@(chdir %%i/build-msvc && $(MAKE) check && chdir ..)

run: dummy
	@for %%i in ($(SUBDIRS)) do \
		@(chdir %%i/build-msvc && $(MAKE) run && chdir .. )

clean: dummy
	@for %%i in ($(SUBDIRS)) do \
		@(chdir %%i/build-msvc && $(MAKE) clean && chdir ..)

dummy:
	@set FLAG_BATCH = 1
