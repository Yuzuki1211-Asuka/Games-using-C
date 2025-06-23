#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
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
int px,py;
int h,w;
int bx,by;
int ex,ey;
int score=0;
int bs;
int over;
void startup()//initializing startup surface
{
	h=20;
	w=30;
	px=h/2;
	py=w/2;
	bx=-1;
	by=py;
	ex=0;
	ey=py;
	bs=1;
}
void show()//displaying real surface and optimizing the whole logic of displaying plane
{
    gotoxy(0, 0);
    for (int i = 0; i < h; i++) 
	{
        for (int j = 0; j < w; j++) 
		{
            if (i == px && j == py - 2) 
			{
                printf("  *  ");
                j += 4;
            } 
            else if (i == px + 1 && j == py - 2) 
			{
                printf("*****");
                j += 4;
            } 
            else if (i == px + 2 && j == py - 1) 
			{
                printf("* * ");
                j += 4;
            }
            else if ((i == ex) && (j == ey)) 
			{
                printf("@");
            }
            else if ((i == bx) && (j == by)) 
			{
                printf("|");
            }
            else 
			{
                printf(" ");
            }
        }
        printf("\n");
    }
}

void updatenoinput()//updating data without input
{
	if(bx>-1)
	{
		bx-=bs;
	}
	if((bx<=ex&&ex<=bx+bs)&&(by==ey))//shooting at the enemy
	{
		score++;
		ex=-1;//generating new plane
		ey=rand()%w;
		bx=-2;//inactivate the bullet
		bs = 1 + score / 5; 
        if (bs> 5) bs= 5;
	}
	if(ex>h)//the enemy flying out-- 
	{
		ex=-1;
		ey=rand()%w;
	}
	static int ve=0;//move only once after 10 loops
	if(ve<10)
	{
		ve++;
	}
	if(ve==10)
	{
		ex++;
		ve=0;
	}
}
void updateinput()//same as "void move()"
{
	char input;
	if(kbhit())
		{
			input=getch();
			if(input=='a'&&py > 2)
			{
				py--;
			}
			if(input=='d'&&py<w-3)
			{
				py++;
			}
			if(input=='s'&&px<h-3)
			{
				px++;
			}
			if(input=='w'&&px>0)
			{
				px--;
			}
			if(input==' ')
			{
				bx=px-1;
				by=py;
			}
			if (input == 27) 
			{
			    over = 1;
			}
		}
}
void displayScore(int score)
{
    printf("==============================\n");
    printf("          SCORE: %04d     \n", score);
    printf("==============================\n\n");
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
		displayScore(score);
		if (over) 
		{
            system("cls");
            gotoxy(10, 10);
            printf("Game over. Final score: %d\n", score);
            Sleep(2000);
            break;
        }
	}
	return 0;
}
//overall optimization:
//1.modifying the appearance of plane(from a single *)
//2.adding the examining process of edges(line 108-120),and avoiding plane flying out
//3.added feature:with enemy shot down, enemy gets faster--
