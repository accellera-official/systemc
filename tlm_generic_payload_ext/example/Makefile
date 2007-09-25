EXE             = tlm_gp-example.exe

CXX	        = g++
CXXFLAGS        = -O -g

INCLUDES  = -I./ -I../ -I$(SYSTEMC_HOME)/include 
LIB_DIR   = -L$(SYSTEMC_HOME)/lib-$(TARGET_ARCH) 
LIBS      = -lsystemc -lm

SRCS      := $(wildcard ./*.cpp)

OBJS      = $(SRCS:%.cpp=%.o) 

#-------------------------------------------------------------------------------
# Rules
#-------------------------------------------------------------------------------

make: $(EXE)

clean:
	rm -rf *.o $(EXE) 


$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) $(LIBS) $(LIB_DIR) -o $@
