#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "RngStream.h"
#include "F6.h"
 
#define SEARCH_AGENTS 30
#define dim 30
#define lb -100
#define ub 100
#define sInf 3425666547544
#define MI 500
#define PI 3.1415926535897932384
#define NS 1000000

typedef struct {
    double pos[dim];
    double fitness;
} Wolf;

Wolf wolf_pack[SEARCH_AGENTS], alpha, beta, delta;


void verLU(int index){
    int i = 0;

    for (i = 0; i < dim; ++i){
        if (wolf_pack[index].pos[i] > ub || wolf_pack[index].pos[i] < lb){
            wolf_pack[index].pos[i]= (alpha.pos[i]+beta.pos[i]+delta.pos[i])/3;
         } 
    }
}

void showPack(){
    int j = 0;
    for (j = 0; j < dim; ++j){
        //printf(" %f ", alpha.pos[j]);        
    }
    //printf("\n");

    printf("Fitness de alpha:\t %.60f\n", alpha.fitness);
}

void initializeAgents(RngStream g1){
    alpha.fitness=sInf;
    beta.fitness=sInf;
    delta.fitness=sInf;
    int i = 0;

    for(i=0; i<SEARCH_AGENTS;i++){
        int j = 0;
        for (j = 0; j < dim; j++){
            wolf_pack[i].pos[j]=RngStream_RandU01(g1)*(ub-lb)+lb;
        }
    }
}

/* double function(int i, int j){
    return wolf_pack[i].pos[j] * wolf_pack[i].pos[j];
}

double function2(){
    double fitness=0;

    for (int i = 0; i < SEARCH_AGENTS; ++i){
        for(int k=0; k<dim; k++){
            fitness = fitness + fabs(wolf_pack[i].pos[k]);              
        }
    }

    double mult=1;

    for (int i = 0; i < SEARCH_AGENTS; ++i){
        for (int m = 0; m < dim;m++){
            mult = mult*fabs(wolf_pack[i].pos[m]);
        }
    }

    return fitness + mult;
}

double function3(int j, int i){
    double sum=0;
    for (int z = 0; z < i; ++z){
        sum = sum + wolf_pack[j].pos[z];
    }

    return (sum * sum);
}

double function7(int j, int i){
    return i * pow(wolf_pack[j].pos[i],4);
}

double function8(int j, int i){
    return wolf_pack[i].pos[j]*sin(sqrt(fabs(wolf_pack[i].pos[j])));
}

double function16(int i){
    double sum1=0, sum2=0, sum3=0;
    for(int j=0; j<dim; j++){
        sum1=sum1+pow(sin(wolf_pack[i].pos[j]),2);
        sum2=sum2+pow(wolf_pack[i].pos[j],2);
        sum3=sum3+sin2(sqrt(fabs(wolf_pack[i].pos[j])));
    }

    return (sum1-exp(-sum2)) * exp(-sum3);
}

double function19(int i){
    double sum=0;
    for (int j = 0; j < dim; ++j){
        sum = sum + aH[i][j]*pow(wolf_pack[i].pos[j]-pH[i][j],2);
    }
    return exp(-sum);
}

double function20(int i){
    double sum=0;
    for (int j = 0; j < dim; ++j){
        sum = sum + aH1[i][j]*pow(wolf_pack[i].pos[j]-pH1[i][j],2);
    }
    return exp(-sum);
} */

double GWO(double (*fObj)(int j2, int dim2, double pos2[])){

    unsigned long germe[6] = { rand(), rand(), rand(), rand(), rand(), rand() };
    RngStream_SetPackageSeed (germe);
    RngStream g1 = RngStream_CreateStream ("Laplace");

    initializeAgents(g1);

    int l=0;
    while(l<MI){
        int i = 0;
        for(i=0; i<SEARCH_AGENTS;i++){
            double sumF = 0;

            verLU(i);

            wolf_pack[i].fitness = fObj(i, dim, wolf_pack[i].pos);

            if (wolf_pack[i].fitness < alpha.fitness) {
                memcpy(delta.pos, beta.pos, sizeof(beta.pos));
                delta.fitness = beta.fitness;

                memcpy(beta.pos, alpha.pos, sizeof(alpha.pos));
                beta.fitness = alpha.fitness;

                memcpy(alpha.pos, wolf_pack[i].pos, sizeof(wolf_pack[i].pos));
                alpha.fitness=wolf_pack[i].fitness;
            }

            if (wolf_pack[i].fitness < beta.fitness && wolf_pack[i].fitness > alpha.fitness){
                memcpy(delta.pos, beta.pos, sizeof(delta.pos));
                delta.fitness = beta.fitness;

                memcpy(beta.pos, wolf_pack[i].pos, sizeof(wolf_pack[i].pos)); 
                beta.fitness=wolf_pack[i].fitness;   
            }

            if (wolf_pack[i].fitness > alpha.fitness && wolf_pack[i].fitness > beta.fitness && wolf_pack[i].fitness < delta.fitness){
                memcpy(delta.pos, wolf_pack[i].pos, sizeof(wolf_pack[i].pos));        
                delta.fitness=wolf_pack[i].fitness;
            }
        }
        
        double a=2-l*((2)/MI);

        for(i=0; i<SEARCH_AGENTS;i++){

            double A1[dim],A2[dim],A3[dim],D_alpha[dim],D_beta[dim],D_delta[dim],C1[dim],C2[dim],C3[dim],X1[dim],X2[dim],X3[dim];

            int z = 0;
            for (z = 0; z < dim; ++z){

                A1[z]=2*a*RngStream_RandU01(g1)-a;
                C1[z]=2*RngStream_RandU01(g1);

                A2[z]=2*a*RngStream_RandU01(g1)-a;
                C2[z]=2*RngStream_RandU01(g1);

                A3[z]=2*a*RngStream_RandU01(g1)-a;
                C3[z]=2*RngStream_RandU01(g1);
                
                D_alpha[z]=fabs(C1[z]*alpha.pos[z]-wolf_pack[i].pos[z]);
                D_beta[z]=fabs(C2[z]*beta.pos[z]-wolf_pack[i].pos[z]);
                D_delta[z]=fabs(C3[z]*delta.pos[z]-wolf_pack[i].pos[z]);
                
                X1[z]=alpha.pos[i]-A1[z]*D_alpha[z];
                X2[z]=beta.pos[i]-A2[z]*D_beta[z];
                X3[z]=delta.pos[i]-A3[z]*D_delta[z];

                wolf_pack[i].pos[z]=(X1[z]+X2[z]+X3[z])/3;
            }
        }
        l++;
    }
    RngStream_DeleteStream (&g1); 
}
 
/*int main (int argc, char const *argv[]){

    srand(time(0));
    unsigned long germe[6] = { rand(), rand(), rand(), rand(), rand(), rand() };
    RngStream_SetPackageSeed (germe);
    g1 = RngStream_CreateStream ("Laplace");

    double (*fObj)(int, int, double[]);
    fObj=function6;
    GWO(fObj);
    showPack();

    RngStream_DeleteStream (&g1); 
    return 0;
}*/