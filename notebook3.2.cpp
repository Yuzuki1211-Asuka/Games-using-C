#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>
#include<time.h> 

#define MAX_COINS 10
#define BRICK_TYPES 3 // 砖块类型数量 ★

int h,w;
int bx,by;
int vx,vy;
int px,py; // center of plank
int r;     // radius of plank
int pl,pr; // left and right side of plank
int num;   // times of bouncing balls
int score;
int lives;
int ballActive;
struct coin {
    int x;
    int y;
    int active;
    int value;
};

//add lives to brick
struct brick
{
    int x;
    int y;
    int active;
    int hp;
    int max_hp;
    int type;
} b[10];

struct coin coins[MAX_COINS];
void SetColor(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void HideCursor()
{
    CONSOLE_CURSOR_INFO cursor_info = {1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}

void resetBall() 
{
    bx = 0;
    by = w/2;
    vx = 1;
    vy = 1;
    ballActive = 1;
}

void initCoins() {
    for (int i = 0; i < MAX_COINS; i++) {
        coins[i].active = 0;
        coins[i].value = 5;
    }
}

void spawnCoin(int x, int y) {
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].active) {
            coins[i].x = x;
            coins[i].y = y;
            coins[i].active = 1;
            return;
        }
    }
}

void startup() 
{
    srand(time(NULL));
    
    h = 15;
    w = 20;
    r = 6;
    lives = 3;
    score = 0;
    num = 0;
    ballActive = 1;
    
    resetBall();
    
    px = h;
    py = w/2;
    pl = py - r;
    pr = py + r;
    
    initCoins();
    for(int i = 0; i < 10; i++) 
    {
        b[i].x = rand() % (h-5) + 2;
        b[i].y = rand() % (w-2) + 1;
        b[i].active = 1;
        b[i].type = rand() % BRICK_TYPES;
        switch(b[i].type) {
            case 0: // 普通砖块 - 1次击碎
                b[i].hp = 1;
                b[i].max_hp = 1;
                break;
            case 1: // 中等砖块 - 2次击碎
                b[i].hp = 2;
                b[i].max_hp = 2;
                break;
            case 2: // 坚硬砖块 - 3次击碎
                b[i].hp = 3;
                b[i].max_hp = 3;
                break;
        }
    }
}

void show() 
{
    gotoxy(0,0);
    for(int i = 0; i <= h+1; i++)
    {
        for(int j = 0; j <= w; j++)
        {
            int draw = 0;
            for(int k = 0; k < 10; k++) 
            {
                if(b[k].active && i == b[k].x && j == b[k].y) 
                {
                	//editing brick features
                    switch(b[k].type) {
                        case 0:
                            SetColor(10);
                            break;
                        case 1:
                            SetColor(14);
                            break;
                        case 2:
                            SetColor(12);
                            break;
                    }
                    if (b[k].hp == 1) printf("1");
                    else if (b[k].hp == 2) printf("2");
                    else if (b[k].hp == 3) printf("3");
                    else printf("B");
                    
                    SetColor(7);
                    draw = 1;
                    break;
                }
            }
            if(draw) continue;
            for (int c = 0; c < MAX_COINS; c++) 
			{
                if (coins[c].active && i == coins[c].x && j == coins[c].y) {
                	
                    SetColor(14); 
                    printf("$");
                    SetColor(7);
                    draw = 1;
                    break;
                }
            }
            if(draw) continue;
            
            if(ballActive && (i == bx) && (j == by)) 
            {
                printf("o");
            }
            else if(j == w)
            {
                printf("|");
            }
            else if(i == h+1)
            {
                printf("-");
            }
            else if((i == h) && (j >= pl) && (j <= pr))
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
    SetColor(10); printf("1:Normal "); 
    SetColor(14); printf("2:Medium "); 
    SetColor(12); printf("3:Hard");
    SetColor(7);
    printf("\n");
    
    printf("Bounces: %d  Blocks: %d  Lives: %d  Score: %d\n", num, score, lives, score);
}

void updatenoinput()
{
    for (int i = 0; i < MAX_COINS; i++) {
        if (coins[i].active) {
            coins[i].x++;
            
            if (coins[i].x == h && coins[i].y >= pl && coins[i].y <= pr) {
                score += coins[i].value;
                coins[i].active = 0;
            }
            else if (coins[i].x > h+1) {
                coins[i].active = 0;
            }
        }
    }
    
    if (ballActive)
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
                lives--;
                ballActive = 0;
                if (lives <= 0) 
                {
                    system("cls");
                    printf("\n\n   GAME OVER!\n");
                    printf("   Final Score: %d\n\n", score);
                    exit(0);
                }
            }
        }
        for(int i = 0; i < 10; i++)
        {
            if(b[i].active && bx == b[i].x && by == b[i].y) 
            {
                b[i].hp--;
                switch(b[i].type) {
                    case 0: score += 1; break;
                    case 1: score += 2; break;
                    case 2: score += 3; break;
                }
                if (b[i].hp <= 0) 
                {
                    b[i].active = 0;
                    spawnCoin(b[i].x, b[i].y);
                    
                    // regenerating bricks
                    b[i].x = rand() % (h-5) + 2;
                    b[i].y = rand() % (w-2) + 1;
                    b[i].active = 1;
                    //new brick type
                    b[i].type = rand() % BRICK_TYPES;                  
                    //editing lives
                    switch(b[i].type) {
                        case 0: 
                            b[i].hp = 1;
                            b[i].max_hp = 1;
                            break;
                        case 1: 
                            b[i].hp = 2;
                            b[i].max_hp = 2;
                            break;
                        case 2: 
                            b[i].hp = 3;
                            b[i].max_hp = 3;
                            break;
                    }
                }
                vx = -vx;
                break;
            }
        }
        
        bx += vx;
        by += vy;
        
        if (bx <= 0) 
        {
            vx = -vx;
            bx = 0;
        }
        if (by <= 0 || by >= w-1) 
        {
            vy = -vy;
            by = (by <= 0) ? 0 : w-1;
        }

        // speed limit
        if (vy < -3) vy = -3;
        if (vy > 3) vy = 3;
    }
    
    Sleep(50);
}

void updateinput()
{
    char input;
    if(kbhit())
    {
        input = getch();
        if((input == 'a') || (input == 75)) 
        {
            py--;
            if(py < r) py = r;
            pl = py - r;
            pr = py + r;
        }
        if((input == 'd') || (input == 77))
        {
            py++;
            if(py > w-1 - r) py = w-1 - r;
            pl = py - r;
            pr = py + r;
        }
        if(input == 's' && abs(vy) > 0) vy--;
        if(input == 'w' && abs(vy) < 3) vy++;
        if(input == ' ') 
        {
            if (!ballActive && lives > 0) 
            {
                resetBall();
            }
        }
        if(input == 27)
        {
            exit(0);
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
//overall optimization:
//1.adding the "live" display of ball
//2.if' 'pressed, new ball generates
//3.if a block shot down, coin generates
//4.added block features:shot down only many times
