#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define H 20
#define W 30
#define MAX_BULLETS 10      // 增加子弹数量以支持散弹
#define MAX_ENEMIES 5       // 最大敌机数量
#define ENEMY_SPAWN_RATE 30 // 敌机生成频率
#define MAX_SHOT_WIDTH 5    // 最大子弹宽度
// 隐藏光标
void HideCursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
// 移动光标到指定位置
void gotoxy(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(handle, pos);
}
// 子弹结构体
typedef struct {
    int x, y;
    int active;
} Bullet;
// 敌机结构体
typedef struct {
    int x, y;
    int active;
    int speed; // 每架敌机有自己的速度
} Enemy;
// 游戏状态变量
int px, py;         // 玩家位置
Bullet bullets[MAX_BULLETS]; // 子弹数组
Enemy enemies[MAX_ENEMIES];  // 敌机数组
int score = 0;      // 得分
int over = 0;       // 游戏结束标志
int lives = 3;      // 玩家生命值
int spawn_timer = 0; // 敌机生成计时器
int shot_width = 1; // 当前子弹宽度
int shot_level = 0; // 子弹等级
int enemy_base_speed = 1; // 敌机基础速度
// 游戏画面数组
char map[H][W];
// 初始化游戏
void startup() {
    px = H - 3;
    py = W / 2;
    score = 0;
    shot_width = 1;
    shot_level = 0;
    enemy_base_speed = 1;
    
    // 初始化所有子弹为未激活状态
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
    
    // 初始化所有敌机为未激活状态
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }
}

// 清空游戏画面
void clearMap() {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (i == 0 || i == H - 1) {
                map[i][j] = '-'; // 上下边界
            } else if (j == 0 || j == W - 1) {
                map[i][j] = '|'; // 左右边界
            } else {
                map[i][j] = ' ';
            }
        }
    }
}

// 绘制玩家飞机
void drawPlayer() {
    // 确保飞机在边界内
    if (px >= 0 && px < H && py >= 2 && py < W - 2) {
        // 绘制飞机图形
        map[px][py] = 'A';
        map[px][py - 1] = '<';
        map[px][py + 1] = '>';
        map[px - 1][py] = '^';
        
        // 绘制机翼装饰
        if (py > 1 && py < W - 2) {
            map[px - 1][py - 2] = shot_level >= 2 ? '=' : ' ';
            map[px - 1][py + 2] = shot_level >= 2 ? '=' : ' ';
        }
    }
}

// 绘制子弹
void drawBullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            int bx = bullets[i].x;
            int by = bullets[i].y;
            
            if (bx >= 1 && bx < H - 1 && by >= 1 && by < W - 1) {
                // 绘制不同宽度的子弹
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

// 绘制敌人
void drawEnemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            int ex = enemies[i].x;
            int ey = enemies[i].y;
            if (ex >= 1 && ex < H - 1 && ey >= 1 && ey < W - 1) {
                // 绘制敌机
                map[ex][ey] = '@';
                map[ex + 1][ey - 1] = '/';
                map[ex + 1][ey + 1] = '\\';
                map[ex + 1][ey] = 'V';
                
                // 根据速度绘制不同颜色的敌机
                if (enemies[i].speed > 3) {
                    map[ex][ey] = 'X'; // 快速敌机
                } else if (enemies[i].speed > 2) {
                    map[ex][ey] = '#'; // 中速敌机
                }
            }
        }
    }
}

// 渲染游戏画面
void show() {
    clearMap();         // 清空画面
    drawPlayer();       // 绘制玩家
    drawBullet();       // 绘制子弹
    drawEnemy();        // 绘制敌人
    
    // 显示游戏信息
    gotoxy(0, H);
    printf("Score: %04d   Lives: %d   Shot: %d   Enemy Speed: %d", 
           score, lives, shot_width, enemy_base_speed);
    
    // 绘制状态条
    gotoxy(0, H + 1);
    printf("[");
    int barWidth = 20;
    int pos = barWidth * lives / 3;
    for (int i = 0; i < barWidth; i++) {
        if (i < pos) printf("=");
        else printf(" ");
    }
    printf("]");
    
    // 打印整个画面
    gotoxy(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            putchar(map[i][j]);
        }
        putchar('\n');
    }
}

