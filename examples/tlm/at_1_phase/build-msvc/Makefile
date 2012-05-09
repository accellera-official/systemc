!include <..\..\build-msvc/Makefile.config>

OBJ_NAME = at_1_phase

SRCDIR           = ../src
SRCDIRCOMMON     = ../../common/src

INCDIR =    -I../include \
            -I../../common/include 

# Note: putting $(INCDIR) before $(FLAGS) should ensure the correct 
# version of memory.h is found when building with Visual C++ 2005
CFLAGS = $(INCDIR) $(FLAGS)

OBJS   = $(OBJ_NAME).obj \
            $(OBJ_NAME)_top.obj \
            initiator_top.obj \
            traffic_generator.obj \
            memory.obj \
            report.obj \
            at_target_1_phase.obj \
            select_initiator.obj

#.PHONY: clean all run check announce

all: announce $(OBJ_NAME).exe

announce:
	@if  not defined $(FLAG_BATCH) echo. , echo $(OBJ_NAME):

# Notes:
# 1. two lines starting @type filter out lines containing
#    "stopped by user" so that differences in messages between SystemC 2.2.0
#    and SystemC 2.3.0 are not significant.
# 2. The full path to find is used as some users may have cygwin installed,
#    which includes another version of find
# 3. find /v returns all lines that don't match. fc /w ignores (collapses)
#    white space
check: announce $(OBJ_NAME).exe
	@.\$(OBJ_NAME).exe > runtemp.log
	@type runtemp.log | $(WINDIR)\system32\find /v /i "stopped by user" >run.log
	@type ..\results\expected.log | $(WINDIR)\system32\find /v /i "stopped by user" >expected.log
	@(fc /w run.log expected.log > diff.log) & if ERRORLEVEL 1  (echo "***ERROR:" & type diff.log) else (echo OK) 

run: announce $(OBJ_NAME).exe
	@.\$(OBJ_NAME).exe


$(OBJ_NAME).exe: $(OBJS)
	$(LD) /OUT:"$@" $(LDFLAGS) $(OBJS)

$(OBJ_NAME).obj: $(SRCDIR)\$(OBJ_NAME).cpp
	$(CC) /c $(CFLAGS) %s

$(OBJ_NAME)_top.obj: $(SRCDIR)/$(OBJ_NAME)_top.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

initiator_top.obj: $(SRCDIR)/initiator_top.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

traffic_generator.obj: $(SRCDIRCOMMON)/traffic_generator.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

memory.obj: $(SRCDIRCOMMON)/memory.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

report.obj: $(SRCDIRCOMMON)/report.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

at_target_1_phase.obj: $(SRCDIRCOMMON)/at_target_1_phase.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

select_initiator.obj: $(SRCDIRCOMMON)/select_initiator.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

clean:
	del $(OBJ_NAME).exe *.obj *.log *.idb *.pdb *ilk
