#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#define LOOP while(1)
#define WATER 'w'
#define NEIGHBOUR 'n'

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
    char name;
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
char map[2][10][10];
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
void getShipCoordinates(int size, int mapNum, Ship * ships, char n);
bool IsOccupied(char c);
int minimum(int a, int b);
int maximum(int a, int b);
int neighbours();
void printMap(int n,bool showAll);
int getNumberBetween(int min, int max);
Player * choosePlayer();


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
    for(int i = 0;i < 2;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            for(int k = 0;k < 10;k++)
            {
                map[i][j][k] = 'w';
            }
        }
    }
    Player * player1 = choosePlayer();
    Player * player2;
    if(mode == PLAYER)
        player2 = choosePlayer();

    putShips(0, ships1);
    if(mode == PLAYER)
        putShips(1, ships2);

}

Player * choosePlayer() {
    clear();
    printf("1. Choose user.\n");
    printf("2. Create new user.");
    int input = getNumberBetween(1,2);
    clear();
    if(input == 1)
    {
        Player * current = players;
        int i = 0;
        while(current != NULL)
        {
            i++;
            printf("%d. %s (%d)\n" , i , current->name ,current->score );
            current = current->next;
        }
        printf("0. cancel");
        int choose = getNumberBetween(0,i);
        if(choose == 0)return choosePlayer();
        current = players;
        i = 0;
        while(1)
        {
            i++;
            if(i == choose)
                return current;
            current = current->next;
        }
    }
    else
    {
        Player * newPlayer = (Player*) malloc(sizeof(struct Player));
        printf("Enter name: ");
        gets(newPlayer->name);
        while(strlen(newPlayer->name) < 2)
        {
            gets(newPlayer->name);
        }
        newPlayer->score = 0;
        newPlayer->next = NULL;
        if(players == NULL)
        {
            players = newPlayer;
            return newPlayer;
        }
        Player * current = players;
        while(current->next != NULL) current = current->next;
        current->next = newPlayer;
        return newPlayer;
    }
}

void putShips(int mapNum, struct Ship * ships){
    printf("------- SET YOUR MAP -------\n\n");
    printf(" You Have 10 Ships in total: \n");
    printf(" 1 ship with the size of 5 \n");
    printf(" 2 ships with the size of 3\n");
    printf(" 3 ships with the size of 2\n");
    printf(" 4 ships with the size of 1\n");
    printf("-----------------------------\n");
    getShipCoordinates(5, mapNum, ships, 'a');
    getShipCoordinates(3, mapNum, ships, 'b');
    getShipCoordinates(3, mapNum, ships, 'c');
    getShipCoordinates(2, mapNum, ships, 'd');
    getShipCoordinates(2, mapNum, ships, 'e');
    getShipCoordinates(2, mapNum, ships, 'f');
    getShipCoordinates(1, mapNum, ships, 'g');
    getShipCoordinates(1, mapNum, ships, 'h');
    getShipCoordinates(1, mapNum, ships, 'i');
    getShipCoordinates(1, mapNum, ships, 'j');
}

void getShipCoordinates(int size, int mapNum, struct Ship * ships, char n){

    int x , y;
    printf("---------------------------------\n");
    printf("| ->      Ship Size : %d      <- |\n", size);
    printf("---------------------------------\n");
    Ship * newShip = (Ship*) malloc(sizeof(Ship));
    newShip->size = size;
    newShip->name = n;

    while(true){
        printf("         Start Location x:");
        scanf("%d", &x);
        while(x < 0 || x > 9)
        {
            printf("Wrong input. try again.");
            scanf("%d", &x);
        }
        printf("         Start Location y:");

        scanf("%d", &y);
        while(y < 0 || y > 9)
        {
            printf("Wrong input. try again.");
            scanf("%d", &y);
        }
        newShip->start_x = x;
        newShip->start_y = y;
        printf("         End Location x:");
        scanf("%d", &x);
        while(x < 0 || x > 9)
        {
            printf("Wrong input. try again.");
            scanf("%d", &x);
        }
        printf("         End Location y:");
        scanf("%d", &y);
        while(y < 0 || y > 9)
        {
            printf("Wrong input. try again.");
            scanf("%d", &y);
        }

        newShip->end_x = x;
        newShip->end_y = y;

        if(x == newShip->start_x ){
            int distance = abs(newShip->start_y - y);
            if(distance != newShip->size ){
                printf("!!!ERROR!!! size doesn't match !!!ERROR!!! \n");
                continue;
            }
        }
        else if(y == newShip->start_y){
            int temp = abs(newShip->start_x - x);
            if(temp != newShip->size ){
                printf("!!!ERROR!!! size doesn't match' !!!ERROR!!! \n");
                continue;
            }
        }
        else{
            printf("!!!ERROR!!! you can't place your ship like this !!!ERROR!!! \n");
            continue;
        }
        bool flag = false;
        for(int i = maximum(minimum(newShip->start_x , newShip->end_x) - 1,0) ; i <= minimum(maximum(newShip->start_x , newShip->end_x) + 1,9);i++)
        {
            for(int j = maximum(minimum(newShip->start_y , newShip->end_y) - 1,0) ; j <= minimum(maximum(newShip->start_y , newShip->end_y) + 1,9);j++)
            {
                if(IsOccupied(map[mapNum][i][j]))
                {
                    printf("!!!ERROR!!! You can't put your ship here !!!ERROR!!!\n");
                    flag = true;
                    break;
                }
            }
            if(flag) break;
        }
        if(flag)continue;
        for(int i = minimum(newShip->start_x,newShip->end_x);i <= maximum(newShip->start_x,newShip->end_x);i++)
        {
            for(int j = minimum(newShip->start_y,newShip->end_y);j <= maximum(newShip->start_y,newShip->end_y);j++)
            {
                map[mapNum][i][j] = newShip->name;
            }
        }

        for(int i = maximum(minimum(newShip->start_x , newShip->end_x) - 1,0) ; i <= minimum(maximum(newShip->start_x , newShip->end_x) + 1,9);i++)
        {
            for(int j = maximum(minimum(newShip->start_y , newShip->end_y) - 1,0) ; j <= minimum(maximum(newShip->start_y , newShip->end_y) + 1,9);j++)
            {
                if(map[mapNum][i][j] == WATER)
                    map[mapNum][i][j] = NEIGHBOUR;
            }
        }
        break;
    }
    printMap(mapNum,true);
}

int minimum(int a, int b){
    if ( a > b)
        return b;
    return a;
}


int maximum(int a, int b){
    if ( a < b)
        return b;
    return a;
}


bool IsOccupied( char c ){
    if( c >= 'a' && c <= 'j' )
        return true;
    return false;
}


void printMap(int mapNum,bool showAll){
    if(showAll)
    {
        printf("_________________________________________\n");
        for(int i = 0;i < 10;i++)
        {
            printf("|   |   |   |   |   |   |   |   |   |   |\n");
            printf("|");
            for(int j = 0;j < 10;j++)
            {
                printf(" %c |" , map[mapNum][i][j] != 'w' ? map[mapNum][i][j] : ' ');
            }
            printf("\n");
            printf("|___|___|___|___|___|___|___|___|___|___|\n");
        }
    }
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
    int choose = getNumberBetween(1,7);
    clear();
    return choose;
}

int getNumberBetween(int min, int max) {
    int temp = -1;
    while(temp < min || temp > max){
        temp = getChar() - '0';
    }
    return temp;
}
