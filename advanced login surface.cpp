#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h> // �����ַ�����
#include "game_utils.h" 

// ������ɫ
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_CYAN 11
#define COLOR_MAGENTA 13
#define COLOR_DEFAULT 7
#define COLOR_WHITE 15
#define COLOR_BLUE 9
#define COLOR_ORANGE 6

// ������̳���
#define BACKSPACE 8
#define ENTER 13
#define ESCAPE 27

// �û�ƾ֤�ļ�
#define USER_FILE "users.dat"

// �û��ṹ��
typedef struct {
    char username[20];
    char password[20];
    int highscore; // ��ѡ���洢�û���߷�
} User;

// ��������
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
    printCenteredColor("|      ��Ϸ��¼ϵͳ       |", COLOR_YELLOW);
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
    printCenteredColor("|      �û�ע��ϵͳ       |", COLOR_YELLOW);
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
                printf("\b \b"); // ɾ��ǰһ���Ǻ�
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
    // ����û�������
    if (strlen(username) < 3 || strlen(username) > 19) {
        printCenteredColor("�û�����������3-19�ַ�֮��", COLOR_RED);
        return 0;
    }
    
    // ����Ƿ�ֻ������ĸ������
    for (int i = 0; username[i]; i++) {
        if (!isalnum(username[i])) {
            printCenteredColor("�û���ֻ�ܰ�����ĸ������", COLOR_RED);
            return 0;
        }
    }
    
    return 1;
}

int validatePassword(const char *password) {
    // ������볤��
    if (strlen(password) < 6) {
        printCenteredColor("���볤��������Ҫ6���ַ�", COLOR_RED);
        return 0;
    }
    
    int hasDigit = 0, hasAlpha = 0;
    
    // ������븴�Ӷ�
    for (int i = 0; password[i]; i++) {
        if (isdigit(password[i])) hasDigit = 1;
        if (isalpha(password[i])) hasAlpha = 1;
    }
    
    if (!hasDigit || !hasAlpha) {
        printCenteredColor("�����������ĸ������", COLOR_RED);
        return 0;
    }
    
    return 1;
}

void saveUserToFile(const User *user) {
    FILE *file = fopen(USER_FILE, "ab"); // �Զ�����׷��ģʽ��
    
    if (file == NULL) {
        printCenteredColor("�޷������û�����", COLOR_RED);
        return;
    }
    
    fwrite(user, sizeof(User), 1, file);
    fclose(file);
}

int authenticateUser(const char *username, const char *password) {
    FILE *file = fopen(USER_FILE, "rb");
    
    if (file == NULL) {
        return 0; // �ļ������ڣ�û���û�
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
        return 0; // �û�������
    }
    
    // ��֤����
    return (strcmp(password, user.password) == 0);
}

