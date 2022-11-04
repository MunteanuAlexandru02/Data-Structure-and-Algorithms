#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#define uc unsigned char

/*De schimbat daca merge programul indicii 3 si 4*/
typedef struct QuadtreeNode {
    unsigned char blue, green, red ;
    uint32_t area ;
    int32_t top_left , top_right ;
    int32_t bottom_left, bottom_right ;
} __attribute__ (( packed )) QuadtreeNode;
/*Structura care va retine datele unui pixel*/
typedef struct RGB{
    unsigned char red,green,blue;
} __attribute__((packed)) RGB;
/*Structura pentru crearea unui arbore*/
typedef struct node{
    unsigned char red,green,blue; //codificarea pixelilor
    uint32_t area; //zona care este colorata
    struct node* child[4];//copiii unui nod
}node;
/*Structura in care vom retine radacina unui arbore, numarul culorilor
si numarul nodurilor*/
typedef struct tree{
    node *root;
    uint32_t no_nodes,no_colors;
}tree;

/*Functie care va calcula media aritmetica a unei zone din matricea "matrix".
Functia are ca argumente matricea in care sunt retinute codificarile pixelilor
coordonatele stanga sus si dreapta jos ale matricei. Functia returneaza un "nod"
care retine valorile medii din matrice*/
RGB* medie_arit(RGB *matrix[], int inc_h, int inc_w, int fin_h, int fin_w){
    RGB *newnode = malloc(sizeof(RGB));

    int i,j,imp = (fin_h-inc_h)*(fin_w-inc_w);
    unsigned long long int red_med, green_med, blue_med;

    red_med = green_med = blue_med = 0;

    for(i = inc_h; i < fin_h; i++)
        for(j = inc_w; j < fin_w; j++){  
            red_med += (unsigned long long int)matrix[i][j].red;
            green_med += (unsigned long long int)matrix[i][j].green;
            blue_med += (unsigned long long int)matrix[i][j].blue;
        }

    red_med /= imp;
    green_med /= imp;
    blue_med /= imp;
    newnode->red = red_med;
    newnode->green = green_med;
    newnode->blue = blue_med;

    return newnode;
}

