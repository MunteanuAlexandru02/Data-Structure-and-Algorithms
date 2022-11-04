/**
 * @file graf.h
 * !contine functii utile pentru implementarea unui graf folosind matrice de 
 * !adiacenta
 */
#include <stdio.h>
#include <stdlib.h>

#define inf 214748364

typedef struct graf{
    int n; //nr de noduri si numarul de muchii
    //lista de adiacenta care va retine costul muchiei daca aceasta exista,
    //altfel va retine inf
    double **a; 
}graf;

void init_graf(graf **g, int n){
    (*g) = malloc(sizeof(graf));
    (*g)->n = n;
    (*g)->a = malloc(n*sizeof(double*));
    for(int i = 0; i < n; i++)
        (*g)->a[i] = malloc(n*sizeof(double));

    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            (*g)->a[i][j] = inf;
}

void free_graf(graf **g){
    for(int i = 0; i < (*g)->n; i++)
        free((*g)->a[i]);
    free((*g)->a);
    free(*g);
}

void afis_matrix(graf *g, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++)  
            printf("%.1lf ",g->a[i][j]);
        printf("\n");
    }
}
