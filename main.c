#include <stdio.h>
#include "F6.h"
#include "GWO-C.h"

int main(int argc, char const *argv[]){

	srand(time(0));

    double (*fObj)(int, int, double[]);
    getfObjtName();
    fObj=function6;

    GWO(fObj);
    showPack();

    return 0;
}