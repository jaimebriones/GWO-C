#include <math.h>

double powF (double x){
	return x * x;
}

double function6(int j, int dim, double pos[]){
	double sumF=0;
	int i=0;
	for (i = 0; i < dim; i++){
        sumF = sumF + powF(pos[i] + 0.5);
    }
    return sumF;
}

void getfObjtName(void){
	printf("Funcion N° 6\n");
}
