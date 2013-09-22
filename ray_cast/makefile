CC = g++
CFLAGS =-c -Wall



all: main.o scene.o objects.o picture.o vectors.o window.o
	$(CC) main.o scene.o objects.o picture.o vectors.o window.o -o ray

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

scene.o: scene.cpp scene.h
	$(CC) $(CFLAGS) scene.cpp

objects.o: objects.cpp objects.h
	$(CC) $(CFLAGS) objects.cpp

picture.o: picture.cpp picture.h
	$(CC) $(CFLAGS) picture.cpp

vectors.o: vectors.cpp vectors.h
	$(CC) $(CFLAGS) vectors.cpp

window.o: window.cpp window.h
	$(CC) $(CFLAGS) window.cpp

clean:
	rm -f *.o ray

