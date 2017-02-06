!include <..\..\..\build-msvc\Makefile.config>

PROJECT = at_extension_optional

CXXFLAGS_DEFINES = \
	$(CXXFLAGS_DEFINES) \
	/DUSING_EXTENSION_OPTIONAL

OBJS = \
	$(OUTDIR)\$(PROJECT).obj \
	$(OUTDIR)\$(PROJECT)_top.obj \
	$(OUTDIR)\initiator_top.obj \
	\
	$(OUTDIR)\at_target_2_phase.obj \
	$(OUTDIR)\at_target_4_phase.obj \
	$(OUTDIR)\extension_initiator_id.obj \
	$(OUTDIR)\memory.obj \
	$(OUTDIR)\report.obj \
	$(OUTDIR)\select_initiator.obj \
	$(OUTDIR)\traffic_generator.obj


!include <..\..\..\build-msvc/Makefile.rules>
