#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include <time.h>
#define h 20
#define w 30
int d;
int c[h][w]={0};//designing a simulated canva
int di;//direction
int fx,fy;//position of food
int score=0;
int live=5;//initializing life value
int over=0;
int prop_x, prop_y;//position of props
int prop_type = 0;//the type of props
int prop_timer = 0;//span of props
int food_count = 0;//counter
int v= 100;//speed
int sp= 0;//time/speed span
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
void reset()//reset the state of the game
{
	for(int i=1;i<h-1;i++)
	{
		for(int j=1;j<w-1;j++)
		{
			c[i][j]=0;
		}
	}
	c[h/2][w/2]=1;
	for(int i=1;i<=4;i++)
	{
		c[h/2][w/2-i]=i+1;
	}
	di=4;
	do
	{
		fx=rand()%(h-5)+2;
		fy=rand()%(w-5)+2;
	}while(c[fx][fy]!=0);
	c[fx][fy]=-3;
	food_count = 0;
    prop_type = 0;
    prop_timer = 0;
    int v= 100;//speed
	int sp= 0;//time(speed) span
}
void move()
{
	if(sp>0)
	{
		sp--;
		if(sp==0)
		{
			v=100;
		}
	}
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
	//going through the wall
	if (h1 == 0) h1 = h - 2;//up->down
	else if (h1 == h-1) h1 = 1;//down-up
	if (h2 == 0) h2 = w - 2;//left->right
	else if (h2 == w-1) h2 = 1;//right->left
	if(c[h1][h2]<0)//optimizing the overall logic of prop detection 
	{
		if (c[h1][h2] == -4)
		{
		    live++;
	        prop_type = 0;//remove the prop
		}
		else if (c[h1][h2] == -5) 
		{
		    live = (live > 1) ? live - 1 : 0;//keep the minimum limit
            prop_type = 0;
            if (live <= 0) over = 1;
		}
		else if (c[h1][h2] == -6) 
		{
		    v= (v> 40) ? v- 30 : 40;
            sp= 30;
		}
        else if (c[h1][h2] == -7) 
		{
            v= (v> 40) ? v+30:200;
            sp=30;
			}
		prop_type = 0;
	}
    //renew timer
    if (prop_type != 0) 
	{
        prop_timer--;
        if (prop_timer <= 0) 
		{
            c[prop_x][prop_y] = 0;//remove expired prop
            prop_type = 0;
        }
    }
	if(c[h1][h2]==-3)//eat the food
	{
		c[fx][fy]=0;
		fx=rand()%(h-5)+2;//->
		fy=rand()%(w-5)+2;
		c[fx][fy]=-3;//->generating a new food(?
		score++;
		food_count++;
		if (food_count >= 5 && prop_type == 0) 
		{
            food_count = 0; 
            do 
			{
                prop_x = rand() % (h - 5) + 2;
                prop_y = rand() % (w - 5) + 2;
            } while (c[prop_x][prop_y] != 0);
            //increase the randomness of four props
            int r=rand()%100;
            if(r<35)  prop_type=-4;
            else if(r>=35&&r<70)  prop_type=-5;
            else if(r>=70&&r<84)  prop_type=-6;
            else  prop_type=-7;
            prop_timer = 25;
            c[prop_x][prop_y] = prop_type;
        }
	}
	else
	{
		c[ti][tj]=0;
	}
	if(c[h1][h2]>0||c[h1][h2]==-1||c[h1][h2]==-2)
	{
		live--;
		if(live==0)
		{
			over=1;
		}
		else
		{
			reset();
		}
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
	//from line 146 to 154, all can be considered as reset()
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
			else if(c[i][j]==-4)
			{
				printf("H");
			}
			else if(c[i][j]==-5)
			{
				printf("D");
			}
			else if(c[i][j]==-6)
			{
				printf("+");
			}
			else if(c[i][j]==-7)
			{
				printf("-");
			}
		}
		printf("\n");
	}
	printf("Score: %d   lives: %d\n", score,live);
	if(over)
	{
		printf("Game Over! Final Score: %d\n", score);
	}
	Sleep(100);
}
void updatenoinput()
{
	if(!over)
	{
		move();
	}
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
		if(input==27)
		{
			printf("Game over\n");
			exit(0);
		}
	}
}
int main()
{
	srand(time(0));
	HideCursor();
	startup();
	while(1)
	{
		if(over)
		{
			show();
			break;
		}
		show();
		updatenoinput();
		updateinput();
	}
	return 0;
}
//overall optimization:
//1.adding the mechanism of live value
//2.designing props that can add/minus live value
//3.optimizing the logic that the body can get longer if food taken
//4.designing props that can add/minus speed(-6 v++,-7 v--) 
