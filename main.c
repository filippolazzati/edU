#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Constants
#define DIM 1025
#define SIZE 100000
typedef char string[1025];

//structs
struct obj1{
  int n1,n2;
  char cmd;
  char cmd2;
  int dim;
  int n;
  int n_cmd;
  int n_cmd_tot;
  struct obj1* next;
  struct obj1* back;
  struct obj1* last;
  int eff_dim;
  list2 *array;
  list2 *head;
  list2 *head_undo;
};

typedef struct obj1* list1;

struct obj2{
  char *s;
};

typedef struct obj2* list2;

//functions' declaration
void change_l(list1 *vec,int *f,int *ptr,list1 *ff,list1 *pt, int n1,int n2,int n_cmd_tot);
list1 build_obj1(int n1,int n2, char c,int n_cmd_tot);
void print_l(list1 pt,int n1,int n2);
list2 read_string();
void delete_l(list1 *vec,int *f,int *ptr,list1 *ff,list1 *pt,int n1,int n2,int n_cmd_tot);
void undo(list1 *vec,int *f,int *ptr,list1 *ff,list1 *pt,int n_undo);

//main
int main()
{
int a=0,l_array=SIZE,n_undo=0,n1,n2,digit,f=0,ptr=0,n_cmd_tot=0;
char temp_cmd,cmd;
list1 *vec=malloc(SIZE*sizeof(list1));
list1 pt=NULL,ff=NULL;

vec[0]=malloc(sizeof(struct obj1));
vec[0]->dim=0;
vec[0]->last=vec[0];
vec[0]->eff_dim=0;
vec[0]->n_cmd=0;
vec[0]->n_cmd_tot=0;
vec[0]->n=0;
vec[0]->cmd2='z';
pt=vec[0];
ff=vec[0];

while(a==0){
n1=0;
digit=fgetc(stdin);
if(digit=='q'){
  temp_cmd=fgetc(stdin);
  a=1;
}
else{
  while(digit>47 && digit<58){
  n1=n1*10;
  digit=digit-48;
  n1=n1+digit;
  digit=fgetc(stdin);
}
temp_cmd=digit;

if(temp_cmd=='u') {
  if(pt->n_cmd_tot-n_undo-n1<=0)
  n_undo=pt->n_cmd_tot;
  else
  n_undo=n_undo+n1;
temp_cmd=fgetc(stdin);
}
else
if(temp_cmd=='r'){
 if(pt->n_cmd_tot+n1-n_undo>=ff->n_cmd_tot)
    n_undo=-1*(ff->n_cmd_tot-pt->n_cmd_tot);
  else
  n_undo=n_undo-n1;
  temp_cmd=fgetc(stdin);
}
else
if(temp_cmd==44){
n2=0;
digit=fgetc(stdin);
while(digit>47 && digit<58){
  n2=n2*10;
  digit=digit-48;
  n2=n2+digit;
  digit=fgetc(stdin);
}
cmd=digit;
temp_cmd=fgetc(stdin);

if(n_undo!=0){
undo(vec,&f,&ptr,&ff,&pt,n_undo);
}
n_undo=0;

if(cmd=='c'){
 if(ff!=pt){
    ff=pt;
    n_cmd_tot=pt->n_cmd_tot;
    f=ptr;
    pt->next=NULL;
    vec[ptr]->n_cmd=pt->n;
    vec[ptr]->last=pt;
  }
  n_cmd_tot++;
  change_l(vec,&f,&ptr,&ff,&pt,n1,n2,n_cmd_tot);
  temp_cmd=fgetc(stdin);
  temp_cmd=fgetc(stdin);
}
else
if(cmd=='d'){
  if(ff!=pt){
    ff=pt;
    n_cmd_tot=pt->n_cmd_tot;
    f=ptr;
    pt->next=NULL;
    vec[ptr]->n_cmd=pt->n;
    vec[ptr]->last=pt;
  }
  n_cmd_tot++;
delete_l(vec,&f,&ptr,&ff,&pt,n1,n2,n_cmd_tot);
}
else
if(cmd=='p'){
print_l(pt,n1,n2);
}
}
}
}
return 0;
}

