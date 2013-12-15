!include <..\..\build-msvc/Makefile.config>

PROJECT = at_extension_optional

CXXFLAGS_DEFINES = \
	$(CXXFLAGS_DEFINES) \
	/DUSING_EXTENSION_OPTIONAL

OBJS = \
	$(OBJDIR)\$(PROJECT).obj \
	$(OBJDIR)\$(PROJECT)_top.obj \
	$(OBJDIR)\initiator_top.obj \
	\
	$(OBJDIR)\at_target_2_phase.obj \
	$(OBJDIR)\at_target_4_phase.obj \
	$(OBJDIR)\extension_initiator_id.obj \
	$(OBJDIR)\memory.obj \
	$(OBJDIR)\report.obj \
	$(OBJDIR)\select_initiator.obj \
	$(OBJDIR)\traffic_generator.obj


!include <..\..\build-msvc/Makefile.rules>
