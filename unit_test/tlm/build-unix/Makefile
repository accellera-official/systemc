include ./Makefile.config

SUBDIRS = \
 	../bus \
	../bus_dmi \
 	../gp_extension \
	../p2p \
	../transactor

.PHONY: all clean

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE)); done

clean:
	@if ( test ! -d $(SYSTEMC_HOME) ); \
		then echo 'SYSTEMC_HOME [$(SYSTEMC_HOME)] is not present. Update $(PWD)/Makefile.config'; \
	elif ( test ! -d $(TLM_HOME) ); \
		then echo 'TLM_HOME [$(TLM_HOME)] is not present. Update $(PWD)/Makefile.config'; \
	else \
		for i in $(SUBDIRS); do (cd $$i/build-unix && $(MAKE) clean); done; \
    fi
       	
