include ./Makefile.config

export FLAG_BATCH = 1

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
	else \
		for i in $(SUBDIRS); do (cd $$i/build-unix && $(MAKE) clean); done; \
    fi
       	
