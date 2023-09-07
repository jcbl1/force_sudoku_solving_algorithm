#include <stdio.h>
#include "stack.h"
// #include <unistd.h>

struct cell{
    unsigned short value;
};

struct cell* empty_cell(){
    // static x=0;
    struct cell* ce =malloc(sizeof(struct cell));
    // ce->value = x++;
}

struct grid{
    struct cell** cells;
};

struct grid* empty_grid(){
    struct grid* gr=malloc(sizeof(struct grid));
    gr->cells=malloc(sizeof(struct cell*)*3);
    for (int i = 0; i <3;i++){
        gr->cells[i]=malloc(sizeof(struct cell)*3);
        for (int j = 0; j <3;j++){
            gr->cells[i][j]=*(empty_cell());
        }
    }

    return gr;
}

struct sheet{
    struct grid** grids;
};

struct sheet* empty_sheet(){
    struct sheet* sh = malloc(sizeof(struct sheet));
    sh->grids=malloc(sizeof(struct grid*)*3);
    for (int i = 0; i <3;i++){
        sh->grids[i]=malloc(sizeof(struct grid)*3);
        for (int j = 0; j <3;j++){
            sh->grids[i][j]=*(empty_grid());
        }
    }
    return sh;
}

struct sheet* fill_from_vector(struct sheet* self, unsigned short* vec){
    for (int i=0; i <9; i++){
        for (int j=0; j <9; j++){
            int i1=i/3,j1=j/3,i2=i%3,j2=j%3;
            (self->grids)[i1][j1].cells[i2][j2].value=vec[9*i+j];
        }
    }
    return self;
}

struct sheet* from_vector(unsigned short* vec){
    struct sheet* sh=empty_sheet();
    sh=fill_from_vector(sh,vec);
    return sh;
}

void print_sheet(const struct sheet* self){
    for (int i=0; i <9; i++){
        for (int j=0; j <9; j++){
            int i1=i/3,j1=j/3,i2=i%3,j2=j%3;
            printf("%d  ",(self->grids)[i1][j1].cells[i2][j2].value);
            if (j==8){
                printf("\n");
            }
        }
    }
}

int check_cell(const struct sheet* self, int x,int y){
    unsigned short orphan = (self->grids)[x/3][y/3].cells[x%3][y%3].value;
    int count;
    // printf("[Debug] orphan: %d\n", orphan);
    // check row
    count=0;
    for (int i=0; i<3; i++){
        int i1=x/3,j1=i;
        for (int j=0; j<3; j++){
            int i2=x%3,j2=j;
            // printf("[DEBUG] (self->grids)[i1][j1].cells[i2][j2].value=%d\n",(self->grids)[i1][j1].cells[i2][j2].value);
            if ((self->grids)[i1][j1].cells[i2][j2].value==orphan){
                count++;
            }
        }
    }
    // printf("[DEBUG] count = %d\n",count);
    if (count!=1){
        return 1; //failed
    }
    // check column
    count=0;
    for (int i=0; i<3; i++){
        int i1=i,j1=y/3;
        for (int j=0; j<3; j++){
            int i2=j,j2=y%3;
            if ((self->grids)[i1][j1].cells[i2][j2].value==orphan){
                count++;
            }
        }
    }
    if (count!=1){
        return 1; //failed
    }
    // check grid
    count=0;
    int i1=x/3,j1=y/3;
    for (int i=0; i<3; i++){
        for (int j=0; j<3; j++){
            int i2=i,j2=j;
            if ((self->grids)[i1][j1].cells[i2][j2].value==orphan){
                count++;
            }
        }
    }
    // printf("count = %d\n",count);
    if (count!=1){
        return 1;//failed
    }
    return 0; //passed
}

struct sheet* solve(struct sheet* self){
    struct stack* st = init_stack();
    // printf("[DEBUG] st->height = %d\n",st->height);

    for (int step=0;step<MAX_HEIGHT;step++){
        // usleep(20000);

        // printf("[DEBUG] step = %d\n",step);
        int x=step/9,y=step%9;
        int i1=x/3,j1=y/3,i2=x%3,j2=y%3;
        if ((self->grids)[i1][j1].cells[i2][j2].value==0){
            push(st,x,y);
        }
        if (st->height>0){
            int tmp_x,tmp_y;
            pop(st,&tmp_x,&tmp_y);
            push(st,tmp_x,tmp_y);
            if (tmp_x==x&&tmp_y==y){
                if ((self->grids)[i1][j1].cells[i2][j2].value==9){
                    (self->grids)[i1][j1].cells[i2][j2].value=0;
                    pop(st,&tmp_x,&tmp_y);
                    pop(st,&x,&y);
                    step=9*x+y-1;
                    push(st,x,y);
                    continue;
                }
                (self->grids)[i1][j1].cells[i2][j2].value++;
                int err=check_cell(self,x,y);
                if (err!=0){
                    // printf("check faild\n");
                    if ((self->grids)[i1][j1].cells[i2][j2].value<9){
                        step--;
                    } else{
                        (self->grids)[i1][j1].cells[i2][j2].value=0;
                        pop(st,&tmp_x,&tmp_y);
                        pop(st,&x,&y);
                        // printf("[DEBUG] x=%d,y=%d\n",x,y);
                        step=9*x+y-1;
                        push(st,x,y);
                    }
                }
            }
        }

        // printf("--------------------------------------------------------\n");
        // print_sheet(self);
    }

    return self;
}



int main(){
    // struct sheet* sh=empty_sheet();
    // printf("sh->grids[1][1]->cells[2][2]=%d\n",(sh->grids)[1][1].cells[2][2]);

    // unsigned short vec[81]={
    //     0,0,5, 0,0,6, 1,0,0,
    //     0,0,1, 7,0,0, 4,9,0,
    //     7,0,0, 0,0,0, 3,0,0,
    //     0,0,0, 6,0,5, 9,0,4,
    //     5,0,0, 0,0,0, 0,1,0,
    //     0,0,0, 0,3,4, 0,5,0,
    //     0,0,6, 0,7,0, 0,0,0,
    //     0,0,9, 1,0,2, 0,6,0,
    //     0,0,0, 0,0,0, 2,0,0
    // };
    unsigned short vec[MAX_HEIGHT]={1,9,2};
    struct sheet* sh=from_vector(vec);
    print_sheet(sh);
    printf("Solving...\n");
    solve(sh);
    print_sheet(sh);

    return 0;
}