//functions
void undo(list1 *vec,int *f,int *ptr,list1 *ff,list1 *pt,int n_undo)
{
  list1 now=NULL,x=NULL;
  list2 *array=NULL;
  int index,i,n_undo_cpy=n_undo,j;
  now=*pt;

  if(n_undo>0){

    if(now->n-n_undo<=0){
      if(*ptr!=0){
      x=now->next;
      array=vec[*ptr]->array;
      while(x!=NULL){

        if(x->cmd2=='b' || x->cmd2=='c' || x->cmd2=='d'){
          memmove(array+x->n1-1,x->head,(x->n2-x->n1+1)*sizeof(list2));
        }
        if(x==*ff)
        break;
        x=x->next;
      }
      n_undo_cpy=n_undo_cpy-now->n;
      index=*ptr-1;
      while(1){
      if(n_undo_cpy==0){
        x=vec[index]->last;
        break;
      }
      else{
        if(vec[index]->n_cmd-n_undo_cpy>0){
          //printf("2 else\n");
          x=vec[index]->last;
          array=x->array;
          for(i=0;i<n_undo_cpy;i++){
            if(x->cmd2=='b'){
              memmove(array+x->n1-1,x->head_undo,(x->n2-x->n1+1)*sizeof(list2));
            }
            if(x->cmd2=='c'){
              memmove(array+x->n1-1,x->head_undo,(x->back->dim-x->n1+1)*sizeof(list2));
            }
            x=x->back;
          }
          break;
        }
        else{
          if(vec[index]->n_cmd-n_undo_cpy==0 && index==0){
            x=vec[index];
            break;
          }
          else{
          n_undo_cpy=n_undo_cpy-vec[index]->n_cmd;
          index--;
        }
        }
      }
      }
      *ptr=index;
      *pt=x;
    }
      else{
        *pt=vec[0];
      }
    }
    else{
    x=now;
    array=x->array;

    for(i=0;i<n_undo;i++){
      if(x->cmd2=='b'){
        memmove(array+x->n1-1,x->head_undo,(x->n2-x->n1+1)*sizeof(list2));
      }
      if(x->cmd2=='c'){
        memmove(array+x->n1-1,x->head_undo,(x->back->dim-x->n1+1)*sizeof(list2));
      }
      x=x->back;
    }
    *pt=x;
  }
  }

  if(n_undo<0){
    if(now->n-n_undo<=vec[*ptr]->n_cmd){
      x=now->next;
      for(i=0;i>n_undo;i--){
        if(x->cmd2=='b' || x->cmd2=='c' || x->cmd2=='d'){
          memmove(x->array+x->n1-1,x->head,(x->n2-x->n1+1)*sizeof(list2));
        }
        if(x==*ff) break;
        if(i-1==n_undo)
        break;
        x=x->next;
      }
      *pt=x;
    }
    else{
      array=now->array;
      x=now->next;
      while(x!=NULL){
          if(x->cmd2=='b' || x->cmd2=='c' || x->cmd2=='d'){
            memmove(array+x->n1-1,x->head,(x->n2-x->n1+1)*sizeof(list2));
          }
          x=x->next;
        }
      n_undo_cpy=0-n_undo_cpy;
      n_undo_cpy=n_undo_cpy-(vec[*ptr]->n_cmd-now->n);
      index=*ptr+1;
      while(1){
      if(n_undo_cpy-vec[index]->n_cmd==0){
        x=vec[index]->last;
        break;
      }
      else{
        if(n_undo_cpy-vec[index]->n_cmd<0){
          x=vec[index]->last;
          j=vec[index]->n_cmd-n_undo_cpy;
          array=x->array;
          for(i=0;i<j;i++){
            if(x->cmd2=='b'){
              memmove(array+x->n1-1,x->head_undo,(x->n2-x->n1+1)*sizeof(list2));
            }
            if(x->cmd2=='c'){
              memmove(array+x->n1-1,x->head_undo,(x->back->dim-x->n1+1)*sizeof(list2));
            }
            x=x->back;
          }
          break;
        }
        else{
          n_undo_cpy=n_undo_cpy-vec[index]->n_cmd;
          index++;
        }
      }
      }
      *pt=x;
      *ptr=index;
  }
  }
}

