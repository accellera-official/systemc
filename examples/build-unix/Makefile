SUBDIRS := $(dir $(wildcard ../sysc/*/Makefile ../sysc/*/*/Makefile))
SUBDIRS += $(wildcard ../tlm/*/build-unix)

TARGETS := all build run check clean
.PHONY: $(TARGETS)

$(TARGETS): %:
	@set -e ; for i in $(SUBDIRS); do \
		$(MAKE) -C $$i FLAG_BATCH=1 $@ ; \
	done
