#include <stdio.h>
#include <stdlib.h>
#include "graf.h"

#define inf 214748364

void printPath(int *path, int start,int *vector, int *cnt){
    if(path[start] == -1)
        return;
    printPath(path,path[start],vector,&(*cnt));
    //printf("%d ",start); 
    vector[(*cnt)] = start;
    (*cnt)++;
}
//value costul intre un depozit si un magazin
void Djikstra(graf *g,int x,int y,float *value,int *vector,int *cnt){
    double *cost = malloc(g->n*sizeof(double));
    int *path = malloc(g->n*sizeof(int));
    int *viz = calloc(g->n,sizeof(int));
    int min;
    double min_value;
    
    for(int i = 0; i < g->n; i++)
        cost[i] = inf;
    
    cost[x] = 0;
    for(int i = 0; i < g->n; i++)
        path[i] = -1;

    for(int i = 0; i < g->n; i++){
        min = 0;
        min_value = inf;
        for(int j = 0; j < g->n; j++)
            if(viz[j] == 0 && cost[j] < min_value)
                {min = j;min_value = cost[j];}

        viz[min] = 1;

        for(int j = 0; j < g->n; j++)
            if(g->a[min][j] != inf)
                if(cost[min] + g->a[min][j] < cost[j]){
                    cost[j] = g->a[min][j] + cost[min];
                    path[j] = min;
                }
    }
    (*value) = cost[y];
    printPath(path,y,vector,&(*cnt));
    free(viz);
    free(cost);
    free(path);
}
