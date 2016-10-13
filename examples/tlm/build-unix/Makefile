SUBDIRS := \
	../at_1_phase \
	../at_2_phase \
	../at_4_phase \
	../at_extension_optional \
	../at_mixed_targets \
	../at_ooo \
	../lt \
	../lt_dmi \
	../lt_extension_mandatory \
	../lt_mixed_endian \
	../lt_temporal_decouple

TARGETS := build run check clean
.PHONY: $(TARGETS)

## default target
all: build

## forwarding rule
$(TARGETS):
	@set -e ; for i in $(SUBDIRS); do \
		$(MAKE) -C $$i/build-unix FLAG_BATCH=1 $@ ; \
	done
