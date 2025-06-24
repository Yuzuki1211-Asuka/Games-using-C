#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

#define PIPE_COUNT 3
#define PIPE_GAP 4

int h, w;
float bx; 
int by;
int ox1[PIPE_COUNT], ox2[PIPE_COUNT], oy[PIPE_COUNT];  //using arrays to better store
int score;
float velocity = 0;
const float gravity = 0.15;
const float jumpForce = -3.0;

void HideCursor() 
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}

void startup() {
    h = 15;
    w = 40;//widen playing area
    bx = h / 2;
    by = w / 4;
    for (int i = 0; i < PIPE_COUNT; i++) 
	{
        oy[i] = w / 2 + i * (w / PIPE_COUNT); //distributing obstacles
        ox1[i] = 3 + rand() % (h - PIPE_GAP - 3);
        ox2[i] = ox1[i] + PIPE_GAP;
    }
    
    velocity = 0;
    score = 0;
}

void show() 
{
    gotoxy(0, 0);
    int roundedBx = (int)(bx + 0.5);
    
    for (int i = 0; i < h; i++) 
	{
        printf("|");
        for (int j = 0; j < w; j++) 
		{
            // 绘制小鸟
            if ((i == roundedBx) && (j == by)) 
			{
                printf("@");
            } 
            // 绘制柱子
            else 
			{
                int pipeDrawn = 0;
                for (int k = 0; k < PIPE_COUNT; k++) 
				{
                    if ((j == oy[k]) && ((i < ox1[k]) || (i > ox2[k]))) 
					{
                        printf("*");
                        pipeDrawn = 1;
                        break;
                    }
                }
                if (!pipeDrawn) 
				{
                    printf(" ");
                }
            }
        }
        printf("|\n");
    }
    
    //down edge
    for (int j = 0; j < w + 2; j++) 
	{
        printf("-");
    }
    printf("\n");
    printf("Score: %d\n", score);
}

void updatenoinput() 
{
    //applying gravity
    velocity += gravity;
    bx += velocity;
    
    //edge detecting
    if (bx < 0) 
	{
        bx = 0;
        velocity = 0;
    }
    if (bx >= h - 1) 
	{
        bx = h - 1;
        velocity = 0;
    }

    for (int i = 0; i < PIPE_COUNT; i++) 
	{
    	
        oy[i]--;
        
        //collision detecting
        if (oy[i] == by) 
		{
            if (bx < ox1[i] || bx > ox2[i]) 
			{
                for (int flash = 0; flash < 3; flash++) 
				{
                    system("cls");
                    gotoxy(h/2, w/2);
                    printf("GAME OVER! Score: %d", score);
                    Sleep(200);
                    system("cls");
                    show();
                    Sleep(200);
                }
                gotoxy(h+3, 0);
                printf("Press any key to exit...");
                getch();
                exit(0);
            } else {
                score++;
            }
        }
        if (oy[i] <= 0) 
		{
            oy[i] = w;
            ox1[i] = 3 + rand() % (h - PIPE_GAP - 3);
            ox2[i] = ox1[i] + PIPE_GAP;
        }
    }
    
    Sleep(100);
}

void updateinput() 
{
    if (_kbhit()) 
	{
        char input = _getch();
        if (input == ' ') 
		{
            velocity = jumpForce;
        }
    }
}

int main() 
{
    startup();
    system("cls");
    HideCursor();
    while (1) 
	{
        updateinput();
        updatenoinput();
        show();
    }
    
    return 0;
}
//overall optimization:
//1.simulating a real-world scenario, where gravity is considered
//2.using array to better display more obstacles
