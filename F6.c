#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "GWO-C.h"

#define lb -100
#define ub 100

double function6(int j, int dim, double pos[]){
	double sumF=0;
	int i=0;
	for (i = 0; i < dim; i++){
        sumF = sumF + pow(pos[i] + 0.5, 2);
    }
    return sumF;
}

void getfObjtName(void){
	printf("Funcion N° 6\n");
}

int main(int argc, char const *argv[]){

	srand(time(0));

    double (*fObj)(int, int, double[]);
    getfObjtName();
    fObj=function6;

    GWO(lb, ub, fObj);
    showPack();

    return 0;
}