#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

void HideCursor() {
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

int px1, py1;     
int bx1, by1;      
int bs1_active;     

int px2, py2;
int bx2, by2;
int bs2_active;

// 公共变量
int h, w;           // 游戏区域尺寸
int ex, ey;         // 敌机位置
int score = 0;      // 共享分数
int bs_speed;       // 子弹速度
int over;           // 游戏结束标志

void startup() {
    h = 20;
    w = 30;
    
    // 初始化玩家1（左下角）
    px1 = h - 3;
    py1 = w / 4;
    bx1 = -1;
    by1 = py1;
    bs1_active = 0;
    
    // 初始化玩家2（右下角）
    px2 = h - 3;
    py2 = 3 * w / 4;
    bx2 = -1;
    by2 = py2;
    bs2_active = 0;
    
    // 初始化敌机（顶部居中）
    ex = 0;
    ey = w / 2;
    
    bs_speed = 1;
    over = 0;
}

void show() {
    gotoxy(0, 0);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int printed = 0;
            
            // 绘制玩家1飞机
            if (i == px1 && j == py1 - 2) {
                printf("  *  ");
                j += 4;
                printed = 1;
            } else if (i == px1 + 1 && j == py1 - 2) {
                printf("*****");
                j += 4;
                printed = 1;
            } else if (i == px1 + 2 && j == py1 - 1) {
                printf("* * ");
                j += 3;
                printed = 1;
            }
            // 绘制玩家2飞机
            else if (i == px2 && j == py2 - 2) {
                printf("  +  ");
                j += 4;
                printed = 1;
            } else if (i == px2 + 1 && j == py2 - 2) {
                printf("+++++");
                j += 4;
                printed = 1;
            } else if (i == px2 + 2 && j == py2 - 1) {
                printf("+ + ");
                j += 3;
                printed = 1;
            }
            // 绘制敌机
            else if ((i == ex) && (j == ey)) {
                printf("@");
                printed = 1;
            }
            // 绘制玩家1子弹
            else if ((i == bx1) && (j == by1) && bs1_active) {
                printf("|");
                printed = 1;
            }
            // 绘制玩家2子弹
            else if ((i == bx2) && (j == by2) && bs2_active) {
                printf("!");
                printed = 1;
            }
            
            if (!printed) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void updatenoinput() {
    // 更新玩家1子弹
    if (bs1_active) {
        bx1 -= bs_speed;
        if (bx1 < 0) {
            bs1_active = 0;
        }
        // 检测玩家1子弹击中敌机
        if ((bx1 <= ex && ex <= bx1 + bs_speed) && (by1 == ey)) {
            score++;
            ex = -1;
            ey = rand() % w;
            bs1_active = 0;
            bs_speed = 1 + score / 5;
            if (bs_speed > 5) bs_speed = 5;
        }
    }
    
    // 更新玩家2子弹
    if (bs2_active) {
        bx2 -= bs_speed;
        if (bx2 < 0) {
            bs2_active = 0;
        }
        // 检测玩家2子弹击中敌机
        if ((bx2 <= ex && ex <= bx2 + bs_speed) && (by2 == ey)) {
            score++;
            ex = -1;
            ey = rand() % w;
            bs2_active = 0;
            bs_speed = 1 + score / 5;
            if (bs_speed > 5) bs_speed = 5;
        }
    }
    
    // 敌机逻辑
    if (ex > h) {
        ex = -1;
        ey = rand() % w;
    }
    
    static int ve = 0;
    if (ve < 10) {
        ve++;
    }
    if (ve == 10) {
        ex++;
        ve = 0;
    }
    
    // 初始化新敌机
    if (ex == -1) {
        ex = 0;
        ey = rand() % w;
    }
}

void updateinput() {
    if (_kbhit()) {
        char input = _getch();
        
        // 玩家1控制 (WASD + 空格)
        if (input == 'a' && py1 > 2) py1--;
        if (input == 'd' && py1 < w - 3) py1++;
        if (input == 's' && px1 < h - 3) px1++;
        if (input == 'w' && px1 > 0) px1--;
        if (input == ' ' && !bs1_active) {
            bx1 = px1 - 1;
            by1 = py1;
            bs1_active = 1;
        }
        
        // 玩家2控制 (方向键 + Enter)
        if (input == 75 && py2 > 2) py2--;       // 左箭头
        if (input == 77 && py2 < w - 3) py2++;   // 右箭头
        if (input == 80 && px2 < h - 3) px2++;   // 下箭头
        if (input == 72 && px2 > 0) px2--;       // 上箭头
        if (input == 13 && !bs2_active) {        // Enter键
            bx2 = px2 - 1;
            by2 = py2;
            bs2_active = 1;
        }
        
        // 退出游戏 (ESC)
        if (input == 27) over = 1;
    }
}

void displayScore(int score) {
    printf("==============================\n");
    printf("   DUAL PLAYER | SCORE: %04d \n", score);
    printf("==============================\n");
    printf(" Player1: WASD+SPACE\n Player2: Arrow+ENTER\n");
    printf("==============================\n\n");
}

int main() {
    HideCursor();
    startup();
    while (!over) {
        show();
        updatenoinput();
        updateinput();
        displayScore(score);
        Sleep(50); // 控制游戏速度
    }
    
    system("cls");
    gotoxy(10, 10);
    printf("Game over. Final score: %d\n", score);
    Sleep(2000);
    return 0;
}
