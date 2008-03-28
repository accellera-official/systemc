#Makefile for cygwin

include ./Makefile.config

export FLAG_BATCH = 1

SUBDIRS = \
          ../bus \
          ../bus_dmi \
          ../gp_extension \
          ../p2p \
          ../static_extensions

.PHONY: all clean run

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-cygwin && $(MAKE)); done

run:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-cygwin && $(MAKE) run); done

check:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-cygwin && $(MAKE) check); done

clean:
	@if ( test ! -d "$(SYSTEMC_HOME)" ); \
		then echo 'SYSTEMC_HOME [$(SYSTEMC_HOME)] is not present. Update $(PWD)/Makefile.config'; \
	elif ( test ! -d "$(TLM_HOME)" ); \
		then echo 'TLM_HOME [$(TLM_HOME)] is not present. Update $(PWD)/Makefile.config'; \
	else \
		for i in $(SUBDIRS); do (cd $$i/build-cygwin && $(MAKE) clean); done; \
    fi
       	
