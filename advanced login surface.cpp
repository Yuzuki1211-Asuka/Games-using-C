#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h> // 用于字符处理
#include "game_utils.h" 

// 定义颜色
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_CYAN 11
#define COLOR_MAGENTA 13
#define COLOR_DEFAULT 7
#define COLOR_WHITE 15
#define COLOR_BLUE 9
#define COLOR_ORANGE 6

// 定义键盘常量
#define BACKSPACE 8
#define ENTER 13
#define ESCAPE 27

// 用户凭证文件
#define USER_FILE "users.dat"

// 用户结构体
typedef struct {
    char username[20];
    char password[20];
    int highscore; // 可选：存储用户最高分
} User;

// 函数声明
int runBirdGame();
int runBrickGame();
int runPlaneGame();
int runSnakeGame();
int loginScreen(User *currentUser);
int registrationScreen();
void saveUserToFile(const User *user);
int authenticateUser(const char *username, const char *password);
void readPassword(char *password, int maxLength);
void printCentered(const char *text);
void printCenteredColor(const char *text, int color);
int getConsoleWidth();
void setColor(int color);
void setConsoleSize();
void drawLoginBox();
void drawRegistrationBox();
void clearInputBuffer();

void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void setConsoleSize() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD bufferSize = {csbi.srWindow.Right - csbi.srWindow.Left + 1, 
                        csbi.srWindow.Bottom - csbi.srWindow.Top + 1};
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    SMALL_RECT rect = {0, 0, bufferSize.X - 1, bufferSize.Y - 1};
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void printCentered(const char* text) {
    int width = getConsoleWidth();
    int textLength = strlen(text);
    int padding = (width - textLength) / 2;
    
    if (padding < 0) padding = 0;
    
    printf("%*s", padding, "");
    printf("%s\n", text);
}

void printCenteredColor(const char* text, int color) {
    setColor(color);
    printCentered(text);
    setColor(COLOR_DEFAULT);
}

void drawLoginBox() {
    system("cls");
    setConsoleSize();
    
    printf("\n\n\n");
    printCenteredColor("+-------------------------+", COLOR_CYAN);
    printCenteredColor("|                         |", COLOR_CYAN);
    printCenteredColor("|      游戏登录系统       |", COLOR_YELLOW);
    printCenteredColor("|                         |", COLOR_CYAN);
    printCenteredColor("+-------------------------+", COLOR_CYAN);
    printf("\n");
}

void drawRegistrationBox() {
    system("cls");
    setConsoleSize();
    
    printf("\n\n\n");
    printCenteredColor("+-------------------------+", COLOR_CYAN);
    printCenteredColor("|                         |", COLOR_CYAN);
    printCenteredColor("|      用户注册系统       |", COLOR_YELLOW);
    printCenteredColor("|                         |", COLOR_CYAN);
    printCenteredColor("+-------------------------+", COLOR_CYAN);
    printf("\n");
}

