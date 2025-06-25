//a complicated plane
int plane(int y)
{
	for(int j=0;j<y;j++)
		{
			printf(" ");
		}
		printf("  *\n");
		for(int j=0;j<y;j++)
		{
			printf(" ");
		}
		printf("*****\n");
		for(int j=0;j<y;j++)
		{
			printf(" ");
		}
		printf(" * * \n");
}
//moving plane
void move(int *x,int *y,char input)
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
		}
}
//moving plane with laser
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
//creating laser
int laser(int *fire,int *x,int *y)
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
			fire=0;
		}
}
//controlling cursor
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
//end the game if esc pressed
if(input==27)
{
	printf("Game over\n");
	exit(0);
}
