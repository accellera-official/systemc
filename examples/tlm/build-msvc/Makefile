!include <.\Makefile.config>

SUBDIRS = \
          ..\lt_min_system \
          ../tlm_1_integration
          
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

