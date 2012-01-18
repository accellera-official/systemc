!include <.\Makefile.config>

SUBDIRS = \
          ../bus \
          ../bus_dmi \
          ../p2p \
          ../static_extensions \
          ../endian_conv \
          ../multi_sockets \
          ../update_original \
          ../cancel_all \
          ../nb2b_adapter
          
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

