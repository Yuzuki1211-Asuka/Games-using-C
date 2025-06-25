#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#define h 20
#define w 30
int d;
int c[h][w]={0};//designing a simulated canva
int di;//direction
int fx,fy;//position of food
int score=0;
void HideCursor()//hiding cursor
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
void gotoxy(int x,int y)//eliminating flashing
{
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}
void move()
{
	for(int i=1;i<h-1;i++)
	{
		for(int j=1;j<w-1;j++)
		{
			if(c[i][j]>0)
			{
				c[i][j]++;
			}
		}
	}
	int ti,tj,hi,hj;//old tail and old head(?)
	int max=0;
	for(int i=1;i<h-1;i++)
	{
		for(int j=1;j<w-1;j++)
		{
			if(c[i][j]>0)
			{
				if(max<c[i][j])
				{
					max=c[i][j];
					ti=i;
					tj=j;
				}
				if(c[i][j]==2)
				{
					hi=i;
					hj=j;
				}
			}
		}
	}
	c[ti][tj]=0;
	int h1,h2;
	if(di==1)//up
	{
		h1=hi-1;
		h2=hj;
	}
	if(di==2)//down
	{
		h1=hi+1;
		h2=hj;
	}
	if(di==3)//left
	{
		h1=hi;
		h2=hj-1;
	}
	if(di==4)//right
	{
		h1=hi;
		h2=hj+1;
	}
	if(c[h1][h2]==-3)
	{
		c[fx][fy]=0;
		fx=rand()%(h-5)+2;
		fy=rand()%(w-5)+2;
		c[fx][fy]=-3;//generating a new food(?
		score+=1;
	}
	else
	{
		c[ti][tj]=0;
	}
	if(c[h1][h2]>0||c[h1][h2]==-1||c[h1][h2]==-2)
	{
		printf("Game over\n");
		exit(0);
	}
	else
	{
		c[h1][h2]=1;
	}
}
void startup()
{
	for(int i=0;i<h;i++)
	{
		c[i][0]=-2;
		c[i][w-1]=-2;
	}
	for(int j=0;j<w;j++)
	{
		c[0][j]=-1;
		c[h-1][j]=-1;
	}
	c[h/2][w/2]=1;//position of snake head
	for(int i=1;i<=4;i++)
	{
		c[h/2][w/2-i]=i+1;//position of snake body
	}
	di=4;
	fx=rand()%(h-5)+2;
	fy=rand()%(w-5)+2;
	c[fx][fy]=-3;
}
void show()
{
	gotoxy(0,0);
	for(int i=0;i<h;i++)
	{
		for(int j=0;j<w;j++)
		{
			if(c[i][j]==0)
			{
				printf(" ");//editing all the blank
			}
			else if(c[i][j]==-2)
			{
				printf("|");
			}
			else if(c[i][j]==-1)
			{
				printf("-");
			}//actually optimizing the display of frame
			else if(c[i][j]==1)
			{
				printf("@");//snake head
			}
			else if(c[i][j]>1)
			{
				printf("*");//snake body
			}
			else if(c[i][j]==-3)
			{
				printf("o");//snake body
			}
		}
		printf("\n");
	}
	printf("Score: %d\n", score);
	Sleep(100);
}
void updatenoinput()
{
	move();
}
void updateinput()
{
	int input;
	if(kbhit())//adjusting the whole logic to two controlling methods
	{
		input=getch();
		if(input=='a'||input==75)//left
		{
			di=3;
			move();
		}
		if(input=='d'||input==77)//right
		{
			di=4;
			move();
		}
		if(input=='w'||input==72)//up
		{
			di=1;
			move();
		}
		if(input=='s'||input==80)//down
		{
			di=2;
			move();
		}
	}
}
int main()
{
	HideCursor();
	startup();
	while(1)
	{
		show();
		updatenoinput();
		updateinput();
	}
	return 0;
}
//little modification
//adding the score bulletin--
