
SUBDIRS = \
	../GenericPayload \
	../GenericPayloadExtension \

.PHONY: all clean check

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE)); done

check:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE) check); done

clean:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE) clean); done
