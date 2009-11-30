LINUX =	Linux
MAC = Darwin

#ifeq	($(OSNAME),$(LINUX))
	GL = -lGL -lGLU -lglut
#else
#	GL = -framework OpenGL -framework GLUT -framework Cocoa
#endif

all: main

main: Simulator.o Environment.o Behavior.o Cell.o Circle.o Formation.o Robot.o Vector.o Neighborhood.o
	g++ $(GL) -o Simulator Simulator.o Environment.o Behavior.o Cell.o Circle.o Formation.o Robot.o Vector.o Neighborhood.o
	
Simulator.o: Simulator.cpp
	g++ -c Simulator.cpp

Behavior.o: Behavior.cpp
	g++ -c Behavior.cpp
	
Cell.o: Cell.cpp
	g++ -c Cell.cpp
	
Circle.o: Circle.cpp
	g++ -c Circle.cpp 
	
Environment.o: Environment.cpp
	g++ -c Environment.cpp
	
Formation.o: Formation.cpp
	g++ -c Formation.cpp
	
Robot.o: Robot.cpp
	g++ -c Robot.cpp
	
Vector.o: Vector.cpp
	g++ $(GL) -c Vector.cpp

Neighborhood.o: Neighborhood.cpp
	g++ $(GL) -c Neighborhood.cpp
		
clean:
	rm -rf *.o *~ Simulator 
	