void readPassword(char* password, int maxLength) {
    int pos = 0;
    char ch;
    
    while (1) {
        ch = _getch();
        
        if (ch == ENTER) {
            password[pos] = '\0';
            printf("\n");
            return;
        } else if (ch == BACKSPACE) {
            if (pos > 0) {
                pos--;
                printf("\b \b"); // 删除前一个星号
            }
        } else if (ch == ESCAPE) {
            password[0] = '\0';
            return;
        } else if (pos < maxLength - 1 && ch >= 32 && ch <= 126) {
            password[pos] = ch;
            pos++;
            printf("*");
        }
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validateUsername(const char *username) {
    // 检查用户名长度
    if (strlen(username) < 3 || strlen(username) > 19) {
        printCenteredColor("用户名长度需在3-19字符之间", COLOR_RED);
        return 0;
    }
    
    // 检查是否只包含字母和数字
    for (int i = 0; username[i]; i++) {
        if (!isalnum(username[i])) {
            printCenteredColor("用户名只能包含字母和数字", COLOR_RED);
            return 0;
        }
    }
    
    return 1;
}

int validatePassword(const char *password) {
    // 检查密码长度
    if (strlen(password) < 6) {
        printCenteredColor("密码长度至少需要6个字符", COLOR_RED);
        return 0;
    }
    
    int hasDigit = 0, hasAlpha = 0;
    
    // 检查密码复杂度
    for (int i = 0; password[i]; i++) {
        if (isdigit(password[i])) hasDigit = 1;
        if (isalpha(password[i])) hasAlpha = 1;
    }
    
    if (!hasDigit || !hasAlpha) {
        printCenteredColor("密码需包含字母和数字", COLOR_RED);
        return 0;
    }
    
    return 1;
}

void saveUserToFile(const User *user) {
    FILE *file = fopen(USER_FILE, "ab"); // 以二进制追加模式打开
    
    if (file == NULL) {
        printCenteredColor("无法保存用户数据", COLOR_RED);
        return;
    }
    
    fwrite(user, sizeof(User), 1, file);
    fclose(file);
}

int authenticateUser(const char *username, const char *password) {
    FILE *file = fopen(USER_FILE, "rb");
    
    if (file == NULL) {
        return 0; // 文件不存在，没有用户
    }
    
    User user;
    int found = 0;
    
    while (fread(&user, sizeof(User), 1, file)) {
        if (strcmp(username, user.username) == 0) {
            found = 1;
            break;
        }
    }
    
    fclose(file);
    
    if (!found) {
        return 0; // 用户不存在
    }
    
    // 验证密码
    return (strcmp(password, user.password) == 0);
}

int registrationScreen() {
    User newUser;
    
    while (1) {
        drawRegistrationBox();
        
        printCenteredColor("请输入用户名 (3-19个字母或数字):", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        scanf("%19s", newUser.username);
        clearInputBuffer(); // 清除输入缓冲区
        
        if (!validateUsername(newUser.username)) {
            Sleep(2000);
            continue;
        }
        
        // 检查用户名是否已存在
        if (authenticateUser(newUser.username, "dummy")) { // 使用虚拟密码检查用户是否存在
            printCenteredColor("该用户名已被注册", COLOR_RED);
            Sleep(2000);
            continue;
        }
        
        printCenteredColor("\n请输入密码 (至少6位，含字母和数字):", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        readPassword(newUser.password, sizeof(newUser.password));
        
        if (!validatePassword(newUser.password)) {
            Sleep(2000);
            continue;
        }
        
        printCenteredColor("\n请确认密码:", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        
        char confirmPassword[20];
        readPassword(confirmPassword, sizeof(confirmPassword));
        
        if (strcmp(newUser.password, confirmPassword) != 0) {
            printCenteredColor("\n两次输入的密码不一致", COLOR_RED);
            Sleep(2000);
            continue;
        }
        
        // 初始化高分
        newUser.highscore = 0;
        
        // 保存用户
        saveUserToFile(&newUser);
        
        printCenteredColor("\n注册成功！", COLOR_GREEN);
        Sleep(1500);
        
        return 1; // 注册成功
    }
}

int loginScreen(User *currentUser) {
    char username[20];
    char password[20];
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        drawLoginBox();
        
        printCenteredColor("请输入用户名:", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        scanf("%19s", username);
        clearInputBuffer(); // 清除输入缓冲区
        
        printCenteredColor("\n请输入密码:", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        readPassword(password, sizeof(password));
        
        if (authenticateUser(username, password)) {
            // 获取用户数据
            FILE *file = fopen(USER_FILE, "rb");
            if (file != NULL) {
                User user;
                while (fread(&user, sizeof(User), 1, file)) {
                    if (strcmp(username, user.username) == 0) {
                        *currentUser = user; // 设置当前用户
                        break;
                    }
                }
                fclose(file);
            }
            
            printCenteredColor("\n登录成功！", COLOR_GREEN);
            Sleep(1000);
            return 1;
        } else {
            attempts++;
            printCenteredColor("\n用户名或密码错误！", COLOR_RED);
            
            if (attempts < maxAttempts) {
                printf("\n");
                printCenteredColor("按任意键重试...", COLOR_YELLOW);
                printf("\n");
                printCenteredColor("剩余尝试次数: ", COLOR_YELLOW);
                printf("%d/%d\n", maxAttempts - attempts, maxAttempts);
                _getch();
            } else {
                printCenteredColor("\n超过最大尝试次数！", COLOR_RED);
                printCenteredColor("系统将在3秒后退出...", COLOR_RED);
                Sleep(3000);
                return 0;
            }
        }
    }
    return 0;
}

void showWelcomeMessage(const char *username) {
    system("cls");
    setConsoleSize();
    
    char welcomeMsg[50];
    snprintf(welcomeMsg, sizeof(welcomeMsg), "欢迎回来, %s!", username);
    
    printf("\n\n\n");
    printCenteredColor("================================", COLOR_CYAN);
    printCenteredColor(welcomeMsg, COLOR_YELLOW);
    printCenteredColor("================================", COLOR_CYAN);
    Sleep(1500);
}

int showLoginMenu() {
    system("cls");
    setConsoleSize();
    
    printf("\n\n\n");
    printCenteredColor("+-------------------------+", COLOR_CYAN);
    printCenteredColor("|                         |", COLOR_CYAN);
    printCenteredColor("|      游戏入口菜单       |", COLOR_YELLOW);
    printCenteredColor("|                         |", COLOR_CYAN);
    printCenteredColor("+-------------------------+", COLOR_CYAN);
    printf("\n");
    
    printCenteredColor("1. 登录现有账户", COLOR_GREEN);
    printCenteredColor("2. 注册新账户", COLOR_BLUE);
    printCenteredColor("3. 退出系统", COLOR_RED);
    printf("\n");
    printCenteredColor("请选择 (1-3): ", COLOR_YELLOW);
    
    char choice = _getch();
    return choice;
}

void showMenu(User *currentUser) {
    system("cls");
    setConsoleSize();
    
    char welcomeMsg[50];
    snprintf(welcomeMsg, sizeof(welcomeMsg), "欢迎, %s!", currentUser->username);
    
    printf("\n\n");
    printCenteredColor("================================", COLOR_CYAN);
    printCenteredColor(welcomeMsg, COLOR_YELLOW);
    
    // 显示高分（如果有）
    if (currentUser->highscore > 0) {
        char scoreMsg[50];
        snprintf(scoreMsg, sizeof(scoreMsg), "当前最高分: %d", currentUser->highscore);
        printCenteredColor(scoreMsg, COLOR_ORANGE);
    }
    
    printCenteredColor("================================", COLOR_CYAN);
    printf("\n");
    
    printCenteredColor("1. 像素鸟", COLOR_GREEN);
    printCenteredColor("2. 打砖块", COLOR_MAGENTA);
    printCenteredColor("3. 飞机大战", COLOR_CYAN);
    printCenteredColor("4. 贪吃蛇", COLOR_YELLOW);
    printCenteredColor("5. 退出登录", COLOR_RED);
    printf("\n");
    printCenteredColor("================================", COLOR_CYAN);
    printf("\n");
    
    printCenteredColor("请选择 (1-5): ", COLOR_YELLOW);
    setColor(COLOR_DEFAULT);
}

void updateHighscore(User *user, int score) {
    if (score > user->highscore) {
        user->highscore = score;
        
        // 更新文件中的高分
        FILE *file = fopen(USER_FILE, "r+b");
        if (file != NULL) {
            User temp;
            long pos = 0;
            
            while (fread(&temp, sizeof(User), 1, file)) {
                if (strcmp(user->username, temp.username) == 0) {
                    fseek(file, pos, SEEK_SET); // 回到记录开始位置
                    fwrite(user, sizeof(User), 1, file); // 更新记录
                    break;
                }
                pos = ftell(file);
            }
            fclose(file);
        }
    }
}

int main() {
    HWND hConsole = GetConsoleWindow();
    ShowWindow(hConsole, SW_MAXIMIZE);
    HideCursor();
    SetConsoleTitle("游戏合集");
    
    // 初始设置控制台大小
    setConsoleSize();
    
    User currentUser;
    int loggedIn = 0;
    
    while (1) {
        if (!loggedIn) {
            int choice = showLoginMenu();
            
            switch(choice) {
                case '1': // 登录
                    loggedIn = loginScreen(&currentUser);
                    if (loggedIn) {
                        showWelcomeMessage(currentUser.username);
                    } else {
                        return 0; // 登录失败退出
                    }
                    break;
                case '2': // 注册
                    if (registrationScreen()) {
                        printCenteredColor("请使用新账户登录", COLOR_GREEN);
                        Sleep(1500);
                    }
                    break;
                case '3': // 退出
                    return 0;
                default:
                    printCenteredColor("\n无效选择!", COLOR_RED);
                    Sleep(500);
            }
        } else {
            showMenu(&currentUser);
            char choice = _getch();
            
            int score = 0;
            switch(choice) {
                case '1': 
                    score = runBirdGame();
                    updateHighscore(&currentUser, score);
                    break;
                case '2':
                    score = runBrickGame();
                    updateHighscore(&currentUser, score);
                    break;
                case '3':
                    score = runPlaneGame();
                    updateHighscore(&currentUser, score);
                    break;
                case '4':
                    score = runSnakeGame();
                    updateHighscore(&currentUser, score);
                    break;
                case '5':
                    loggedIn = 0; // 退出登录
                    break;
                default:
                    setColor(COLOR_RED);
                    printCentered("\n无效选择!");
                    setColor(COLOR_DEFAULT);
                    Sleep(500);
            }
            ShowWindow(hConsole, SW_MAXIMIZE);
        }
    }
    return 0;
}
