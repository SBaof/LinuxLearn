#include"STACK.h"
#include<stdio.h>
extern ceng CENG[SIZE];
//数据入栈
void push_stack(stack*p,int number){
	p->arr[p->head]=number;
	p->head++;
}

//初始化栈1
void init_stack1(stack*p){
	p->head=0;
	push_stack(p,1);
	push_stack(p,2);
	push_stack(p,3);
	push_stack(p,4);
	push_stack(p,5);
	push_stack(p,6);
	push_stack(p,7);
	push_stack(p,8);
}
//初始化栈2 3
void init_stack2_3(stack* p1,stack* p2){
	p1->head=0;
	p1->arr[p1->head]=0;
	p2->head=0;
	p2->arr[p2->head]=0;
}
//弹出栈顶元素
int pop_stack(stack* p){
	p->head--;
	return p->arr[p->head];
}
//访问栈顶元素
int top_stack(stack* p){
	return p->arr[p->head-1];
}
//比较两个栈顶元素的大小
int sizecmp_stack(stack* p1,stack* p2){
	if(p1->arr[p1->head-1]>p2->arr[p2->head-1])
	return 1;
	else if(p1->arr[p1->head-1]<p2->arr[p2->head-1])
	return -1;
	else return 0;
}

//测出栈的高度
int high_stack(stack* p){
	return p->head;
}

//是否为空栈
int empty_stack(stack* p){
	return p->head==0;
}

//是否栈满
int full_stack(stack* p){
 return p->head==SIZE;
}

//初始化层1
void init_ceng1(ceng* p){
	p->number=1;
	p->row=SIZE-1;
	p->col=0;
}

//初始化层2
void init_ceng2(ceng* p){
	p->number=2;
	p->row=SIZE-2;
	p->col=0;
}

//初始化层3
void init_ceng3(ceng* p){
	p->number=3;
	p->row=SIZE-3;
	p->col=0;
}

//初始化层4
void init_ceng4(ceng* p){
	p->number=4;
	p->row=SIZE-4;
	p->col=0;
}
//初始化层5
void init_ceng5(ceng*p){
	p->number=5;
	p->row=SIZE-5;
	p->col=0;
}
//初始化层6
void init_ceng6(ceng*p){
	p->number=6;
	p->row=SIZE-6;
	p->col=0;
	}
//初始化层7
void init_ceng7(ceng*p){
	p->number=7;
	p->row=SIZE-7;
	p->col=0;
}
//初始化层8
void init_ceng8(ceng*p){
	p->number=8;
	p->row=SIZE-8;
	p->col=0;
}
//移动层
void move_ceng(int level,int *nrow,int *ncol,stack*p1,stack* p2,stack* p3,int stdec){
	stack* arr[3];
	 arr[0]=p1;
	 arr[1]=p2;
	 arr[2]=p3;
	*nrow=level-1-high_stack(arr[stdec]);
	*ncol=stdec;
}

 //显示图片
void show_map(void){
	int i,j;
	for(i=0;i<SIZE;i++)
	{
		for(j=0;j<3;j++)
		{	
		  if(CENG[0].row==i&&CENG[0].col==j)
		  {	
		  	printf("   |       1       |   ");
		 }else if(CENG[1].row==i&&CENG[1].col==j)
		  {
		  	printf("    |      2      |    ");
		 }else if(CENG[2].row==i&&CENG[2].col==j)
		  {
		  	printf("     |     3     |     ");
		 }else if(CENG[3].row==i&&CENG[3].col==j)
		 {
		 	printf("      |    4    |      ");
		 }else if(CENG[4].row==i&&CENG[4].col==j)
		 {
		 	printf("       |   5   |       ");
		 }else if(CENG[5].row==i&&CENG[5].col==j)
		 {
		 	printf("        |  6  |        ");
		 }else if(CENG[6].row==i&&CENG[6].col==j){
		 	printf("         | 7 |         ");
		 }else if(CENG[7].row==i&&CENG[7].col==j){
		 	printf("          |8|          ");
    }else   printf("                       ");
		}
		printf("\n");
	}
	printf("=====================================================================\n");
}
