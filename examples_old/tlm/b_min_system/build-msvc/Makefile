# MSVC NMAKE makefile
!include <..\..\build-msvc\Makefile.config>

OBJ_NAME = b_min_system

MY_INCLUDES = /I..\include \
              /I..\example_bus\include \
              /I..\example_protocol\include
              
SRCDIR = ..\src
SRCBUSDIR = ..\example_bus\src
SRCPROTOCOLDIR = ..\example_protocol\src

CFLAGS = $(FLAGS) $(MY_INCLUDES)

OBJS   = $(OBJ_NAME).obj \
          top.obj \
          simple_memory.obj \
          traffic_generator.obj \
          rw_initiator_socket.obj \
          rw_master_base.obj \
          rw_slave_base.obj \
          rw_target_socket.obj

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

top.obj: $(SRCDIR)\top.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

simple_memory.obj: $(SRCDIR)\simple_memory.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

traffic_generator.obj: $(SRCDIR)\traffic_generator.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

rw_initiator_socket.obj: $(SRCPROTOCOLDIR)\rw_initiator_socket.cpp
	$(CC) /c $(CFLAGS) /Fo"$@"  %s

rw_master_base.obj: $(SRCPROTOCOLDIR)\rw_master_base.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

rw_slave_base.obj: $(SRCPROTOCOLDIR)\rw_slave_base.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

rw_target_socket.obj: $(SRCPROTOCOLDIR)\rw_target_socket.cpp
	$(CC) /c $(CFLAGS) /Fo"$@" %s

clean:
	del $(OBJ_NAME).exe *.obj run.log diff.log *.idb *.pdb *ilk

