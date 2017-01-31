!include <..\..\..\build-msvc/Makefile.config>

PROJECT = lt_mixed_endian

OBJS = \
	$(OUTDIR)\lt.obj \
	$(OUTDIR)\lt_top.obj \
	$(OUTDIR)\initiator_top.obj \
	$(OUTDIR)\me_traffic_generator.obj \
	\
	$(OUTDIR)\at_target_1_phase.obj \
	$(OUTDIR)\at_target_4_phase.obj \
	$(OUTDIR)\lt_initiator.obj \
	$(OUTDIR)\memory.obj \
	$(OUTDIR)\report.obj

INPUT  = ../results/input.txt
FILTER = "::"

!include <..\..\..\build-msvc/Makefile.rules>