/*Functia mean calculeaza scorul conform cerintei*/
unsigned long long int mean(RGB *matrix[],RGB *node, int inc_h, int inc_w, int fin_h, int fin_w){
    int i,j;
    unsigned long long int suma = 0;

    for(i = inc_h; i < fin_h; i++){
        for(j = inc_w; j < fin_w; j++){
            suma = suma + (node->red-matrix[i][j].red)*(node->red-matrix[i][j].red);
            suma = suma + (node->green-matrix[i][j].green)*(node->green-matrix[i][j].green);
            suma = suma + (node->blue-matrix[i][j].blue)*(node->blue-matrix[i][j].blue);
        }
    }

    suma /= 3*(fin_h-inc_h)*(fin_w-inc_w);
    
    return suma;
}
/*Functie care initializeaza un arbore*/
void init_tree(tree **t){
    (*t) = malloc(sizeof(tree));
    (*t)->root = NULL;
    (*t)->no_nodes = (*t)->no_colors = 0;
}
/*Functie care creeaza un nod*/
void create_node(node **newnode,uc red, uc green, uc blue, int inc, int fin){
    (*newnode) = malloc(sizeof(node));
    (*newnode)->red = red;
    (*newnode)->green = green;
    (*newnode)->blue = blue;
    (*newnode)->area = (fin-inc)*(fin-inc);
    for(int i = 0; i < 4; i++)
        (*newnode)->child[i] = NULL;
}
/*Functie care creeaza un arbore conform cerintei, folosindu-ne de functiile
medie si mean definite anterior, apoi vom imparti matricea in 4 zone, 
apelandu-le recursiv*/
void create_tree(node **nnode, int inc_h, int inc_w, int fin_h, int fin_w, RGB *matrix[], int prag,tree *t){
    node *de_adaugat;
    RGB *newnode;
    /*Folosesc newnode pentru a retine valoarea medie dintr-o matrice*/
    newnode = medie_arit(matrix,inc_h,inc_w,fin_h,fin_w);
    /*creez un nod "de_adaugat" pe care il voi insera in arbore*/
    create_node(&de_adaugat,newnode->red,newnode->green,newnode->blue,inc_h,fin_h);
    unsigned long long int scor = mean(matrix,newnode,inc_h,inc_w,fin_h,fin_w);
    
    free(newnode);
    
    if(*nnode == NULL && de_adaugat->area > 0){
        t->no_nodes++;
        (*nnode) = de_adaugat;
    }
    
    if(scor <= prag){
        t->no_colors++;
        return;
    }
    /*apelez recursiv functia pentru fiecare dintre submatrice*/
    else{
        create_tree(&(*nnode)->child[0],inc_h,inc_w,(inc_h+fin_h)/2,(inc_w+fin_w)/2,matrix,prag,t);
        create_tree(&(*nnode)->child[1],inc_h,(inc_w+fin_w)/2,(inc_h+fin_h)/2,fin_w,matrix,prag,t);
        create_tree(&(*nnode)->child[2],(inc_h+fin_h)/2,inc_w,fin_h,(inc_w+fin_w)/2,matrix,prag,t);
        create_tree(&(*nnode)->child[3],(inc_h+fin_h)/2,(inc_w+fin_w)/2,fin_h,fin_w,matrix,prag,t);
    }
    
    return;
}
/*Functie care are rolul de transformare a unui arbore intr-un vector.
M-am folosit de vectorul pos pentru a retine pozitiile la care ma voi deplasa
in vector*/
void create_vector(QuadtreeNode **vector, node *newnode,int poz, int *last){
    
    int pos[4] = {-1,-1,-1,-1};

    if(newnode == NULL)
        return;

    for(int i = 0; i < 4; i++){
        if(newnode->child[i] != NULL){
            //adaug nodul in vector
            (*vector)[*last].red = newnode->child[i]->red;
            (*vector)[*last].green = newnode->child[i]->green;
            (*vector)[*last].blue = newnode->child[i]->blue;
            (*vector)[*last].area = newnode->child[i]->area;
            //asociez descendentii
            if(i == 0)
                (*vector)[poz].top_left = (*last);
            if(i == 1)
                (*vector)[poz].top_right = (*last);
            if(i == 2)
                (*vector)[poz].bottom_left = (*last);
            if(i == 3)
                (*vector)[poz].bottom_right = (*last);

            pos[i] = (*last);
            (*last)++;
        }
        else{
            if(i == 0)
                (*vector)[poz].top_left = -1;
            if(i == 1)
                (*vector)[poz].top_right = -1;
            if(i == 2)
                (*vector)[poz].bottom_left = -1;
            if(i == 3)
                (*vector)[poz].bottom_right = -1;
        }
    }
    //apelez recursiv functia pentru fiecare descendent nenul
    for(int i = 0; i < 4; i++)
        if(pos[i] != -1)
            create_vector(&(*vector),newnode->child[i],pos[i],last);

}

