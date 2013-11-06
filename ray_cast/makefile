CC = g++
CFLAGS =-c -Wall



all: main.o scene.o picture.o vectors.o window.o specreflec.o
	$(CC) -Wall -g main.o scene.o picture.o vectors.o window.o specreflec.o -o ray

main.o: main.cpp vector.h
	$(CC) $(CFLAGS) main.cpp

scene.o: scene.cpp scene.h objects.h vector.h
	$(CC) $(CFLAGS) scene.cpp

picture.o: picture.cpp picture.h
	$(CC) $(CFLAGS) picture.cpp

vectors.o: vectors.cpp vectors.h
	$(CC) $(CFLAGS) vectors.cpp

window.o: window.cpp window.h
	$(CC) $(CFLAGS) window.cpp

specreflec.o:	specreflec.cpp specreflec.h
	$(CC) $(CFLAGS) specreflec.cpp

clean:
	rm -f *.o ray

