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
}Ship;

typedef struct Player{
    char name[50];
    int score;
    struct Playerm * next;
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



int main(){
	LOOP
	{
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

void starGame(int mode)
{

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
    while(temp < '1' || temp > '7')
        temp = getChar();
    clear();
    return temp-'0';
}
