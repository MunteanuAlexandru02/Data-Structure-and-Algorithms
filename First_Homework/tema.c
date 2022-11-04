#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//fisier header care contine functii utile pentru simularea unui vector de aparitii
#include "vector_apar.h"
//functii utile pentru rezolvarea punctului 2.5
#include "st.h"
//functie pentru rezolvarea 2.4
#include "c.h"
//definirea unui nod
typedef struct node{
    int timestamp;
    double value;
    struct node *next;
    struct node *prev;
}node;
//definirea unei liste dublu inlantuite
typedef struct TDoubleLL{
    node *head;
    node *tail;
    int len;
}TDoubleLL;
//initializarea listei dublu inlantuite
void init(TDoubleLL **list){
    (*list) = malloc(sizeof(TDoubleLL));
    (*list)->head = (*list)->tail = NULL;
    (*list)->len = 0;
    return;
}
/*Functie care adauga un nod la finalul listei*/
void add(TDoubleLL *list, int time, double x){
    //declaram un nod pe care o sa-l inseram in lista
    node *newnode = (struct node*) malloc(sizeof(node));
    newnode->next = newnode->prev = NULL;
    newnode->timestamp = time;
    newnode->value = x;

    if(list->head == NULL) // primul element
        list->head = list->tail = newnode;
    
    else{ //inseram la finalul listei
        list->tail->next = newnode;
        newnode->prev = list->tail;
        newnode->next = NULL;
        list->tail = newnode;
    }

    return;
}
//functie pentru afisarea listei
void print(TDoubleLL *list){
    node *newnode = list->head;

    printf("%d\n",list->len);

    while(newnode != NULL){
        printf("%d %0.2f\n",newnode->timestamp,newnode->value);
        newnode = newnode->next;
    }
}

