export FLAG_BATCH = 1

SUBDIRS := $(dir $(wildcard */Makefile */*/Makefile))

TARGETS := all clean check run

.PHONY: $(TARGETS)

$(TARGETS): %:
	@set -e ; for i in $(SUBDIRS); do \
		$(MAKE) -C $$i $@ ; \
	done
