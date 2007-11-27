
SUBDIRS = \
	../GenericPayload \
	../GenericPayloadBurst \
	../GenericPayloadEndianness \
	../GenericPayloadExtension \
	../GenericPayloadSerializers \
    ../GenericPayloadMixEndianness

.PHONY: all clean run check

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
        (cd $$i/build-unix && $(MAKE) clean); done
