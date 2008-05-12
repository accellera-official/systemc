# MSVC NMAKE makefile
!include <..\..\build-msvc/Makefile.config>

OBJ_NAME = lt_min_system

MY_INCLUDES = /I..\include \
              /I..\..\common\include \
              /I..\..\common\include\SimpleSocket \
              /I..\..\common\include\models \
              /I..\..\common\include\utils
              
SRCDIR = ..\src
SRCCOMMONDIR = ..\..\common\src

CFLAGS = $(FLAGS) $(MY_INCLUDES)

OBJS   = $(OBJ_NAME).obj lt_initiator.obj lt_processor_abstraction.obj traffic_generator.obj lt_target_memory.obj lt_top.obj mtrand.obj

#.PHONY: clean all run check announce

all: announce $(OBJ_NAME).exe

announce:
	@if  not defined $(FLAG_BATCH) echo. , echo $(OBJ_NAME):  
	
	
check: announce $(OBJ_NAME).exe
	@.\$(OBJ_NAME).exe > run.log
	@(fc run.log ..\results\expected.log > diff.log) & if ERRORLEVEL 1  (echo "***ERROR:" & type diff.log) else (echo OK) 

run: announce $(OBJ_NAME).exe
	@.\$(OBJ_NAME).exe


$(OBJ_NAME).exe: $(OBJS)
	$(LD) /OUT:"$@" $(LDFLAGS) $(OBJS)

$(OBJ_NAME).obj: $(SRCDIR)\$(OBJ_NAME).cpp
	$(CC) /c $(CFLAGS) %s

lt_top.obj: $(SRCDIR)\lt_top.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

lt_initiator.obj: $(SRCCOMMONDIR)\lt_initiator.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

lt_processor_abstraction.obj: $(SRCCOMMONDIR)\lt_processor_abstraction.cpp
	$(CC) /c $(CFLAGS) /Fo"$@"  %s

traffic_generator.obj: $(SRCCOMMONDIR)\traffic_generator.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

lt_target_memory.obj: $(SRCCOMMONDIR)\lt_target_memory.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

mtrand.obj: $(SRCCOMMONDIR)\mtrand.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

clean:
	del $(OBJ_NAME).exe *.obj run.log diff.log *.idb *.pdb *ilk

