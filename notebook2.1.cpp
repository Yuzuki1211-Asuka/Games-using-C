#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
int plane(int *y)
{
	for(int j=0;j<(*y);j++)
		{
			printf(" ");
		}
		printf("  *\n");
		for(int j=0;j<(*y);j++)
		{
			printf(" ");
		}
		printf("*****\n");
		for(int j=0;j<(*y);j++)
		{
			printf(" ");
		}
		printf(" * * \n");
}
void move(int *x,int *y,char input,int *fire)
{
		if(kbhit())
		{
			input=getch();
			if(input=='a')
			{
				(*y)--;
			}
			if(input=='d')
			{
				(*y)++;
			}
			if(input=='s')
			{
				(*x)++;
			}
			if(input=='w')
			{
				(*x)--;
			}
			if(input==' ')
			{
				(*fire)=1;
			}
		}
}
int laser(int *fire,int *x,int *y,int *kill,int *ny)
{
	if((*fire)==0)
		{
			for(int i=0;i<(*x);i++)
			{
				printf("\n");
			}
		}
		else
		{
			for(int i=0;i<(*x);i++)
			{
				for(int j=0;j<(*y);j++)
				{
					printf(" ");
				}
				printf("  |\n");
			}
			if((*y)+2==(*ny))
			{
				(*kill)=1;
			}
			fire=0;
		}
}
void displayScore(int score)
{
    printf("=====================\n");
    printf("     SCORE: %04d     \n", score);
    printf("=====================\n\n");
}
int main()
{
	int x=5,y=10;
	char input;
	int fire=0;
	int ny=5;
	int kill=0,score=0;
	while(1)
	{
		system("cls");
		displayScore(score);
		if(!kill)
		{
			for(int j=0;j<ny;j++)
			{
				printf(" ");
			}
			printf(" +\n");
		}
		if(kill)
		{
			score += 10;        // 每次击中得10分
			kill = 0;           // 重置击中状态
			ny = rand() % 30;   // 随机生成新目标位置
		}
		laser(&fire,&x,&y,&kill,&ny);
		plane(&y);
		move(&x,&y,input,&fire);
		for(int k = 0; k < 50000000; k++) {}	
	}
	return 0;
}
