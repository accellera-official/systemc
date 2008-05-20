!include <..\..\..\build-msvc/Makefile.config>

OBJ_NAME = simple_AT

MY_INCLUDES = /I..\..\..\common\include\models
              
SRCDIR = ..\src
SRCCOMMONDIR = ..\..\common\src

CFLAGS = $(MY_INCLUDES) $(FLAGS)

OBJS   = $(OBJ_NAME).obj

#.PHONY: clean all run check announce

all: announce $(OBJ_NAME).exe

announce:
	@if  not defined $(FLAG_BATCH) echo. , echo $(OBJ_NAME):  
	
	
# Notes: 
# 1. two lines starting @type filter out lines containing
#    "stopped by user" so that differences in messages between SystemC 2.2.0
#    and SystemC 2.2.1 are not significant. 
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

clean:
	del $(OBJ_NAME).exe *.obj *.log *.idb *.pdb *ilk

