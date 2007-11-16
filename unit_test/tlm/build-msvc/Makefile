!include <.\Makefile.config>

SUBDIRS = \
          ../bus \
          ../bus_dmi \
          ../gp_extension \
          ../p2p \
          ../transactor \
          ../static_extensions
          
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

