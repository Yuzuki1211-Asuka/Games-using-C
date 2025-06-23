#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
int main()
{
	int x=0,y=5;
	int vx=1,vy=1;
	int l=0,r=20;
	int t=0,b=10;
	while(1)
	{
		x+=vx;
		y+=vy;
		system("cls");
		for(int j=0;j<22;j++)
		{
			printf("#");
		}
		printf("\n");
        for(int i = 0; i <= b; i++)
        {
            printf("#");
            for(int j = 0; j <= r; j++)
            {
                if(i == x && j == y)
                {
                    printf("¡ñ");
                }
                else
                {
                    printf(" ");
                }
            }
            printf("#\n");
        }
		for(int j = 0; j<22; j++)
        {
            printf("#");
        }
        printf("\n");
		for(long k=0; k<50000000; k++) {}
		if((x==t)||(x==b))
		{
			vx=-vx;
		}
		if((y==l)||(y==r))
		{
			vy=-vy;
		}
	}
	return 0;
}
