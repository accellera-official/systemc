
SUBDIRS = \
	../BaseSocketLT \
	../CoreDecoupling \
	../EndEventLT \
	../RegisterSocketProcessLT \
	../SimpleAT \
	../SimpleAT_TA

.PHONY: all clean

all:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE)); done

clean:
	@for i in $(SUBDIRS); do \
        (cd $$i/build-unix && $(MAKE) clean); done
