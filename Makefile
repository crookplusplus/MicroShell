# Authored by Chris Rook

#UPDATING CHECKLIST
#Add object file name to objs var
#Make file set method structure ONE PER FILE

# CXX Make variable for compiler
CXX=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -Wall -g3 -c 

# object files
OBJS = main.o microshell.o

# Program name
PROGRAM = msh

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CXX) -o $(PROGRAM) $(OBJS) 

#structure for object file creation

microshell.o : microshell.cpp microshell.h
	$(CXX) $(CXXFLAGS) microshell.cpp

main.o : main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

clean :
	rm -f *.o *~ $(PROGRAM)