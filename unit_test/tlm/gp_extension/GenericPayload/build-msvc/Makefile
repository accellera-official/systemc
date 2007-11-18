!include <..\..\..\build-msvc/Makefile.config>

OBJ_NAME = generic_payload

MY_INCLUDES = /I..\include \
              /I..\..\common\include \
              /I..\..\common\include\SimpleSocket \
              /I..\..\common\include\models \
              /I..\..\common\include\utils
              
SRCDIR = ..\src
SRCCOMMONDIR = ..\..\common\src

CFLAGS = $(FLAGS) $(MY_INCLUDES)

OBJS   = $(OBJ_NAME).obj tlm_target.obj

#.PHONY: clean all run check announce

all: announce $(OBJ_NAME).exe

announce:
	@if  not defined $(FLAG_BATCH) echo. , echo $(OBJ_NAME):  
	
	
check: announce $(OBJ_NAME).exe
	@.\$(OBJ_NAME).exe > run.log
	@(fc run.log ..\results\expected.log > diff.log ) & if ERRORLEVEL 1  (echo "***ERROR:" & type diff.log) else (echo OK) 

run: announce $(OBJ_NAME).exe
	@.\$(OBJ_NAME).exe


$(OBJ_NAME).exe: $(OBJS)
	$(LD) /OUT:"$@" $(LDFLAGS) $(OBJS)

$(OBJ_NAME).obj: $(SRCDIR)\$(OBJ_NAME).cpp
	$(CC) /c $(CFLAGS) %s

tlm_target.obj: $(SRCDIR)\tlm_target.cpp
	$(CC) /c $(CFLAGS) %s

clean:
	del $(OBJ_NAME).exe *.obj run.log diff.log *.idb *.pdb *ilk

