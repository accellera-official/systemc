include ./Makefile.config

export FLAG_BATCH = 1

SUBDIRS = \
          ../lt_min_system \
          ../tlm_1_integration

.PHONY: all clean check run

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE)); done

check:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE) check); done

run:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE) run); done

clean:
	@if ( test ! -d $(SYSTEMC_HOME) ); \
		then echo 'SYSTEMC_HOME [$(SYSTEMC_HOME)] is not present. Update $(PWD)/Makefile.config'; \
	elif ( test ! -d $(TLM_HOME) ); \
		then echo 'TLM_HOME [$(TLM_HOME)] is not present. Update $(PWD)/Makefile.config'; \
	else \
		for i in $(SUBDIRS); do (cd $$i/build-unix && $(MAKE) clean); done; \
    fi
       	
