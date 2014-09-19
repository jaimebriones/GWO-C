all: exec

exec: GWO-C.o RngStream.o F6.o
	gcc RngStream.o GWO-C.o F6.o -o exec -lm -Wall

F6.o: F6.c
	gcc -c F6.c -lm

RngStream.o: RngStream.c RngStream.h 
	gcc -c RngStream.c -lm

GWO-C.o: GWO-C.c GWO-C.h
	gcc -c GWO-C.c -lm

clean:
	rm -rf *o exec