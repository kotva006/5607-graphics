CC = g++
CFLAGS =-c -Wall



all: main.o scene.o picture.o vectors.o window.o
	$(CC) main.o scene.o picture.o vectors.o window.o -o ray

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

scene.o: scene.cpp scene.h objects.h
	$(CC) $(CFLAGS) scene.cpp

picture.o: picture.cpp picture.h
	$(CC) $(CFLAGS) picture.cpp

vectors.o: vectors.cpp vectors.h
	$(CC) $(CFLAGS) vectors.cpp

window.o: window.cpp window.h
	$(CC) $(CFLAGS) window.cpp

clean:
	rm -f *.o ray sample.ppm

