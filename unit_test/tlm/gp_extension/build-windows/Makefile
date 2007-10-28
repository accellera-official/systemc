
SUBDIRS = \
	../GenericPayload \
	../GenericPayloadExtension \
	../GenericPayloadSerializers \
	../GenericPayloadBurst

.PHONY: all clean run check

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE)); done

check:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE) check); done

run:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE) run); done

clean:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE) clean); done