int registrationScreen() {
    User newUser;
    
    while (1) {
        drawRegistrationBox();
        
        printCenteredColor("�������û��� (3-19����ĸ������):", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        scanf("%19s", newUser.username);
        clearInputBuffer(); // ������뻺����
        
        if (!validateUsername(newUser.username)) {
            Sleep(2000);
            continue;
        }
        
        // ����û����Ƿ��Ѵ���
        if (authenticateUser(newUser.username, "dummy")) { // ʹ�������������û��Ƿ����
            printCenteredColor("���û����ѱ�ע��", COLOR_RED);
            Sleep(2000);
            continue;
        }
        
        printCenteredColor("\n���������� (����6λ������ĸ������):", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        readPassword(newUser.password, sizeof(newUser.password));
        
        if (!validatePassword(newUser.password)) {
            Sleep(2000);
            continue;
        }
        
        printCenteredColor("\n��ȷ������:", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        
        char confirmPassword[20];
        readPassword(confirmPassword, sizeof(confirmPassword));
        
        if (strcmp(newUser.password, confirmPassword) != 0) {
            printCenteredColor("\n������������벻һ��", COLOR_RED);
            Sleep(2000);
            continue;
        }
        
        // ��ʼ���߷�
        newUser.highscore = 0;
        
        // �����û�
        saveUserToFile(&newUser);
        
        printCenteredColor("\nע��ɹ���", COLOR_GREEN);
        Sleep(1500);
        
        return 1; // ע��ɹ�
    }
}

int loginScreen(User *currentUser) {
    char username[20];
    char password[20];
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        drawLoginBox();
        
        printCenteredColor("�������û���:", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        scanf("%19s", username);
        clearInputBuffer(); // ������뻺����
        
        printCenteredColor("\n����������:", COLOR_WHITE);
        printf("\n");
        printCentered("> ");
        readPassword(password, sizeof(password));
        
        if (authenticateUser(username, password)) {
            // ��ȡ�û�����
            FILE *file = fopen(USER_FILE, "rb");
            if (file != NULL) {
                User user;
                while (fread(&user, sizeof(User), 1, file)) {
                    if (strcmp(username, user.username) == 0) {
                        *currentUser = user; // ���õ�ǰ�û�
                        break;
                    }
                }
                fclose(file);
            }
            
            printCenteredColor("\n��¼�ɹ���", COLOR_GREEN);
            Sleep(1000);
            return 1;
        } else {
            attempts++;
            printCenteredColor("\n�û������������", COLOR_RED);
            
            if (attempts < maxAttempts) {
                printf("\n");
                printCenteredColor("�����������...", COLOR_YELLOW);
                printf("\n");
                printCenteredColor("ʣ�ೢ�Դ���: ", COLOR_YELLOW);
                printf("%d/%d\n", maxAttempts - attempts, maxAttempts);
                _getch();
            } else {
                printCenteredColor("\n��������Դ�����", COLOR_RED);
                printCenteredColor("ϵͳ����3����˳�...", COLOR_RED);
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
    snprintf(welcomeMsg, sizeof(welcomeMsg), "��ӭ����, %s!", username);
    
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
    printCenteredColor("|      ��Ϸ��ڲ˵�       |", COLOR_YELLOW);
    printCenteredColor("|                         |", COLOR_CYAN);
    printCenteredColor("+-------------------------+", COLOR_CYAN);
    printf("\n");
    
    printCenteredColor("1. ��¼�����˻�", COLOR_GREEN);
    printCenteredColor("2. ע�����˻�", COLOR_BLUE);
    printCenteredColor("3. �˳�ϵͳ", COLOR_RED);
    printf("\n");
    printCenteredColor("��ѡ�� (1-3): ", COLOR_YELLOW);
    
    char choice = _getch();
    return choice;
}

void showMenu(User *currentUser) {
    system("cls");
    setConsoleSize();
    
    char welcomeMsg[50];
    snprintf(welcomeMsg, sizeof(welcomeMsg), "��ӭ, %s!", currentUser->username);
    
    printf("\n\n");
    printCenteredColor("================================", COLOR_CYAN);
    printCenteredColor(welcomeMsg, COLOR_YELLOW);
    
    // ��ʾ�߷֣�����У�
    if (currentUser->highscore > 0) {
        char scoreMsg[50];
        snprintf(scoreMsg, sizeof(scoreMsg), "��ǰ��߷�: %d", currentUser->highscore);
        printCenteredColor(scoreMsg, COLOR_ORANGE);
    }
    
    printCenteredColor("================================", COLOR_CYAN);
    printf("\n");
    
    printCenteredColor("1. ������", COLOR_GREEN);
    printCenteredColor("2. ��ש��", COLOR_MAGENTA);
    printCenteredColor("3. �ɻ���ս", COLOR_CYAN);
    printCenteredColor("4. ̰����", COLOR_YELLOW);
    printCenteredColor("5. �˳���¼", COLOR_RED);
    printf("\n");
    printCenteredColor("================================", COLOR_CYAN);
    printf("\n");
    
    printCenteredColor("��ѡ�� (1-5): ", COLOR_YELLOW);
    setColor(COLOR_DEFAULT);
}

void updateHighscore(User *user, int score) {
    if (score > user->highscore) {
        user->highscore = score;
        
        // �����ļ��еĸ߷�
        FILE *file = fopen(USER_FILE, "r+b");
        if (file != NULL) {
            User temp;
            long pos = 0;
            
            while (fread(&temp, sizeof(User), 1, file)) {
                if (strcmp(user->username, temp.username) == 0) {
                    fseek(file, pos, SEEK_SET); // �ص���¼��ʼλ��
                    fwrite(user, sizeof(User), 1, file); // ���¼�¼
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
    SetConsoleTitle("��Ϸ�ϼ�");
    
    // ��ʼ���ÿ���̨��С
    setConsoleSize();
    
    User currentUser;
    int loggedIn = 0;
    
    while (1) {
        if (!loggedIn) {
            int choice = showLoginMenu();
            
            switch(choice) {
                case '1': // ��¼
                    loggedIn = loginScreen(&currentUser);
                    if (loggedIn) {
                        showWelcomeMessage(currentUser.username);
                    } else {
                        return 0; // ��¼ʧ���˳�
                    }
                    break;
                case '2': // ע��
                    if (registrationScreen()) {
                        printCenteredColor("��ʹ�����˻���¼", COLOR_GREEN);
                        Sleep(1500);
                    }
                    break;
                case '3': // �˳�
                    return 0;
                default:
                    printCenteredColor("\n��Чѡ��!", COLOR_RED);
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
                    loggedIn = 0; // �˳���¼
                    break;
                default:
                    setColor(COLOR_RED);
                    printCentered("\n��Чѡ��!");
                    setColor(COLOR_DEFAULT);
                    Sleep(500);
            }
            ShowWindow(hConsole, SW_MAXIMIZE);
        }
    }
    return 0;
}
