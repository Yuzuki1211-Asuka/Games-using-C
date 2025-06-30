#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include "game_utils.h" 

//defining the color
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_CYAN 11
#define COLOR_MAGENTA 13
#define COLOR_DEFAULT 7

int runBirdGame();
int runBrickGame();
int runPlaneGame();
int runSnakeGame();

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

void showMenu() {
    system("cls");
    setConsoleSize();
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int padding = (width - 20) / 2;
    printf("\n\n");
    setColor(COLOR_CYAN);
    printf("%*s", padding, ""); 
    printf("====== ");
    setColor(COLOR_YELLOW);
    printf("游戏菜单");
    setColor(COLOR_CYAN);
    printf(" ======\n");
    
    printf("%*s", padding, "");
    setColor(COLOR_GREEN);
    printf("1. ");
    setColor(COLOR_MAGENTA);
    printf("像素鸟\n");
    
    printf("%*s", padding, "");
    setColor(COLOR_GREEN);
    printf("2. ");
    setColor(COLOR_MAGENTA);
    printf("打砖块\n");
    
    printf("%*s", padding, "");
    setColor(COLOR_GREEN);
    printf("3. ");
    setColor(COLOR_MAGENTA);
    printf("飞机大战\n");
    
    printf("%*s", padding, "");
    setColor(COLOR_GREEN);
    printf("4. ");
    setColor(COLOR_MAGENTA);
    printf("贪吃蛇\n");
    
    printf("%*s", padding, "");
    setColor(COLOR_GREEN);
    printf("5. ");
    setColor(COLOR_MAGENTA);
    printf("退出\n");
    
    printf("%*s", padding, "");
    setColor(COLOR_CYAN);
    printf("======================\n\n");
    
    printf("%*s", padding, "");
    setColor(COLOR_YELLOW);
    printf("请选择 (1-5): ");
    setColor(COLOR_DEFAULT);
}

int main() 
{
    HWND hConsole = GetConsoleWindow();
    ShowWindow(hConsole, SW_MAXIMIZE);
    HideCursor();
    SetConsoleTitle("游戏合集");
    
    // 初始设置控制台大小
    setConsoleSize();
    
    while (1) 
    {
        showMenu();
        char choice = _getch();
        
        switch(choice) {
            case '1': 
                runBirdGame();
                break;
            case '2':
                runBrickGame();
                break;
            case '3':
                runPlaneGame();
                break;
            case '4':
                runSnakeGame();
                break;
            case '5':
                return 0;
            default:
                setColor(COLOR_RED);
                printf("\n无效选择!");
                setColor(COLOR_DEFAULT);
                Sleep(500);
        }
        ShowWindow(hConsole, SW_MAXIMIZE);
    }
    return 0;
}
