CC = g++
CFLAGS =-c -Wall



all: main.o scene.o objects.o
	$(CC) main.o scene.o objects.o -o ray

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

scene.o: scene.cpp scene.h
	$(CC) $(CFLAGS) scene.cpp

objects.o: objects.cpp objects.h
	$(CC) $(CFLAGS) objects.cpp

clean:
	rm *.o

