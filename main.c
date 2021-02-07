#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define LOOP while(1)

//ENUMS
enum {
    PLAY_WITH_FRIEND = 1,
    PLAY_WITH_BOT,
    LOAD_GAME,
    LOAD_LAST_GAME,
    SETTINGS,
    SCOREBOARD,
    EXIT,
    PLAYER,
    BOT
};

//STRUCTS
typedef struct Ship{
    int size;
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    struct ship * next;
}Ship;

typedef struct Player{
    char name[50];
    int score;
    struct Player * next;
}Player;

//GLOBAL VALUES
char map1[10][10];
char map2[10][10];
Player * players = NULL;
Ship * ships1 = NULL;
Ship * ships2 = NULL;

//FUNCTIONS
int getInt();
char getChar();
void clear();
void settings();
int mainMenu();
void startGame();
void loadGame();
void loadLastGame();
void scoreboard();
void putShips();
void getShipCoordinates();
bool checkValidation();
bool IsOccupied(char);
void printMap();

int main(){
    LOOP{
        int choose = mainMenu();
        if(choose == PLAY_WITH_FRIEND)
            startGame(PLAYER);
        if(choose == PLAY_WITH_BOT)
            startGame(BOT);
        if(choose == LOAD_GAME)
            loadGame();
        if(choose == LOAD_LAST_GAME)
            loadLastGame();
        if(choose == SETTINGS)
            settings();
        if(choose == SCOREBOARD)
            scoreboard();
        if(choose == EXIT)
            return 0;
    }
}

void scoreboard() {

}

void loadLastGame() {

}

void loadGame() {

}

void startGame(int mode){
    Player player1, player2;
    //player 1 = choosePlayer(null);
    putShips(map1, ships1);
    //player 2 = choosePlayer(null / player1);
    //putShips(player2);
}

void putShips(char map[10][10], struct Ship * ships){
    printMap(map);
    printf("------- SET YOUR MAP -------\n\n");
    printf(" You Have 10 Ships in total: \n");
    printf(" 1 ship with the size of 5 \n");
    printf(" 2 ships with the size of 3\n");
    printf(" 3 ships with the size of 2\n");
    printf(" 4 ships with the size of 1\n");
    printf("-----------------------------\n");
    getShipCoordinates(5, map, ships);
    getShipCoordinates(3, map, ships);
    getShipCoordinates(3, map, ships);
    getShipCoordinates(2, map, ships);
    getShipCoordinates(2, map, ships);
    getShipCoordinates(2, map, ships);
    getShipCoordinates(1, map, ships);
    getShipCoordinates(1, map, ships);
    getShipCoordinates(1, map, ships);
    getShipCoordinates(1, map, ships);
}

void getShipCoordinates(int size, char map [10][10], struct Ship * ships){
    char tempx[2], tempy[2];
    printf(" ->      Ship Size : %d      <- \n", size);
    Ship neww;
    neww.size = size;
    while(true){
        printf("         Start Location x:");
        scanf("%s", &tempx);
        while(!checkValidation(tempx,1,10)){
            printf("!!!ERROR!!! Out of boundry : Try again !!!ERROR!!! \n");
            printf("         Start Location x:");
            scanf("%s", &tempx);
        }
        printf("         Start Location y:");
        scanf("%s", &tempy);
        while(!checkValidation(tempy,1,10)){
            printf("!!!ERROR!!! Out of boundry : Try again !!!ERROR!!! \n");
            printf("         Start Location y:");
            scanf("%s", &tempy);
        }
        int x = atoi(tempx); int y = atoi(tempy);
        int neighbours = 0;
        int i, j;
        for( i = x - 1; i <= x + 1; ++i ){
            for( j = y - 1; j <= y + 1; ++j ){
                neighbours += IsOccupied( map[i][j] ) ? 1 : 0;
            }
        }
        if( neighbours == 0 )
            break;
        printf("!!!ERROR!!! This cell or it's neighbours are occupied !!!ERROR!!!\n");
    }
    neww.start_x = atoi(tempx);
    neww.start_y = atoi(tempy);
    map[neww.start_y-1][neww.start_x-1] = 's';

    while(true){
        printf("         End Location x:");
        scanf("%s", &tempx);
        while(!checkValidation(tempx,1,10)){
            printf("!!!ERROR!!! Out of boundry : Try again !!!ERROR!!! \n");
            printf("         End Location x:");
            scanf("%s", &tempx);
        }
        printf("         End Location y:");
        scanf("%s", &tempy);
        while(!checkValidation(tempy,1,10)){
            printf("!!!ERROR!!! Out of boundry : Try again !!!ERROR!!! \n");
            printf("         End Location y:");
            scanf("%s", &tempy);
        }
        int x = atoi(tempx); int y = atoi(tempy);
        int neighbours = 0;
        int i, j;
        for( i = x - 1; i <= x + 1; ++i ){
            for( j = y - 1; j <= y + 1; ++j ){
                neighbours += IsOccupied( map[i][j] ) ? 1 : 0;
            }
        }
        if( neighbours == 0 )
            break;
        printf("!!!ERROR!!! This cell or it's neighbours are occupied !!!ERROR!!!\n");
    }

    printMap(map);
}

bool IsOccupied( char c ){
    if( c == 's' )
        return true;
    return false;
}

bool checkValidation( char n[], int s, int f){
    int number = atoi(n);
    if( number == 0 )
        return false;
    if ( number < s || number > f )
        return false;
    return true;
}

void printMap(char map[10][10] ){
    int i, j;
    printf("    1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10\n");
    for( i = 0 ; i < 10 ; i++ ){
        printf("   _______________________________________\n");
        if( i == 9 )
            printf("%d|", i+1);
        else
            printf(" %d|", i+1);
        for( j = 0 ; j < 10 ; j++ ){
            //if( map[i][j] == 's')
            //	printf("   |");
            //else
            printf(" %c |", map[i][j]);
        }
        printf("\n");
    }
    printf("   _______________________________________\n");
}

void settings() {
    printf("Choose your Theme:\n");
    printf("___________________________\n");
    printf("|1- Blue and White         |\n");
    printf("|2- Green and White        |\n");
    printf("|3- Red and White          |\n");
    printf("|4- Black and White        |\n");
    printf("|5- Exit to Menu           |\n");
    printf("|__________________________|\n");
    while (1){
        char theme;
        theme = getChar();
        if (theme == '1')
            system("color f1");
        if (theme == '2')
            system("color f2");
        if (theme == '3')
            system("color f4");
        if (theme == '4')
            system("color f0");
        if (theme == '5')
            return;
    }
}

int getInt() {
    int temp;
    scanf("%d" , &temp);
    return temp;
}

char getChar() {
    return _getch();
}

void clear() {
    system("cls");
}

int mainMenu() {

    printf("________Main Menu________\n");
    printf("|                       |\n");
    printf("| 1. Play with a Friend |\n");
    printf("| 2. Play with bot      |\n");
    printf("| 3. Load game          |\n");
    printf("| 4. Load last game     |\n");
    printf("| 5. Settings           |\n");
    printf("| 6. Score Board        |\n");
    printf("| 7. Quit               |\n");
    printf("|_______________________|\n");
    char temp = '0';
    while(temp < '1' || temp > '7'){
        temp = getChar();
        //scanf("%c",&temp);
        //printf("temp: %c\n", temp);
    }
    clear();
    return temp-'0';
}
