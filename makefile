all: exec

exec: GWO-C.o RngStream.o functions.o
	gcc RngStream.o GWO-C.o functions.o -o exec -lm -Wall

functions.o: functions.c
	gcc -c functions.c -lm

RngStream.o: RngStream.c RngStream.h 
	gcc -c RngStream.c -lm

GWO-C.o: GWO-C.c GWO-C.h
	gcc -c GWO-C.c -lm

clean:
	rm -rf *o exec