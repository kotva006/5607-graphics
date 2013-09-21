CC = g++
CFLAGS =-c -Wall



all: main.o scene.o objects.o picture.o vec.o
	$(CC) main.o scene.o objects.o picture.o -o ray

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

scene.o: scene.cpp scene.h vec.h
	$(CC) $(CFLAGS) scene.cpp

objects.o: objects.cpp objects.h vec.h
	$(CC) $(CFLAGS) objects.cpp

picture.o: picture.cpp picture.h vec.h
	$(CC) $(CFLAGS) picture.cpp

vec.o: vec.cpp vec.h
	$(CC) $(CFLAGS) vec.cpp

clean:
	rm -f *.o ray

