all: c4

c4: main.o c4.o c4l.o
	gcc -o c4 main.o c4.o c4l.o -llua

main.o: main.c c4.h
	gcc -c main.c

c4.o: c4.c c4.h c4l.h
	gcc -c c4.c

c4l.o: c4l.c c4l.h c4.h
	gcc -c c4l.c

clean:
	rm -f c4 *.o
