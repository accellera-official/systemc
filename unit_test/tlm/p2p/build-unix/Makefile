
SUBDIRS = \
	../BaseSocketLT \
	../CoreDecoupling \
	../EndEventLT \
	../RegisterSocketProcessLT \
	../SimpleAT \
	../SimpleAT_TA \
        ../HierarchicalSocket

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
        (cd $$i/build-unix && $(MAKE) clean); done
