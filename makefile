
all: GWOC

F6.o: F6.c
	gcc -c F6.c 

RngStream.o: RngStream.c
	gcc -c RngStream.c

GWO-C.o: GWO-C.c
	gcc -c GWO-C.c

main.o: main.c
	gcc -c main.c 

GWOC: main.o GWO-C.o RngStream.o F6.o
	gcc main.o GWO-C.o RngStream.o F6.o -o GWOC

clean:
	rm -rf *o GWOC