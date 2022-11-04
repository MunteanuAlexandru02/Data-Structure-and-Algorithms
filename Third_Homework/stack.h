typedef struct node{
    int  val;
    struct node* next;
}node;

typedef struct Stack{
    struct node *top;
    int len;
}Stack;

void init_stack(Stack **s){
    (*s) = malloc(sizeof(Stack));
    (*s)->len = 0;
    (*s)->top = NULL;
}

int add_stack(Stack *s, int x){
    node *newnode = malloc(sizeof(node));
    if(newnode == NULL)
        return 0;
    newnode->val = x;
    newnode->next = s->top;
    if(s->top == NULL)
        {s->top = newnode; s->top->next = NULL;}
    else{
        newnode->next = s->top;
        s->top = newnode;
    }
    s->len++;
    return 1;
}

int pop_stack(Stack *s){
    node *newnode = s->top;
    s->top = newnode->next;
    int de_retur = newnode->val;
    s->len--;
    free(newnode);
    return de_retur;
}

void print_stack(Stack *s){
    node *newnode = s->top;
    for(int i = 0; i < s->len; i++){
        printf("%d ", newnode->val);
        newnode = newnode->next;
    }
    printf("\n");
}