// 生成新敌机
void spawnEnemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].x = 1; // 从顶部生成
            enemies[i].y = rand() % (W - 4) + 2; // 确保敌机不会在边界上
            enemies[i].active = 1;
            
            // 根据分数设置敌机速度（基础速度 + 随机偏移）
            enemies[i].speed = enemy_base_speed + rand() % (1 + score / 200);
            if (enemies[i].speed > 6) enemies[i].speed = 6;
            
            return;
        }
    }
}

// 发射散弹
void fireShotgun() {
    int bullets_fired = 0;
    
    // 中间子弹
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = px - 1;
            bullets[i].y = py;
            bullets[i].active = 1;
            bullets_fired++;
            break;
        }
    }
    
    // 左边子弹
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = px - 1;
            bullets[i].y = py - 2;
            bullets[i].active = 1;
            bullets_fired++;
            break;
        }
    }
    
    // 右边子弹
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = px - 1;
            bullets[i].y = py + 2;
            bullets[i].active = 1;
            bullets_fired++;
            break;
        }
    }
    
    // 如果分数足够高，发射额外的两发子弹
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

// 更新游戏状态（无输入）
void updatenoinput() {
    // 移动所有激活的子弹
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x -= 1; // 子弹速度固定为1
            
            // 如果子弹移出屏幕上方，将其设为未激活
            if (bullets[i].x <= 0) {
                bullets[i].active = 0;
            }
            
            // 检测子弹击中敌人
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (bullets[i].active && enemies[j].active) {
                    // 检测宽子弹的碰撞
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
                        bullets[i].active = 0; // 禁用这颗子弹
                        enemies[j].active = 0;  // 禁用敌机
                        
                        // 更新子弹宽度和等级
                        if (score >= 100 && shot_level == 0) {
                            shot_width = 3;
                            shot_level = 1;
                        } else if (score >= 200 && shot_level == 1) {
                            shot_width = 5;
                            shot_level = 2;
                        }
                        
                        // 增加敌机基础速度
                        enemy_base_speed = 1 + score / 100;
                        if (enemy_base_speed > 5) enemy_base_speed = 5;
                        
                        break;
                    }
                }
            }
        }
    }
    
    // 敌机移动和碰撞检测
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // 敌机移动（速度越快移动频率越高）
            static int ve = 0;
            ve++;
            if (ve >= 10 / enemies[i].speed) {
                enemies[i].x += 1;
                ve = 0;
            }
            
            // 检测敌机超出边界
            if (enemies[i].x >= H - 1) {
                enemies[i].active = 0;  // 禁用敌机
                lives--; // 玩家失去一条生命
                if (lives <= 0) {
                    over = 1;
                }
            }
            
            // 检测玩家与敌人碰撞
            if (enemies[i].active) {
                int collision = 0;
                // 检测玩家飞机各个部分
                if (enemies[i].x == px && enemies[i].y == py) collision = 1;
                if (enemies[i].x == px && enemies[i].y == py - 1) collision = 1;
                if (enemies[i].x == px && enemies[i].y == py + 1) collision = 1;
                if (enemies[i].x == px - 1 && enemies[i].y == py) collision = 1;
                
                if (collision) {
                    enemies[i].active = 0; // 禁用敌机
                    lives--;
                    if (lives <= 0) {
                        over = 1;
                    } else {
                        // 被击中后暂时失去强化
                        shot_width = 1;
                        shot_level = 0;
                    }
                }
            }
        }
    }
    
    // 生成新敌机
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
                // 根据子弹等级决定发射方式
                if (shot_level >= 1) {
                    fireShotgun(); // 散弹模式
                } else {
                    // 单发子弹
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
            case 27: over = 1; break;  // ESC退出
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
        Sleep(50); // 控制游戏速度
    }
    gameOverScreen();
    return 0;
}