//Functie care calculeaza media aritmetica dintr-un interval inchis [inc,fin]
double medie(TDoubleLL *list, int inc, int fin){
    node *newnode = list->head;
    int i;
    double med = 0;

    for(i = 1; i < inc; i++)
        newnode = newnode->next;
    for(; i < fin; i++){
        med +=newnode->value;
        newnode = newnode->next;
    }
    return (med/(fin-inc));
}
//Functie care calculeaza valoarea tau conform cerintei
double tau(TDoubleLL *list, int inc, int fin){
    double media = medie(list,inc,fin);
    double rez = 0;
    int i = 0;
    node *newnode = list->head;

    for(i = 1; i < inc; i++)
        newnode = newnode->next;
    
    for(; i<fin; i++){
        rez += (newnode->value-media)*(newnode->value-media);
        newnode = newnode->next;
    }

    rez /= 5;
    rez = sqrt(rez);

    return rez;
}
//Functie care realizeaza eliminarea unui nod de pe pozitia k din lista list
void elim(TDoubleLL *list, int k){
    node *newnode;
    newnode = list->head;

    for(int i = 1; i <= k-1; i++)
        newnode = newnode->next;
    
    newnode->prev->next = newnode->next;
    newnode->next->prev = newnode->prev;
    newnode->next = newnode->prev = NULL;

    list->len--;

    free(newnode);
}
//Functie care verifica daca un numar este "ok" sau "not ok" confor cerintei
//de la 2.1
int verificare(TDoubleLL *list, int inc, int fin, double med, double t){
    int poz = (inc+fin-1)/2;
    node *newnode = list->head;
    double capat_inf = med - t;
    double capat_sup = med + t;
    
    for(int i = 1; i < poz; i++)
        newnode = newnode->next;
    //printf("%f %f %d %f\n",med,t,newnode->timestamp,newnode->value);
    if(newnode->value >= capat_inf && newnode->value <= capat_sup)
        return 1;
    else
       return 0;
}
//Functie care realizeaza eliberarea memoriei alocate pentru o lista
void free_list(TDoubleLL **list){
    node *newnode;

    while((*list)->head != NULL){
        newnode = (*list)->head;
        (*list)->head = (*list)->head->next;
        free(newnode);
    }
    
    free(*list);
}
//Functie de eliberare a memoriei alocate unei liste, cu exceptia lui head/tail
void free2(TDoubleLL **list){
    node *newnode;

    while((*list)->head != NULL){
        newnode = (*list)->head;
        (*list)->head = (*list)->head->next;
        free(newnode);
    }
}
//Functie care copiaza datele din aux in list
void overwrite(TDoubleLL *list, TDoubleLL *aux){
    free2(&list);
    list->head = list->tail = NULL;
    node *newnode = aux->head;

    while(newnode != NULL){
        add(list,newnode->timestamp,newnode->value);
        newnode = newnode->next;
    }

    list->len = aux->len;
}
//Functie care realizeaza sortarea intervalelor pentru realizarea cerintei 
//de la 2.5
double sortare(TDoubleLL *list, int inc, int fin){
    TDoubleLL *aux;
    init(&aux);
    int i, cp_time;
    double cp_value;
    node *newnode = list->head;
    node *p;
    for(i = 1; i <= inc-1; i++)
        newnode = newnode->next;
    //construim fectorul aux, pornind de la pozitia inc si ajungand la fin
    for(; i <= fin; i++){
        cp_time = newnode->timestamp;
        cp_value = newnode->value;
        add(aux,cp_time,cp_value);
        newnode = newnode->next;
    }
    newnode = aux->head;
    while(newnode != NULL){
        p = newnode->next;
        while(p != NULL){
            if(newnode->value - p->value > 0){
                double copie = newnode->value;
                newnode->value = p->value;
                p->value = copie;
            }
            p = p->next;
        }
        newnode = newnode->next;
    }
    
    newnode = aux->head;
    newnode = newnode->next->next;
    double rez = newnode->value;
    //pentru a putea elibera memoria alocata listei aux, com folosi o variabila
    //auxiliara "rez", care va retine valoarea ce urmeaza a fi returnata
    free_list(&aux);
    return rez;
}
//Functie pentru determinarea unui maxim dintr-o lista
/*Vom folosi ceil pentru a aproxima la cel mai mare numar, astfel ca,
30.4 va fi aproximat la 31, nu la 30, pentru ca valoarea 30.4 sa fie inclusa
intr-un interval*/
int maxim_lista(TDoubleLL *list){
    node *newnode = list->head->next;
    int maxim = (int)ceil(list->head->value);

    while(newnode != NULL){
        if((int)ceil(newnode->value) > maxim)
            maxim = (int)ceil(newnode->value);
        newnode = newnode->next;
    }

    return maxim;
}
//Functie pentru determinarea elementului minim dintr-o lista
/*Analog functiei maxim_lista, vom folosi floor pentru a aproxima la valoarea
mai mica*/
int minim_lista(TDoubleLL *list){
    node *newnode = list->head->next;
    int minim = (int)floor(list->head->value);

    while(newnode != NULL){
        if((int)floor(newnode->value) < minim)
            minim = (int)floor(newnode->value);
        newnode = newnode->next;
    }

    return minim;
}
//Functie care adauga elemente in listele left si right pentru realizarea
//cerintei de la 2.4
void adauga(TDoubleLL *left, TDoubleLL *right, TDoubleLL *list, int contor){
    int pornire = contor-2;
    node *newnode = list->head;
    for(int i = 1; i < pornire; i++)
        newnode = newnode->next;
    for(int i = 1; i <= 3; i++){
        add(left,newnode->timestamp,newnode->value);
        newnode = newnode->next;
    }
    for(int i = 1; i <= 3; i++){
        add(right,newnode->timestamp,newnode->value);
        newnode = newnode->next;
    }
}
//Functie care realizeaza schimbarea valorilor din listele left si right
void replace(TDoubleLL *left, TDoubleLL *right, TDoubleLL *list, int contor){
    int pornire = contor-2;
    node *newnode = list->head;

    for(int i = 1; i < pornire; i++)
        newnode = newnode->next;
    
    node *p=left->head;

    for(int i = 1; i <= 3; i++){
        p->timestamp=newnode->timestamp;
        p->value = newnode->value;
        newnode = newnode->next;
        p = p->next;
    }

    p=right->head;

    for(int i = 1; i <= 3; i++){
        p->timestamp=newnode->timestamp;
        p->value = newnode->value;
        newnode = newnode->next;
        p=p->next;
    }
}
//Functie care adauga un element in list la o anumita pozitie
void add_at_index(TDoubleLL *list, int t, double v,int n){
    node *newnode = (struct node*) malloc(sizeof(node));
    newnode->next = newnode->prev = NULL;
    newnode->timestamp = t;
    newnode->value = v;
    node *p = list->head;

    for(int i = 1; i <= n-1; ++i)
        p = p->next;

    newnode->next = p->next;
    p->next->prev = newnode;
    newnode->prev = p;
    p->next = newnode;

    return;
}

