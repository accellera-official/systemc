#Makefile for cygwin

include ./Makefile.config

SUBDIRS = \
 	../bus \
	../bus_dmi \
 	../gp_extension \
	../p2p \
	../transactor \
        ../accessors \
        ../data_types

.PHONY: all clean run

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE)); done

run:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE) run); done

clean:
	@if ( test ! -d "$(SYSTEMC_HOME)" ); \
		then echo 'SYSTEMC_HOME [$(SYSTEMC_HOME)] is not present. Update $(PWD)/Makefile.config'; \
	elif ( test ! -d "$(TLM_HOME)" ); \
		then echo 'TLM_HOME [$(TLM_HOME)] is not present. Update $(PWD)/Makefile.config'; \
	else \
		for i in $(SUBDIRS); do (cd $$i/build-windows && $(MAKE) clean); done; \
    fi
       	
