C = gcc
FLAGS = -Wall -g
.DEFAULT_GOAL = all

all: raytracer

raytracer: raycast.o rayutils.o rayengine.o vector3D.o parser.o filehandler.o
	$(C) $(FLAGS) -o raytracer raycast.o rayutils.o rayengine.o parser.o vector3D.o filehandler.o -lm	

raycast.o: ./Core/raycast.c
	$(C) $(FLAGS) -c ./Core/raycast.c

vector3D.o: ./Math/vector3D.c
	$(C) $(FLAGS) -c ./Math/vector3D.c

parser.o: ./Parsing/parser.c
	$(C) $(FLAGS) -c ./Parsing/parser.c	

rayengine.o: ./Core/rayengine.c
	$(C) $(FLAGS) -c ./Core/rayengine.c	

rayutils.o: ./Utility/rayutils.c
	$(C) $(FLAGS) -c ./Utility/rayutils.c	

filehandler.o: ./Parsing/filehandler.c
	$(C) $(FLAGS) -c ./Parsing/filehandler.c	

clean:
	rm raytracer raycast.o rayutils.o parser.o filehandler.o vector3D.o rayengine.o