#ifndef 		__STACK_H__
#define			__STACK_H__
#define			SIZE 		8
typedef struct STACK{
	int head;
	int arr[SIZE];
}stack;

typedef struct CENG{
	int number;
	int row;
	int col;
}ceng;

extern ceng CENG[SIZE];

//入栈
void push_stack(stack*,int);
//初始化栈1
void init_stack1(stack*);
//初始化栈2和3
void init_stack2_3(stack*,stack*);
//出栈
int pop_stack(stack*);
//访问栈顶元素
int top_stack(stack*);
//比较栈顶元素大小 -1 1
int sizecmp_stack(stack*,stack*);
//判断栈的高度
int high_stack(stack*);
//判断栈是否为空1 0
int empty_stack(stack*);
//判断栈是否满1 0
int full_stack(stack*);

//初始化层1
void init_ceng1(ceng*);
//初始化层2
void init_ceng2(ceng*);
//初始化层3
void init_ceng3(ceng*);
//初始化层4
void init_ceng4(ceng*);
//初始化层5
void init_ceng5(ceng*);
//初始化层6
void init_ceng6(ceng*);
//初始化层7
void init_ceng7(ceng*);
//初始化层8
void init_ceng8(ceng*);
//移动层块
void move_ceng(int ,int * a,int *b,stack*,stack*,stack*,int k );
//打印画面
void show_map();
#endif  

