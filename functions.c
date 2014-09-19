#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "GWO-C.h"
#include "RngStream.h"

#define lb -100
#define ub 100

double function(int j, int dim, double pos[]){
	double sumF=0;
	int i=0;

	for (i = 0; i < dim; i++){
        sumF = sumF + pos[j] * pos[j];
    }
    return sumF;
}

double function7(int j, int dim, double pos[]){
	double sumF=0;
	int i=0;
	for (i = 0; i < dim; i++){
        sumF = sumF + (i*(pow(pos[i], 4)));
    }

    return sumF + (rand() / (double)RAND_MAX );
}

double function8(int j, int dim, double pos[]){
    double sumF=0;
	int i=0;
	for (i = 0; i < dim; i++){
        sumF = sumF + pos[j]*sin(sqrt(fabs(pos[j])));
    }
    return -sumF;
}

double function6(int j, int dim, double pos[]){
	double sumF=0;
	int i=0;
	for (i = 0; i < dim; i++){
        sumF = sumF + pow(pos[i] + 0.5, 2);
    }
    return sumF;
}

int main(int argc, char const *argv[]){

	srand(time(0));

	unsigned long germe[6] = { rand(), rand(), rand(), rand(), rand(), rand() };
    RngStream_SetPackageSeed (germe);
    RngStream g1 = RngStream_CreateStream ("Laplace");

    double (*fObj)(int, int, double[]);

    printf("Funcion 1\n");
    fObj=function;
    GWO(lb, ub, fObj, g1);
    showPack();
    
    printf("Funcion 6\n");
    fObj=function6;
    GWO(lb, ub, fObj, g1);
    showPack();

    printf("Funcion 7\n");
    fObj=function7;
    GWO(lb, ub, fObj, g1);
    showPack();

    printf("Funcion 8\n");
    fObj = function8;
    GWO(lb, ub, fObj, g1);
    showPack();

    RngStream_DeleteStream (&g1); 
    return 0;
}
