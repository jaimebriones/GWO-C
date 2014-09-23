#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "RngStream.h"
 
#define SEARCH_AGENTS 30
#define dim 30
#define sInf 3425666547544
#define MI 500
#define PI 3.1415926535897932384
#define NS 1000000

typedef struct {
    double pos[dim];
    double fitness;
} Wolf;

Wolf wolf_pack[SEARCH_AGENTS], alpha, beta, delta;


void verLU(int index, int lb, int ub){
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

void initializeAgents(RngStream g1, int lb, int ub){
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

void GWO(int lb, int ub, double (*fObj)(int j2, int dim2, double pos2[]) ){

    unsigned long germe[6] = { rand(), rand(), rand(), rand(), rand(), rand() };
    RngStream_SetPackageSeed (germe);
    RngStream g1 = RngStream_CreateStream ("Laplace");

    initializeAgents(g1, lb, ub);

    int l=0;
    while(l<MI){
        int i = 0;
        for(i=0; i<SEARCH_AGENTS;i++){

            verLU(i, lb, ub);

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
