#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define H 20
#define W 30
#define MAX_BULLETS 10      // �����ӵ�������֧��ɢ��
#define MAX_ENEMIES 5       // ���л�����
#define ENEMY_SPAWN_RATE 30 // �л�����Ƶ��
#define MAX_SHOT_WIDTH 5    // ����ӵ����
// ���ع��
void HideCursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
// �ƶ���굽ָ��λ��
void gotoxy(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(handle, pos);
}
// �ӵ��ṹ��
typedef struct {
    int x, y;
    int active;
} Bullet;
// �л��ṹ��
typedef struct {
    int x, y;
    int active;
    int speed; // ÿ�ܵл����Լ����ٶ�
} Enemy;
// ��Ϸ״̬����
int px, py;         // ���λ��
Bullet bullets[MAX_BULLETS]; // �ӵ�����
Enemy enemies[MAX_ENEMIES];  // �л�����
int score = 0;      // �÷�
int over = 0;       // ��Ϸ������־
int lives = 3;      // �������ֵ
int spawn_timer = 0; // �л����ɼ�ʱ��
int shot_width = 1; // ��ǰ�ӵ����
int shot_level = 0; // �ӵ��ȼ�
int enemy_base_speed = 1; // �л������ٶ�
// ��Ϸ��������
char map[H][W];
// ��ʼ����Ϸ
void startup() {
    px = H - 3;
    py = W / 2;
    score = 0;
    shot_width = 1;
    shot_level = 0;
    enemy_base_speed = 1;
    
    // ��ʼ�������ӵ�Ϊδ����״̬
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
    
    // ��ʼ�����ел�Ϊδ����״̬
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }
}

// �����Ϸ����
void clearMap() {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (i == 0 || i == H - 1) {
                map[i][j] = '-'; // ���±߽�
            } else if (j == 0 || j == W - 1) {
                map[i][j] = '|'; // ���ұ߽�
            } else {
                map[i][j] = ' ';
            }
        }
    }
}

// ������ҷɻ�
void drawPlayer() {
    // ȷ���ɻ��ڱ߽���
    if (px >= 0 && px < H && py >= 2 && py < W - 2) {
        // ���Ʒɻ�ͼ��
        map[px][py] = 'A';
        map[px][py - 1] = '<';
        map[px][py + 1] = '>';
        map[px - 1][py] = '^';
        
        // ���ƻ���װ��
        if (py > 1 && py < W - 2) {
            map[px - 1][py - 2] = shot_level >= 2 ? '=' : ' ';
            map[px - 1][py + 2] = shot_level >= 2 ? '=' : ' ';
        }
    }
}

// �����ӵ�
void drawBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            int bx = bullets[i].x;
            int by = bullets[i].y;
            
            if (bx >= 1 && bx < H - 1 && by >= 1 && by < W - 1) {
                // ���Ʋ�ͬ��ȵ��ӵ�
                for (int w = 0; w < shot_width; w++) {
                    int offset = w - shot_width / 2;
                    if (by + offset >= 1 && by + offset < W - 1) {
                        map[bx][by + offset] = '|';
                    }
                }
            }
        }
    }
}

// ���Ƶ���
void drawEnemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            int ex = enemies[i].x;
            int ey = enemies[i].y;
            if (ex >= 1 && ex < H - 1 && ey >= 1 && ey < W - 1) {
                // ���Ƶл�
                map[ex][ey] = '@';
                map[ex + 1][ey - 1] = '/';
                map[ex + 1][ey + 1] = '\\';
                map[ex + 1][ey] = 'V';
                
                // �����ٶȻ��Ʋ�ͬ��ɫ�ĵл�
                if (enemies[i].speed > 3) {
                    map[ex][ey] = 'X'; // ���ٵл�
                } else if (enemies[i].speed > 2) {
                    map[ex][ey] = '#'; // ���ٵл�
                }
            }
        }
    }
}

// ��Ⱦ��Ϸ����
void show() {
    clearMap();         // ��ջ���
    drawPlayer();       // �������
    drawBullet();       // �����ӵ�
    drawEnemy();        // ���Ƶ���
    
    // ��ʾ��Ϸ��Ϣ
    gotoxy(0, H);
    printf("Score: %04d   Lives: %d   Shot: %d   Enemy Speed: %d", 
           score, lives, shot_width, enemy_base_speed);
    
    // ����״̬��
    gotoxy(0, H + 1);
    printf("[");
    int barWidth = 20;
    int pos = barWidth * lives / 3;
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) printf("=");
        else printf(" ");
    }
    printf("]");
    
    // ��ӡ��������
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            putchar(map[i][j]);
        }
        putchar('\n');
    }
}

// �����µл�
void spawnEnemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].x = 1; // �Ӷ�������
            enemies[i].y = rand() % (W - 4) + 2; // ȷ���л������ڱ߽���
            enemies[i].active = 1;
            
            // ���ݷ������õл��ٶȣ������ٶ� + ���ƫ�ƣ�
            enemies[i].speed = enemy_base_speed + rand() % (1 + score / 200);
            if (enemies[i].speed > 6) enemies[i].speed = 6;
            
            return;
        }
    }
}

