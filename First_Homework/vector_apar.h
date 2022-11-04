#include <stdio.h>
#include <stdlib.h>

typedef struct apar{
    int val;
    struct apar *next;
    struct apar *prev;
}apar;

typedef struct lista_apar{
    apar *head;
    apar *tail;
    int len;
}lista_apar;
/*Fisierul contine functii necesare construirii unui vector de aparitii*/
void init_apar(lista_apar **list){
    (*list) = malloc(sizeof(lista_apar));
    (*list)->head = (*list)->tail = NULL;
    (*list)->len = 0;
    return;
}

void push_apar(lista_apar *list, int val){
    apar *newnode = malloc(sizeof(apar));
    newnode->next = newnode->prev = NULL;
    newnode->val = val;

    if(list->head == NULL)
        list->head = list->tail = newnode;
    else{
        list->tail->next = newnode;
        newnode->prev = list->tail;
        newnode->next = NULL;
        list->tail = newnode;
    }
    list->len++;

    return;
}
void replace_value(lista_apar *list, int poz, int value){
    apar *newnode = list->head;

    for(int i = 1; i < poz; i++)
        newnode = newnode->next;

    newnode->val = value;
}
void eliminare(lista_apar *list, int k){
    apar *newnode;
    newnode = list->head;

    for(int i = 1; i <= k-1; i++)
        newnode = newnode->next;
    
    newnode->prev->next = newnode->next;
    newnode->next->prev = newnode->prev;
    newnode->next = newnode->prev = NULL;

    list->len--;

    free(newnode);
}

void print_apar(lista_apar *list){
    apar *newnode = list->head;

    while(newnode !=  NULL){
        printf("%d ",newnode->val);
        newnode = newnode->next;
    }
}

void free_apar(lista_apar **list){
    apar *newnode;

    while((*list)->head != NULL){
        newnode = (*list)->head;
        (*list)->head = (*list)->head->next;
        free(newnode);
    }

    free(*list);
}