include ./Makefile.config

export FLAG_BATCH = 1

SUBDIRS = \
          ../at_1_phase \
          ../at_2_phase \
          ../at_4_phase \
          ../at_ooo \
          ../at_mixed_targets \
          ../at_extension_optional \
          ../at_extension_mandatory \
          ../lt

#          ../at_dmi \
#          ../at_1_phase_temporal_decoupling \
#          ../at_endianess \
#          ../lt_temporal_decoupling  

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
       	
