#include"STACK.h"
#include<stdio.h>

ceng CENG[SIZE];
int main(){
	int res=1,dec=1;
	char con;
	int newrow;
	int newcol;
	int step=0;
	int level=0;
 while(1)
{	 if (level==0)
	{
		do{
		printf("请输入游戏等级:\n");
		scanf("%d",&level);
		if(level<=0||level>8)
		printf("等级范围错误，重新输入\n");
		}while(level<=0||level>8);
	}
	level=8;
	newrow=0,newcol=0;
	stack STACK[3];
	//初始化3个栈
	init_stack1(&STACK[0]);
	init_stack2_3(&STACK[1],&STACK[2]);
	//初始化8个层
	init_ceng1(&CENG[0]);
	init_ceng2(&CENG[1]);
	init_ceng3(&CENG[2]);
	init_ceng4(&CENG[3]);
	init_ceng5(&CENG[4]);
	init_ceng6(&CENG[5]);
	init_ceng7(&CENG[6]);
	init_ceng8(&CENG[7]);
while(1)
{	
	//打印画面
	switch(level){
	case 1:show_map();break;
	case 2:show_map();break;
	case 3:show_map();break;
	case 4:show_map();break;
	case 5:show_map();break;
	case 6:show_map();break;
	case 7:show_map();break;
	case 8:show_map();break;
	}
while(1)
  {	
 // printf("游戏等级为:%d\n",level);
//	printf("源栈最高层是%d  ......\n",top_stack(&STACK[res]));
	printf("     一号的高度%d ",STACK[0].head);
	printf("           二号的高度%d ",STACK[1].head);
	printf("          三号的高度%d\n",STACK[2].head);
	printf("\n已经走的步数为 %d \n",step);
	//选择源 目标
	scanf("%d",&res);
	scanf("%d",&dec);
	res--;
	dec-- ;
	if(!(res>=0&&res<3&&dec>=0&&dec<3))/*||(empty_stack(&STACK[res]))*/
	{	printf("\n\n输入范围超出\n");
	}else if(empty_stack(&STACK[res])==1)
	{
		printf("%d\n",STACK[0].head);
		printf("\n\n源栈空\n");
	}
	else if(sizecmp_stack(&STACK[res],&STACK[dec])<0)
	{	printf("\n\n大块的不能放在小块的上面\n");
	}else
	{	
		if(dec!=res){
		printf("\n\n\n正在移动层块....\n");
		step++;
		move_ceng(level,&newrow,&newcol,&STACK[0],&STACK[1],&STACK[2],dec);
		CENG[top_stack(&STACK[res])-1].row=newrow;
		CENG[top_stack(&STACK[res])-1].col=newcol;
		push_stack(&STACK[dec],pop_stack(&STACK[res]));
		break;
		}else 
		{
			printf("\n\n\n放轻松\n");
		}break;

	}
	show_map();
  }
	
	if (full_stack(STACK+1)==1||full_stack(STACK+2)==1)
	{
		printf("完成了汉诺塔!\n");
		level++;
		break;
	}
}
	show_map();
	printf("是否继续游戏?继续请输入y,否则按其它键\n");
	scanf("%*[^\n]");
	scanf("%*c");
	scanf("%c",&con);
	if(con!='y')
	break;
}
	return 0;
}

