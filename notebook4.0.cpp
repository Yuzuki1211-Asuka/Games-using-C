#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
int h,w;
int bx,by;
int ox1,ox2,oy;
int score;
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
void gotoxy(int x,int y)
{
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}
void startup()
{
	h=15;
	w=20;
	bx=0;
	by=w/3;
	oy=w/2;
	ox1=h/3;
	ox2=h/2;
}
void show()
{
	gotoxy(0,0);
	for(int i=0;i<h;i++)
	{
		printf("|");
		for(int j=0;j<w;j++)
		{
			if((i==bx)&&(j==by))
			{
				printf("@");
			}
			else if((j==oy)&&((i<ox1)||(i>ox2)))
			{
				printf("*"); 
			}
			else
			{
				printf(" ");
			}
		}
		printf("|\n");
	}
	for(int j=0;j<w+2;j++)
	{
		printf("-");
	}
	printf("\n");
	printf("score: %d\n",score);
}
void updatenoinput()
{
	bx++;
	oy--;
	if(by==oy)
	{
		if((bx>=ox1)&&(bx<=ox2))
		{
			score++;
		}
		else
		{
			printf("Game over\n");
			system("pause");
			exit(0);
		}
	}
	if(oy<=0)
	{
		oy=w;
		int temp=rand()%int(h*0.8);
		ox1=temp-h/10;
		ox2=temp+h/10;
	}
	Sleep(150);
}
void updateinput()
{
	char input;
	if(kbhit())
	{
		input=getch();
		if(input==' ')
		{
			bx=bx-2;
		}
	}
}
int main()
{
	startup();
    system("cls");
    HideCursor();
	while(1)
	{
		show();
		updatenoinput();
		updateinput();
	}
	return 0;
}
