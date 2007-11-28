
SUBDIRS = \
	../GenericPayload \
	../GenericPayloadEndianness \
	../GenericPayloadExtension \
	../GenericPayloadSerializers \
	../GenericPayloadBurst \
        ../GenericPayloadMixEndianness

.PHONY: all clean run check

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-cygwin && $(MAKE)); done

check:
	@for i in $(filter-out ../GenericPayloadEndianness, $(SUBDIRS)); do \
        (cd $$i/build-cygwin && $(MAKE) check); done

run:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-cygwin && $(MAKE) run); done

clean:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-cygwin && $(MAKE) clean); done
