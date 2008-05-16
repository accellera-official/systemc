include ./Makefile.config

export FLAG_BATCH = 1

SUBDIRS = \
          ../bus \
          ../bus_dmi \
          ../p2p \
          ../static_extensions \
          ../endian_conv \
          ../multi_sockets

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
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE) clean); done;