void change_l(list1 *vec,int *f,int *ptr,list1 *ff,list1 *pt, int n1,int n2,int n_cmd_tot)
{
  int back_dim,back_dim_eff,i,k;
  list2 buildto=NULL;
  list2 *temp=NULL;
  list1 w=NULL,now=NULL;
  now=*pt;
w=build_obj1(n1,n2,'c',n_cmd_tot);

back_dim=now->dim;
back_dim_eff=now->eff_dim;

if(n2>back_dim_eff){

w->array=malloc((2*n2)*sizeof(list2));
w->dim=n2;
w->eff_dim=2*n2;
w->n_cmd=1;
w->n=1;
w->cmd2='a';
vec[*ptr+1]=w;
vec[*ptr+1]->last=w;
if(back_dim!=0 && n1!=1)
memmove(w->array,now->array,(n1-1)*sizeof(list2));
for(i=n1-1;i<n2;i++){
  buildto=read_string(stdin);
    (w->array)[i]=buildto;
}
*ptr=*ptr+1;
*f=*f+1;
}
else{
  w->head=malloc((n2-n1+1)*sizeof(list2));
  w->array=vec[*ptr]->array;
  vec[*ptr]->n_cmd=vec[*ptr]->n_cmd+1;
  w->n=now->n+1;
  now->next=w;
  w->back=*pt;
  vec[*ptr]->last=w;
  w->eff_dim=back_dim_eff;

  if(n2<=back_dim){
  w->dim=back_dim;
  w->cmd2='b';
  w->head_undo=malloc((n2-n1+1)*sizeof(list2));
  memmove(w->head_undo,w->array+n1-1,(n2-n1+1)*sizeof(list2));
  }
  else{

  if(n1<=back_dim){
    w->cmd2='c';
  w->head_undo=malloc((back_dim-n1+1)*sizeof(list2));
  memmove(w->head_undo,w->array+n1-1,(back_dim-n1+1)*sizeof(list2));
  }
  else{
    w->cmd2='d';
  }
    w->dim=n2;
  }
  k=0;
  for(i=n1-1;i<n2;i++){
    buildto=read_string(stdin);
    (w->head)[k]=buildto;
    k++;
    (w->array)[i]=buildto;
  }
}

*pt=w;
*ff=w;
}

void delete_l(list1 *vec,int *f,int *ptr,list1 *ff,list1 *pt,int n1,int n2,int n_cmd_tot)
{
  int i,back_dim,back_dim_eff;
  list2 buildto;
  list1 w=NULL,now=NULL;
  now=*pt;

  if(n1==0 && n2!=0)
    n1=1;

  w=build_obj1(n1,n2,'d',n_cmd_tot);

  back_dim=now->dim;
  back_dim_eff=now->eff_dim;

if(n1==0 && n2==0){
w->n=now->n+1;
w->array=vec[*ptr]->array;
vec[*ptr]->n_cmd=vec[*ptr]->n_cmd+1;
w->dim=back_dim;
w->eff_dim=back_dim_eff;
now->next=w;
w->back=now;
w->cmd2='e';
vec[*ptr]->last=w;
}
else{
  if(n1>back_dim){
    vec[*ptr]->last=w;
    w->n=now->n+1;
    w->array=vec[*ptr]->array;
    vec[*ptr]->n_cmd=vec[*ptr]->n_cmd+1;
    w->dim=back_dim;
    w->eff_dim=back_dim_eff;
    now->next=w;
    w->back=now;
    w->cmd2='f';
  }
  else{
    w->n_cmd=1;
    w->n=1;
    w->array=malloc(back_dim_eff*sizeof(list2));
    vec[*ptr+1]=w;
    vec[*ptr+1]->last=w;
    *ptr=*ptr+1;
    *f=*f+1;
    if(n1!=1)
    memmove(w->array,now->array,(n1-1)*sizeof(list2));
    w->eff_dim=back_dim_eff;
    if(n2<=back_dim){
      if(n2<back_dim)
      memmove(w->array+n1-1,now->array+n2,(back_dim-n2)*sizeof(list2));
      w->dim=back_dim-(n2-n1+1);
      w->cmd2='g';
    }
    else{
      w->dim=n1-1;
      w->cmd2='h';
    }
  }
}
*pt=w;
*ff=w;
}

void print_l(list1 pt,int n1,int n2)
{
    int i,dim;
    list2 *temp=NULL;

    dim=pt->dim;
    temp=pt->array;

    if(n1==0){
      fputc('.',stdout);
      fputc('\n',stdout);
      if(n2!=0)
      n1=1;
  }
    if(n1!=0){
      if(n2<=dim){
    for(i=n1-1;i<n2;i++){
        fputs(temp[i]->s,stdout);
    }
    }
      else{
        if(n1>dim){
      for(i=n1;i<=n2;i++){
        fputc('.',stdout);
        fputc('\n',stdout);
      }
      }
        else{
          for(i=n1-1;i<dim;i++){
          fputs(temp[i]->s,stdout);
          }
          for(i=dim;i<n2;i++){
          fputc('.',stdout);
          fputc('\n',stdout);
          }
    }
    }
  }
  }

list2 read_string()
{

  list2 temp=malloc(sizeof(struct obj2));
  size_t c1=0;
   ssize_t c2;
   c2=getline(&(temp->s),&c1,stdin);
    return temp;
}

list1 build_obj1(int n1,int n2,char c,int n_cmd_tot)
{

    list1 temp=malloc(sizeof(struct obj1));
    temp->n1=n1;
    temp->n2=n2;
    temp->cmd=c;
    temp->n_cmd_tot=n_cmd_tot;
    return temp;
  }
