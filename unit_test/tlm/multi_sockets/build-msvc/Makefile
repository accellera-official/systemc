!include <..\..\build-msvc/Makefile.config>

OBJ_NAME = multi_sockets

INCDIR = -I../../common/include/models -I../include -I$(BOOST_HOME)
SRCDIR = ..\src
SRCCOMMONDIR = ..\..\common\src

CFLAGS = $(FLAGS) $(INCDIR)

OBJS   = $(OBJ_NAME).obj

!IFDEF BOOST_HOME

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

clean:
	del $(OBJ_NAME).exe *.obj run.log diff.log *.idb *.pdb *ilk

!ELSE
all:
	@echo Variable BOOST_HOME not specified. Set it so that it points to your boost installation \(one level above /boost\).
!ENDIF