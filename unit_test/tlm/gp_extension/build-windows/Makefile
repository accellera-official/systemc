
SUBDIRS = \
	../GenericPayload \
	../GenericPayloadExtension \

.PHONY: all clean run

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE)); done

run:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE) run); done

clean:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-windows && $(MAKE) clean); done
