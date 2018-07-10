CC=g++
flags=-Wall -std=c++14

prgm.exe: main.o Vec3.o Vertex.o Triangle.o Ray.o Model.o Image.o
	$(CC) main.o Vec3.o Vertex.o Triangle.o Ray.o Model.o Image.o -o prgm.exe

main.o: main.cpp
	$(CC) -c $(flags) main.cpp -o main.o

Image.o: Image.h Image.cpp
	$(CC) -c $(flags) Image.cpp -o Image.o

Model.o: Model.h Model.cpp
	$(CC) -c $(flags) Model.cpp -o Model.o
Ray.o: Ray.h Ray.cpp
	$(CC) -c $(flags) Ray.cpp -o Ray.o

Triangle.o: Triangle.h Triangle.cpp
	$(CC) -c $(flags) Triangle.cpp -o Triangle.o

Vertex.o: Vertex.cpp Vertex.h
	$(CC) -c $(flags) Vertex.cpp -o Vertex.o

Vec3.o: Vec3.cpp Vec3.h
	$(CC) -c $(flags) Vec3.cpp -o Vec3.o

clean:
	del *.swp *.un~ Makefile~ *.o *.exe *.ppm

new: clean prgm.exe
