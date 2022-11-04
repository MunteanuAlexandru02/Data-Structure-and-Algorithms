typedef struct intervale{
    int value;
    struct intervale *next;
    struct intervale *prev;
}intervale;

typedef struct LL{
    intervale *head;
    intervale *tail;
    int len;
}LL;
/*Fisierul contine functii, care vor fi folosite pentru a construi intervalele*/
void init_interval(LL **list){
    (*list) = malloc(sizeof(LL));
    (*list)->head = (*list)->tail = NULL;
    (*list)->len = 0;
    return;
}

void add_interval(LL *list, int x){
    intervale *newnode = malloc(sizeof(intervale));
    newnode->next = newnode->prev = NULL;
    newnode->value = x;

    if(list->head == NULL) // primul element
        list->head = list->tail = newnode;
    
    else{ //inseram la finalul listei
        list->tail->next = newnode;
        newnode->prev = list->tail;
        newnode->next = NULL;
        list->tail = newnode;
    }
    list->len++;
    return;
}

void print2(LL *list){
    intervale *newnode = list->head;
    printf("%d\n",list->len);
    while(newnode != NULL){
        printf("%d\n",newnode->value);
        newnode = newnode->next;
    }
}

void free_intervale(LL **list){
    intervale *newnode;

    while((*list)->head != NULL){
        newnode = (*list)->head;
        (*list)->head = (*list)->head->next;
        free(newnode);
    }
    
    free(*list);
}