/*Functie care construieste un arbore dintr-un vector*/
void convert_vect_tree(QuadtreeNode *vector, node **newnode, int i,int no_nodes){
    
    //adaug un element din vector in arbore
    if((*newnode) == NULL){
        create_node(&(*newnode),vector[i].red,vector[i].green,vector[i].blue,0,0);
        (*newnode)->area = vector[i].area;
    }
    //daca nodul este frunza
    if(vector[i].top_left == -1) return;
    
    convert_vect_tree(vector,&(*newnode)->child[0],vector[i].top_left,no_nodes);
    convert_vect_tree(vector,&(*newnode)->child[1],vector[i].top_right,no_nodes);
    convert_vect_tree(vector,&(*newnode)->child[2],vector[i].bottom_left,no_nodes);
    convert_vect_tree(vector,&(*newnode)->child[3],vector[i].bottom_right,no_nodes);
}
/*Functie care construieste o matrice dintr-un arbore*/
void create_matrix(RGB ***matrix,node *newnode, int inc_h, int inc_w, int fin_h, int fin_w){

    int ok = 1; //daca se poate afisa
    for(int i = 0; i < 4; i++)
        if(newnode->child[i] != NULL){
            ok = 0;
            break;
        }
    //daca pot afisa nodul, il voi adauga in matrice
    if(ok == 1){
        for(int i = inc_h; i < fin_h; i++)
            for(int j = inc_w; j < fin_w; j++){
                (*matrix)[i][j].red = newnode->red;
                (*matrix)[i][j].green = newnode->green;
                (*matrix)[i][j].blue = newnode->blue;
            }
    }
    //apelez recursiv fucntia pentru cele 4 submatrice
    else{
        create_matrix(&(*matrix),newnode->child[0],inc_h,inc_w,(inc_h+fin_h)/2,(inc_w+fin_w)/2);
        create_matrix(&(*matrix),newnode->child[1],inc_h,(inc_w+fin_w)/2,(inc_h+fin_h)/2,fin_w);
        create_matrix(&(*matrix),newnode->child[2],(inc_h+fin_h)/2,inc_w,fin_h,(inc_w+fin_w)/2);
        create_matrix(&(*matrix),newnode->child[3],(inc_h+fin_h)/2,(inc_w+fin_w)/2,fin_h,fin_w);
    }

}
/*Functie care realizeaza rotirea pe orizontala
Functia realizeaza o simpla interschimbare intre noduri, conform cerintei*/
void rotate_h(node **newnode){
    int ok = 1;
    for(int i = 0; i < 4; i++)
        if((*newnode)->child[i] == NULL)
            ok = -1; // e frunza
    
    if(ok == -1)
        return;
    node *temp = (*newnode)->child[0];
    (*newnode)->child[0] = (*newnode)->child[1];
    (*newnode)->child[1] = temp;

    temp = (*newnode)->child[2];
    (*newnode)->child[2] = (*newnode)->child[3];
    (*newnode)->child[3] = temp;

    //apelare recursiva a functiei pentru fiecare fiu
    for(int i = 0; i < 4; i++)
        rotate_h(&(*newnode)->child[i]);

}
/*Functie care realizeaza rotirea pe verticala
Functia realizeaza interschimbare intre noduri conform cerintei*/
void rotate_v(node **newnode){
    int ok = 1;
    for(int i = 0; i < 4; i++)
        if((*newnode)->child[i] == NULL)
            ok = -1; // e frunza
    
    if(ok == -1)
        return;
    node *temp = (*newnode)->child[0];
    (*newnode)->child[0] = (*newnode)->child[2];
    (*newnode)->child[2] = temp;

    temp = (*newnode)->child[1];
    (*newnode)->child[1] = (*newnode)->child[3];
    (*newnode)->child[3] = temp;
    //apelare recursiva pentru fiecare fiu
    for(int i = 0; i < 4; i++)
        rotate_v(&(*newnode)->child[i]);
}
/*functie care elibereaza memoria alocata unui arbore*/
void free_tree(node **newnode){
    if((*newnode) == NULL)
        return;
    free_tree(&(*newnode)->child[0]);
    free_tree(&(*newnode)->child[1]);
    free_tree(&(*newnode)->child[2]);
    free_tree(&(*newnode)->child[3]);
    free(*newnode);
}

