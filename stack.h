#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 81

struct point{
    int x,y;
};

struct point* point_from_2d(int x,int y){
    struct point* p=malloc(sizeof(struct point));
    p->x=x;
    p->y=y;
    return p;
}

struct stack{
    struct point data[MAX_HEIGHT];
    unsigned short height;
};

struct stack* init_stack(){
    struct stack* st=malloc(sizeof(struct stack));
    st->height=0;
    return st;
}

int push(struct stack* st, int x, int y){
    if (st->height==MAX_HEIGHT){
        return -1;
    }
    st->data[st->height]=(struct point){x,y};
    st->height++;
    return 0;
}

int pop(struct stack* st, int* x, int* y){
    // printf("[DEBUG] st->height = %d\n", st->height);
    if (st->height==0){
        return -1;
    }
    st->height--;
    // printf("[DEBUG] st->height = %d, st->data[0].x=%d,st->data[0].y=%d\n", st->height,st->data[0].x,st->data[0].y);
    *x=(st->data)[st->height].x; *y=(st->data)[st->height].y;
    return 0;
}