int main(int argc, char *argv[]){
    TDoubleLL *list;
    TDoubleLL *c;
    lista_apar *ver;
    int time,k,valoare,ok = 1;
    double x;
    node *newnode;
    init(&list); //initializam lista
    init_apar(&ver); //initializam o lista de aparitii

    scanf("%d",&list->len);

    for(int i = 1; i <= list->len; i++){
        scanf("%d%lf",&time,&x);
        add(list,time,x);
    }
    double med, t;

    k=5;
    //Parcurgem fiecare argument si cautam un argument specific unei cerinte
for(int q = 1; q < argc; q++){
    if(argv[q][2] == 'e' && argv[q][3] == '1'){
        for(int i = 1; i <= list->len; i++)
            if(i == 1 || i == 2 || i == list->len || i ==list->len-1)
                push_apar(ver,1); //primele doua si ultimele doua valori nu vor fi modificate
            else
                push_apar(ver,0);
    
        for(int i = 1; i <= list->len-k+1; i++){
            med = medie(list,i,i+k);
            t = tau(list,i,i+k);
            valoare = verificare(list,i,i+k,med,t);
            replace_value(ver,i+2,valoare); 
        }
    
        apar *p;
    
        for(int i = 1; i <= list->len; i++){
            p = ver->head;
            for(int j = 1; j <= i; j++){
                if(p->val == 0){
                    elim(list,i);
                    eliminare(ver,i);
                    i--;
                    break;
                
                }
                p = p->next;
            }
       }
    }
    else if(argv[q][2] == 'e' && argv[q][3] == '2'){
        double cp_val;
        init(&c); //initializam o lista auxiliara
        for(int i = 1; i <= list->len-4; i++){
            newnode = list->head;
            for(int j = 1; j < i; j++)
                newnode = newnode->next;
            newnode = newnode->next->next;
            cp_val = sortare(list,i,i+4);
            add(c,newnode->timestamp,cp_val);
            c->len++;
        }
        overwrite(list,c); // list primeste toate datele din lista c
        free_list(&c);
    }
    else if(argv[q][2] == 'e' && argv[q][3] == '3'){
        init(&c); //lista auxiliara
        for(int i = 1; i <= list->len-4; i++){
            newnode = list->head;
            for(int j = 1; j < i; j++)
                newnode = newnode->next;
            newnode = newnode->next->next;
            med = medie(list,i,i+5);
            add(c,newnode->timestamp,med);
            c->len++;
        }
        overwrite(list, c);
        free_list(&c);
    }
    else if(argv[q][2] == 'u'){
        newnode = list->head->next;
        while(newnode != NULL){
            int de_comparat = fabs(newnode->timestamp-newnode->prev->timestamp); //prima data am folosit double
            if(de_comparat >= 100 && de_comparat <= 1000){
                //noul timestamp si noua valoare
                newnode->timestamp = (newnode->prev->timestamp+newnode->timestamp)/2; 
                newnode->value = (newnode->prev->value+newnode->value)/2;
            }
            newnode = newnode->next;
        }
    }
    else if(argv[q][2] == 's'){
        ok = 0; // nu mai trebuie sa afisam lista
        int lambda =(int) atoi((char*)(argv[q]+4)); //diferenta dintre un capat superior si un capat inferior
        int maxim = maxim_lista(list);
        int minim = minim_lista(list);
        LL *intervals, *contors;
        init_interval(&intervals);//lista in care se vor salva capetele intervalelor
        init_interval(&contors);//lista de aparitii
        add_interval(intervals,minim);
        //construirea intervalelor
        while(minim <= maxim){
            minim += lambda;
            add_interval(intervals,minim); // construirea intervalelor
        }
        int cnt;
        intervale *newnode = intervals->head;
        while(newnode->next != NULL){
            node *p = list->head;
            cnt = 0;
            for(int i = 1; i <= list->len; i++)
                {
                if(p->value-(double)newnode->value >= 0 && p->value-(double)newnode->next->value<=0)
                    cnt++;
                p=p->next;
                }
            add_interval(contors,cnt);
            newnode=newnode->next;
        }
        newnode = intervals->head;
        intervale *c = contors->head;
        while(newnode->next!=NULL){ //afisarea intervalelor si a valorilor
            if(c->value!=0)
                printf("[%d, %d] %d\n",newnode->value,newnode->next->value,c->value);
            newnode=newnode->next;
            c=c->next;
        }
        free_intervale(&contors);
        free_intervale(&intervals);
    }
    else if(argv[q][2] == 'c'){
        TDoubleLL *left,*right;
        newnode = list->head;
        init(&left);
        init(&right);
        int contor=1,poz = 1;
        while(newnode->next != NULL){
            int de_comparat = fabs(newnode->next->timestamp-newnode->timestamp); //prima data am folosit double
            if(de_comparat>1000){
                node *p;
                if(left->len == 0){
                    adauga(left,right,list,contor);
                    right->len = left->len = 3;
                }
                else
                    replace(left,right,list,contor);
                int new_timestamp = left->head->next->next->timestamp+200;
                int capat_timestamp = left->head->next->next->timestamp;
                double C,new_value,sum_right=0,sum_left=0;
                int cnt=0;
                //adaugam elementele conform cerintei
                while(new_timestamp < right->head->timestamp){
                    sum_left = sum_right = 0;
                    C = (new_timestamp - capat_timestamp)/((right->head->timestamp-capat_timestamp)*1.0);
                    //printf("\n\n%f\n\n",C);
                    //construirea noilor valori
                    p = left->head;
                    for(int i = 1; i <= 3; i++){
                        sum_left += p->value*omega(i-1,3);
                        p = p->next;
                    }
                    sum_left = (1-C)*sum_left;
                    p = right->tail;
                    for(int i = 1; i <= 3; i++){
                        sum_right += p->value*omega(i-1,3);
                        p = p->prev;
                    }
                    sum_right = C*sum_right;
                    new_value = sum_left + sum_right;
                    //adaugarea unui element in lista
                    add_at_index(list,new_timestamp,new_value,poz);
                    list->len++; //incrementarea lungimei listei
                    poz++;
                    cnt++;
                    new_timestamp +=200;
                }
                
                poz-=cnt;
            }
            contor++;
            poz++; //poz = unde va fi inserat urmatorul element in lista
            newnode = newnode->next;
        }
        free_list(&left);
        free_list(&right);
    }
}
    //afisam lista
    if(ok == 1)
        print(list);
    free_apar(&ver);
    free_list(&list);
}