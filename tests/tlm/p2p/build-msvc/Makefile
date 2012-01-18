!include <..\..\build-msvc\Makefile.config>

SUBDIRS = \
	../BaseSocketLT \
	../CoreDecoupling \
	../EndEventLT \
	../RegisterSocketProcessLT \
	../SimpleAT \
	../SimpleAT_TA \
    ../HierarchicalSocket

#.PHONY: all clean run check

all:
	@for %%i in ($(SUBDIRS)) do \
        @(chdir %%i/build-msvc && $(MAKE) && chdir .. )

check:
	@for %%i in ($(SUBDIRS)) do \
        @(chdir %%i/build-msvc && $(MAKE) check && chdir ..)

run:
	@for %%i in ($(SUBDIRS)) do \
        @(chdir %%i/build-msvc && $(MAKE) run && chdir .. )

clean:
	@for %%i in ($(SUBDIRS)) do \
        @(chdir %%i/build-msvc && $(MAKE) clean && chdir ..)
