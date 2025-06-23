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

// ���1����
int px1, py1;      // ���1λ��
int bx1, by1;      // ���1�ӵ�λ��
int bs1_active;     // ���1�ӵ�����״̬

// ���2����
int px2, py2;      // ���2λ��
int bx2, by2;      // ���2�ӵ�λ��
int bs2_active;     // ���2�ӵ�����״̬

// ��������
int h, w;           // ��Ϸ����ߴ�
int ex, ey;         // �л�λ��
int score = 0;      // �������
int bs_speed;       // �ӵ��ٶ�
int over;           // ��Ϸ������־

void startup() {
    h = 20;
    w = 30;
    
    // ��ʼ�����1�����½ǣ�
    px1 = h - 3;
    py1 = w / 4;
    bx1 = -1;
    by1 = py1;
    bs1_active = 0;
    
    // ��ʼ�����2�����½ǣ�
    px2 = h - 3;
    py2 = 3 * w / 4;
    bx2 = -1;
    by2 = py2;
    bs2_active = 0;
    
    // ��ʼ���л����������У�
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
            
            // �������1�ɻ�
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
            // �������2�ɻ�
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
            // ���Ƶл�
            else if ((i == ex) && (j == ey)) {
                printf("@");
                printed = 1;
            }
            // �������1�ӵ�
            else if ((i == bx1) && (j == by1) && bs1_active) {
                printf("|");
                printed = 1;
            }
            // �������2�ӵ�
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
    // �������1�ӵ�
    if (bs1_active) {
        bx1 -= bs_speed;
        if (bx1 < 0) {
            bs1_active = 0;
        }
        // ������1�ӵ����ел�
        if ((bx1 <= ex && ex <= bx1 + bs_speed) && (by1 == ey)) {
            score++;
            ex = -1;
            ey = rand() % w;
            bs1_active = 0;
            bs_speed = 1 + score / 5;
            if (bs_speed > 5) bs_speed = 5;
        }
    }
    
    // �������2�ӵ�
    if (bs2_active) {
        bx2 -= bs_speed;
        if (bx2 < 0) {
            bs2_active = 0;
        }
        // ������2�ӵ����ел�
        if ((bx2 <= ex && ex <= bx2 + bs_speed) && (by2 == ey)) {
            score++;
            ex = -1;
            ey = rand() % w;
            bs2_active = 0;
            bs_speed = 1 + score / 5;
            if (bs_speed > 5) bs_speed = 5;
        }
    }
    
    // �л��߼�
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
    
    // ��ʼ���µл�
    if (ex == -1) {
        ex = 0;
        ey = rand() % w;
    }
}

void updateinput() {
    if (_kbhit()) {
        char input = _getch();
        
        // ���1���� (WASD + �ո�)
        if (input == 'a' && py1 > 2) py1--;
        if (input == 'd' && py1 < w - 3) py1++;
        if (input == 's' && px1 < h - 3) px1++;
        if (input == 'w' && px1 > 0) px1--;
        if (input == ' ' && !bs1_active) {
            bx1 = px1 - 1;
            by1 = py1;
            bs1_active = 1;
        }
        
        // ���2���� (����� + Enter)
        if (input == 75 && py2 > 2) py2--;       // ���ͷ
        if (input == 77 && py2 < w - 3) py2++;   // �Ҽ�ͷ
        if (input == 80 && px2 < h - 3) px2++;   // �¼�ͷ
        if (input == 72 && px2 > 0) px2--;       // �ϼ�ͷ
        if (input == 13 && !bs2_active) {        // Enter��
            bx2 = px2 - 1;
            by2 = py2;
            bs2_active = 1;
        }
        
        // �˳���Ϸ (ESC)
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
        Sleep(50); // ������Ϸ�ٶ�
    }
    
    system("cls");
    gotoxy(10, 10);
    printf("Game over. Final score: %d\n", score);
    Sleep(2000);
    return 0;
}
