include ./Makefile.config

export FLAG_BATCH = 1

SUBDIRS = \
          ../b_min_system \
          ../lt_min_system \
          ../lt_temporal_decoupling \
          ../lt_direct_memory_interface \
          ../lt_endianness \
          ../tlm_1_integration \
          ../tlm_1_integration_explicit_slave \
          ../at_basic_system \
          ../at_lt_full_system \
          ../gp_optional_extension

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
       	
