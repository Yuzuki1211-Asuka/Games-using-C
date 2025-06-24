#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
int h,w;
int bx,by;
int vx,vy;
int px,py;//center of plank
int r;//ridus of plank
int pl,pr;//left and right side of plank
int num;//times of bouncing balls
int score;
struct brick
{
	int x;
	int y;
	int active;
}b[10];
void HideCursor()//hiding cursor
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
void gotoxy(int x,int y)//controlling cursor
{
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X=x;
	pos.Y=y;
	SetConsoleCursorPosition(handle,pos);
}
void startup()//initializing data
{
	h=15;
	w=20;
	bx=0;
	by=w/2;
	vx=1;
	vy=1;
	r=6;
	px=h,py=w/2;
	pl=py-r;
	pr=py+r;
	num=0;
	score=0;
	for(int i=0; i<10; i++)//initializing bricks
	{
        b[i].x = rand() % (h-5) + 2;
        b[i].y = rand() % (w-2) + 1;
        b[i].active = 1;
    }
}
void show()//initializing startup surface
{
	gotoxy(0,0);
	for(int i=0;i<=h+1;i++)
	{
		for(int j=0;j<=w;j++)
		{
			int draw=0;
			for(int k=0; k<10; k++)//adding the examining process of active brick 
			{
                if(b[k].active && i == b[k].x && j == b[k].y) 
				{
                    printf("B");
                    draw= 1;
                    break;
                }
            }
            if(draw)
            {
            	continue;
			}
			if((i==bx)&&(j==by))
			{
				printf("o");
			}
			else if(j==w)
			{
				printf("|");
			}
			else if(i==h+1)
			{
				printf("-");
			}
			else if((i==h)&&(j>=pl)&&(j<=pr))
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("The number of bouncing balls: %d\n",num);
	printf("The number of disappearing blocks: %d\n",score);
}
void updatenoinput()
{
	if (bx == h)
    {
        if (by >= pl && by <= pr)
        {
            num++;
            vx = -vx;
            if (by < pl + (pr - pl) / 3) 
			{
                vy = -2;
            }
            else if (by > pl + 2 * (pr - pl) / 3) 
			{
                vy = 2;
            }
            else 
			{
                vy = 0;
            }
        }
        else
        {
            printf("Game over.\n");
            system("pause");
            exit(0);
        }
    }
	for(int i=0; i<10; i++)//adding continuing actions of ball
	{
        if(b[i].active && bx == b[i].x && by == b[i].y) 
		{
            score++;
            b[i].active = 0;
            b[i].x = rand() % (h-5) + 2;
            b[i].y = rand() % (w-2) + 1;
            b[i].active = 1;
        }
    }
	bx+=vx;
	by+=vy;
	if (bx <= 0) 
	{
        vx = -vx;
        bx = 0; //avoiding flying out
    }
    if (by <= 0 || by >= w-1) 
	{
        vy = -vy;
        by = (by <= 0) ? 0 : w-1; //adjusting position
    }

	//speed limit
	if (vy < -3) vy = -3;
    if (vy > 3) vy = 3;
	Sleep(50);
}
void updateinput()
{
	char input;
	if(kbhit())
	{
		input=getch();
		if((input=='a')||(input==75))//adding a new controling method
		{
			py--;
			if(py < r) py = r; //optimizing bouncing logic
			pl=py-r;
			pr=py+r;
		}
		if((input=='d')||(input==77))
		{
			py++;
			if(py > w-1 - r) py = w-1 - r;
			pl=py-r;
			pr=py+r;
		}
		//adding the speed control of ball
		if(input=='s' && abs(vy)>0) vy--;
        if(input=='w' && abs(vy)<3) vy++;
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
//overall optimization:
//1.adding blocks to increase the probability od shooting blocks
//2.adding actions of ball
//3.adding the examing process of bouncing ball
