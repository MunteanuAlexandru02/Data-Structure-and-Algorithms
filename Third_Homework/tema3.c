#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stack.h"
#include "Djikstra.h"
//#include "solveE3.h"

#define inf 214748364
void dfs(graf *g, int *visited, int nod, Stack **s,int **matrix_ctc, int linie){
    visited[nod] = 1;
    matrix_ctc[linie][0]++;
    matrix_ctc[linie][matrix_ctc[linie][0]] = nod;
    for(int i = 0; i < g->n; i++)
        if(g->a[nod][i] != inf && visited[i] == 0)
            dfs(g,visited,i,&(*s),matrix_ctc,linie);

    add_stack((*s),nod);
}

void apel_dfs(int *visited, graf *g, Stack **s,int **matrix_ctc,int index){
    for(int i = 0; i < g->n; i++)
        visited[i] = 0;
    for(int i = 0; i < g->n; i++)
        if(visited[i] == 0)
            dfs(g,visited,i,&(*s),matrix_ctc,index);
}

void swap(int *x,int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

void sortare_matrix(int **matrix_ctc,int ctc){
    for(int i = 0; i < ctc; i++)
        for(int j = 1; j < matrix_ctc[i][0] ; j++)
            for(int k = j+1; k <= matrix_ctc[i][0]; k++)
                if(matrix_ctc[i][j] > matrix_ctc[i][k])
                    swap(&matrix_ctc[i][j],&matrix_ctc[i][k]);
            
}

int apartine_depozite(int x, int d, int *depo){
    for(int i = 0; i < d; i++)
        if(x == depo[i])
            return 1;
    return 0;
}

void print_matrix(int **matrix_ctc, int ctc,int d, int *depozite){
    int min,linie,linii_afisate = 0;
    int *visited = calloc(ctc,sizeof(int));
    for(int i = 0; i < ctc-d; i++){
        min = inf;
        for(int j = 0; j < ctc; j++)
            if(min > matrix_ctc[j][1] && visited[j] == 0)
                if(apartine_depozite(matrix_ctc[j][1],d,depozite) == 0)
                {
                    min = matrix_ctc[j][1];
                    linie = j;
                }
        if(min != inf)
            linii_afisate++;
        for(int j = 1; j <= matrix_ctc[linie][0]-1; j++)
            printf("%d ",matrix_ctc[linie][j]);
        if(linii_afisate != ctc - d)
            printf("%d\n",matrix_ctc[linie][matrix_ctc[linie][0]]);
        else
            printf("%d",matrix_ctc[linie][matrix_ctc[linie][0]]);
        visited[linie] = 1;
    }
    free(visited);
}

void tare_conex(int *visited, graf *g, graf *g_t, Stack *s,int d, int *depo){
    int **matrix_ctc = malloc((g->n+1)*sizeof(int*));
    for(int i = 0; i < (g->n+1); i++)
        matrix_ctc[i] = malloc((g->n+1)*sizeof(int));
    for(int i = 0; i < (g->n+1); i++)
        for(int j = 0; j < (g->n+1); j++){
            if(j!=0) 
                matrix_ctc[i][j] = -1;
            else
                matrix_ctc[i][j] = 0;
        }

    apel_dfs(visited, g, &s,matrix_ctc,0);

    for(int i = 0; i < g->n+1; i++)
        matrix_ctc[0][i] = -1;
    matrix_ctc[0][0] = 0;

    int *visited2 = calloc(g->n,sizeof(int)), x, ctc = 0;
    
    while(s->top != NULL){
        x = pop_stack(s);
        if(visited2[x] == 0){
            dfs(g_t,visited2,x,&s,matrix_ctc,ctc);
            ctc++;
        }
    }
    printf("%d\n",ctc-d);
    sortare_matrix(matrix_ctc,ctc);
    print_matrix(matrix_ctc,ctc,d,depo);
        
    for(int i = 0; i <= g->n; i++)
        free(matrix_ctc[i]);
    free(matrix_ctc);
    free(visited2);
}

typedef struct coada{
    int id, masca;
}coada;

void decimal_to_binary(int *viz, int mask){
    int cnt = 0;
    
    while(mask > 0){
        viz[cnt++] = mask%2;
        mask/=2;
    }
}

int completed(int *viz, int nr_elem){
    for(int i = 0; i < nr_elem; i++)
        if(viz[i] == 0)
            return 0;
    return 1;
}

double mod_djikstra(graf *g, int *elem, int nr_elem, int depo, int stop){
    double **vis_mask = calloc(2*nr_elem,sizeof(double*));
    int mask,x;
    //int auxiliar = 
    for(int i = 0; i < nr_elem; i++)
        vis_mask[i] = calloc(2*pow(2,nr_elem),sizeof(double));
    
    int maxim = 1, power = 2;
    for(int i = 1; i < nr_elem; i++)
        maxim += power, power*=2;

    int nr_elem_coada = -1,c = 1;
    int *viz = calloc(nr_elem,sizeof(int));
    double *cost = malloc(nr_elem*sizeof(double));

    for(int i = 0; i < nr_elem; i++)
        cost[i] = inf;

    coada *q = malloc(2*g->n*sizeof(coada));

    q[++nr_elem_coada].id = depo;
    q[nr_elem_coada].masca = c;
    
    while(nr_elem_coada != -1){
        x = q[0].id;
        mask = q[0].masca;
        c=mask;
        for(int i = 0; i < nr_elem; i++)
            viz[i] = 0;
        decimal_to_binary(viz,mask);
        /*if(x != depo){
            for(int i = 0; i < nr_elem; i++)
                if(x == elem[i]){
                    vis_mask[i][mask] = 1;
                    break;
                }
        }*/
        if(nr_elem_coada > 0)
            for(int i = 0; i <= nr_elem_coada - 1; i++)
                {q[i].id = q[i+1].id;q[i].masca = q[i+1].masca;}
        nr_elem_coada--;

        if(x == stop && mask == maxim)
            break;
        for(int i = 0; i < nr_elem; i++){
            if(g->a[x][elem[i]] != inf){
                int auxiliar;
                for(int j = 0; j < nr_elem; j++){
                    if(elem[j] == x)
                        auxiliar = j;
                }
                if(viz[i] == 1 && vis_mask[i][c] == 0){
                    q[++nr_elem_coada].id = elem[i];
                    q[nr_elem_coada].masca = c;
                    vis_mask[i][c] = inf;
                }
                if(viz[i] == 0 && vis_mask[i][c+(int)pow(2,i)] == 0){
                    q[++nr_elem_coada].id = elem[i];
                    q[nr_elem_coada].masca = c+pow(2,i);
                    vis_mask[i][c+(int)pow(2,i)] = inf;
                    c += (int)pow(2,i);
                }
                
                if(x == depo)
                    if(vis_mask[i][c] > g->a[x][elem[i]])
                        vis_mask[i][c] = g->a[x][elem[i]];
                if(x != depo)
                    if(vis_mask[i][c] > g->a[x][elem[i]]+vis_mask[auxiliar][mask])
                        vis_mask[i][c] = g->a[x][elem[i]] + vis_mask[auxiliar][mask];
                
            }
        }
    }
    
    for(int i = 0; i < nr_elem; i++)
        {for(int j = 0; j < pow(2,nr_elem); j++)
            printf("%d ",vis_mask[i][j]);
        printf("\n");}
    return 1;
}
int main(){
    int n,m,d,x,y,nr_cerinte,plec,k;
    double cost;
    int *depozite, *magazine;
    graf *g;
    char id[3];

    scanf("%d%d%d",&n,&m,&d);

    depozite = malloc(d*sizeof(int));

    init_graf(&g,n);

    for(int i = 1; i <= m; i++){
        scanf("%d%d%lf",&x,&y,&cost);
        g->a[x][y] = cost;
    }
    
    for(int i = 0; i < d; i++)
        scanf("%d",&depozite[i]);

    scanf("%d",&nr_cerinte);

    for(int q = 1; q <= nr_cerinte; q++){
        scanf("%s",id);
        if(strcmp(id,"e1") == 0){
            int *vector = malloc(n*n*sizeof(int));
            int cnt = 0;
            //depozitul de unde voi pleca
            scanf("%d",&plec);
            //nr de magazine aprovizionate
            scanf("%d",&k);

            magazine = malloc(k*sizeof(int));

            for(int i = 0; i < k; i++){
                scanf("%d",&x);
                magazine[i] = x;
            }
            float c, cost_dus,cost_intors,cost_final=0;
            for(int j = 0; j < k; j++){
                cnt = 0;
                printf("%d\n",magazine[j]);
                vector[cnt++] = plec;
                Djikstra(g,plec,magazine[j],&c,vector,&cnt);
                cost_dus = c;
                Djikstra(g,magazine[j],plec,&c,vector,&cnt);
                cost_intors = c;
                printf("%.1lf %.1lf\n",cost_dus,cost_intors);
                for(int k = 0; k < cnt-1; k++)
                    printf("%d ",vector[k]);
                printf("%d\n",vector[cnt-1]);
                cost_final += (cost_dus + cost_intors);
            
            }
            printf("%.1lf",cost_final);
            free(vector);
            free(magazine);
        }
        if(strcmp(id,"e2") == 0){
            Stack *s;
            init_stack(&s);
            int *visited = calloc(n,sizeof(int));
            graf *g_t;
            init_graf(&g_t,n);
            //construim graful transpus
            for(int i = 0; i < d; i++)
                for(int j = 0; j < n; j++)
                    g->a[depozite[i]][j] = g->a[j][depozite[i]] = inf;
            for(int i = 0; i < n; i++)
                for(int j = 0; j < n; j++)
                    g_t->a[i][j] = g->a[j][i];
            tare_conex(visited,g,g_t,s,d,depozite);
            free(visited);
            free(s);
            free_graf(&g_t);
        }
        if(strcmp(id,"e3") == 0){
            int r, k, *elem = malloc(2*n*sizeof(int));
            double minim = inf,aux;
            scanf("%d",&r);
            for(int i = 0; i < r; i++){
                
                scanf("%d",&k);
                for(int j = 0; j < k; j++)
                    scanf("%d",&elem[j]);
                for(int j = 0; j < d; j++)
                    for(int p = 0; p < k; p++)
                        if(g->a[elem[p]][depozite[j]] != inf){
                            aux = mod_djikstra(g,elem,k,depozite[j],elem[p]);
                            if(minim > aux)
                                minim = aux;
                        }
                printf("%.1lf",minim);
                //if(k==4)mod_djikstra(g,elem,k,0,6);
                //if(k==3)mod_djikstra(g,elem,k,0,1);
                //if(k==1)mod_djikstra(g,elem,k,0,8);
                if(i < r-1)
                    printf("\n");
                
            }
            free(elem);

        }
        if(q != nr_cerinte)
            printf("\n");
    }
    free_graf(&g);
    free(depozite);
}