int main(int argc, char *argv[]){
    
    char c,junk;
    int type, i, prag;
    int width, height, max_value;
    tree *t; // arbore pentru rezolvarea cerintelor 2 si 3
    init_tree(&t); 
    
    if(strcmp(argv[1],"-c") == 0){
        prag = atoi(argv[2]);

        FILE *f = fopen(argv[3],"rb+");
        FILE *g = fopen(argv[4],"wb+");

        fscanf(f,"%c%d",&c,&type);
        fscanf(f,"%d %d",&width,&height);
        fscanf(f,"%d",&max_value);
        fscanf(f,"%c",&junk);

        RGB **matrix = malloc(height*sizeof(RGB*));
        for(i = 0; i < height; i++)
            matrix[i] = malloc(width*sizeof(RGB));

        for(i = 0; i < height; i++)
            fread(matrix[i],3,width,f); 

        create_tree(&t->root,0,0,height,width,matrix,prag,t);
        QuadtreeNode *vector = malloc(2*t->no_nodes*sizeof(QuadtreeNode));
        
        int last = 0;
        vector[0].red = t->root->red;
        vector[0].green = t->root->green;
        vector[0].blue = t->root->blue;
        vector[0].area = t->root->area;
        last = 1;
        create_vector(&vector,t->root,0,&last);
         /*scrierea in fisierul binar a vectorului de compresie*/
        fwrite(&t->no_colors,sizeof(int),1,g);
        fwrite(&t->no_nodes,sizeof(int),1,g);
        for(i = 0; i < t->no_nodes; i++){
            fwrite(&vector[i],sizeof(QuadtreeNode),1,g);
        }
        fclose(f);
        fclose(g);
        free(vector);
        for(int i = 0; i < height; i++)
            free(matrix[i]);
        free(matrix);
        free_tree(&t->root);
        free(t);
    }
    else if(strcmp(argv[1],"-d") == 0){
        FILE *f = fopen(argv[2],"rb");
        FILE *g = fopen(argv[3],"wb");
        int no_colors,no_nodes;
       
        fread(&no_colors,sizeof(int),1,f);
        fread(&no_nodes,sizeof(int),1,f);
        QuadtreeNode *vector = malloc(no_nodes*sizeof(QuadtreeNode));
        
        fread(vector,sizeof(QuadtreeNode),no_nodes,f);
        
        
        t->no_nodes = no_nodes;
        t->no_colors = no_colors;
        convert_vect_tree(vector,&t->root,0,no_nodes);
        
        height = width = sqrt(t->root->area);
        RGB **matrix = malloc(height*sizeof(RGB*));
        for(i = 0; i < height; i++)
            matrix[i] = malloc(width*sizeof(RGB));

        create_matrix(&matrix,t->root,0,0,height,width);
        
        
        fprintf(g,"P6\n");
        fprintf(g,"%d %d\n",width,height);
        fprintf(g,"%d",255);
        fprintf(g,"\n");
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
                fwrite(&matrix[i][j],3,1,g);
        
        fclose(f);
        fclose(g);
        free(vector);
        for(int i = 0; i < height; i++)
            free(matrix[i]);
        free(matrix);
        free_tree(&t->root);
        free(t);
    }
    else if(strcmp(argv[1],"-m") == 0){
        //prima data citim matricea si construim arborele
        prag = atoi(argv[3]);
        FILE *f = fopen(argv[4],"rb+");
        FILE *g = fopen(argv[5],"wb+");
        fscanf(f,"%c%d",&c,&type);
        fscanf(f,"%d %d",&width,&height);
        fscanf(f,"%d",&max_value);
        fscanf(f,"%c",&junk);
        RGB **matrix = malloc(height*sizeof(RGB*));
        for(i = 0; i < height; i++)
            matrix[i] = malloc(width*sizeof(RGB));
        for(i = 0; i < height; i++)
            fread(matrix[i],3,width,f); 
        create_tree(&t->root,0,0,height,width,matrix,prag,t);
        //lucram cu arborele
        if(strcmp(argv[2],"h") == 0){
            rotate_h(&t->root);
        }
        else if(strcmp(argv[2],"v") == 0){
            rotate_v(&t->root);
        }
        create_matrix(&matrix,t->root,0,0,height,width);

        fprintf(g,"P6\n");
        fprintf(g,"%d %d\n",width,height);
        fprintf(g,"%d",255);
        fprintf(g,"\n");
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
                fwrite(&matrix[i][j],3,1,g);
        
        fclose(f);
        fclose(g);
        for(int i = 0; i < height; i++)
            free(matrix[i]);
        free(matrix);
        free_tree(&t->root);
        free(t);
    }
}