// ����ɢ��
void fireShotgun() {
    int bullets_fired = 0;
    
    // �м��ӵ�
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = px - 1;
            bullets[i].y = py;
            bullets[i].active = 1;
            bullets_fired++;
            break;
        }
    }
    
    // ����ӵ�
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = px - 1;
            bullets[i].y = py - 2;
            bullets[i].active = 1;
            bullets_fired++;
            break;
        }
    }
    
    // �ұ��ӵ�
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = px - 1;
            bullets[i].y = py + 2;
            bullets[i].active = 1;
            bullets_fired++;
            break;
        }
    }
    
    // ��������㹻�ߣ��������������ӵ�
    if (shot_level >= 2) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) {
                bullets[i].x = px - 1;
                bullets[i].y = py - 4;
                bullets[i].active = 1;
                bullets_fired++;
                break;
            }
        }
        
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) {
                bullets[i].x = px - 1;
                bullets[i].y = py + 4;
                bullets[i].active = 1;
                bullets_fired++;
                break;
            }
        }
    }
}

// ������Ϸ״̬�������룩
void updatenoinput() {
    // �ƶ����м�����ӵ�
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x -= 1; // �ӵ��ٶȹ̶�Ϊ1
            
            // ����ӵ��Ƴ���Ļ�Ϸ���������Ϊδ����
            if (bullets[i].x <= 0) {
                bullets[i].active = 0;
            }
            
            // ����ӵ����е���
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (bullets[i].active && enemies[j].active) {
                    // �����ӵ�����ײ
                    int hit = 0;
                    for (int w = 0; w < shot_width; w++) {
                        int offset = w - shot_width / 2;
                        if (bullets[i].x == enemies[j].x && 
                            bullets[i].y + offset == enemies[j].y) {
                            hit = 1;
                            break;
                        }
                    }
                    
                    if (hit) {
                        score += 10;
                        bullets[i].active = 0; // ��������ӵ�
                        enemies[j].active = 0;  // ���õл�
                        
                        // �����ӵ���Ⱥ͵ȼ�
                        if (score >= 100 && shot_level == 0) {
                            shot_width = 3;
                            shot_level = 1;
                        } else if (score >= 200 && shot_level == 1) {
                            shot_width = 5;
                            shot_level = 2;
                        }
                        
                        // ���ӵл������ٶ�
                        enemy_base_speed = 1 + score / 100;
                        if (enemy_base_speed > 5) enemy_base_speed = 5;
                        
                        break;
                    }
                }
            }
        }
    }
    
    // �л��ƶ�����ײ���
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // �л��ƶ����ٶ�Խ���ƶ�Ƶ��Խ�ߣ�
            static int ve = 0;
            ve++;
            if (ve >= 10 / enemies[i].speed) {
                enemies[i].x += 1;
                ve = 0;
            }
            
            // ���л������߽�
            if (enemies[i].x >= H - 1) {
                enemies[i].active = 0;  // ���õл�
                lives--; // ���ʧȥһ������
                if (lives <= 0) {
                    over = 1;
                }
            }
            
            // �������������ײ
            if (enemies[i].active) {
                int collision = 0;
                // �����ҷɻ���������
                if (enemies[i].x == px && enemies[i].y == py) collision = 1;
                if (enemies[i].x == px && enemies[i].y == py - 1) collision = 1;
                if (enemies[i].x == px && enemies[i].y == py + 1) collision = 1;
                if (enemies[i].x == px - 1 && enemies[i].y == py) collision = 1;
                
                if (collision) {
                    enemies[i].active = 0; // ���õл�
                    lives--;
                    if (lives <= 0) {
                        over = 1;
                    } else {
                        // �����к���ʱʧȥǿ��
                        shot_width = 1;
                        shot_level = 0;
                    }
                }
            }
        }
    }
    
    // �����µл�
    spawn_timer++;
    if (spawn_timer >= ENEMY_SPAWN_RATE) {
        spawnEnemy();
        spawn_timer = 0;
    }
}

void updateinput() {
    if (_kbhit()) {
        char input = _getch();
        switch (input) {
            case 'a': if (py > 2) py--; break;
            case 'd': if (py < W - 3) py++; break;
            case 's': if (px < H - 2) px++; break;
            case 'w': if (px > 1) px--; break;
            case ' ': 
                // �����ӵ��ȼ��������䷽ʽ
                if (shot_level >= 1) {
                    fireShotgun(); // ɢ��ģʽ
                } else {
                    // �����ӵ�
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].x = px - 1;
                            bullets[i].y = py;
                            bullets[i].active = 1;
                            break;
                        }
                    }
                }
                break;
            case 27: over = 1; break;  // ESC�˳�
        }
    }
}

void gameOverScreen() {
    system("cls");
    gotoxy(8, 10);
    printf("====================================\n");
    gotoxy(9, 10);
    printf("           G A M E   O V E R        \n");
    gotoxy(10, 10);
    printf("         Final Score: %04d         \n", score);
    printf("====================================\n");
    gotoxy(14, 10);
    printf("   Press any key to exit...");
    _getch();
}

int main() {
    srand(time(0));
    HideCursor();
    startup();
    
    while (!over) {
        show();
        updatenoinput();
        updateinput();
        Sleep(50); // ������Ϸ�ٶ�
    }
    gameOverScreen();
    return 0;
}
