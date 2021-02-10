#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

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
    struct Ship * next;
}Ship;

typedef struct Player{
    char name[50];
    int score;
    int coins;
    struct Player * next;
}Player;

//GLOBAL VALUES
char map[2][10][10];
Player * players = NULL;
Ship * ships[2];
Player * currentPlayers[2];
int turn = 0;
bool aiMode = false;

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
void getShipCoordinates(int size, int mapNum, char n);
bool IsOccupied(char c);
int minimum(int a, int b);
int maximum(int a, int b);
void createShip(int num , int size , char name , int start_x , int start_y, int end_x , int end_y);
int neighbours();
void printMap(int n,bool showAll);
int getNumberBetween(int min, int max);
Player * choosePlayer();


void putShipsForAI();

void gameLoop();

bool shipAlive(char name, int num);

void attack(int turn);

void checkShips(int num);

void aiMove();

bool checkEnd();

void savePlayer();

void loadPlayers();

void getCoins(int playerNum, int size);

int main(){
    loadPlayers();
    srand(time(0));
    LOOP{
        savePlayer();
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

void loadPlayers() {
    FILE * f = fopen("players.sav" , "rb");
    if(f == NULL)return;
    Player newPlayer;
    Player * current = NULL;
    while(fread(&newPlayer, sizeof(Player), 1, f) != EOF)
    {
        if(players == NULL)
        {
            players = (Player *) malloc(sizeof(Player));
            *players = newPlayer;
            current = players;
            current->next = NULL;
        }
        else
        {
            current->next = (Player *) malloc(sizeof(Player));
            *current->next = newPlayer;
            current = current->next;
            current->next = NULL;
        }
    }
    fclose(f);

}

void savePlayer() {
    FILE * f = fopen("players.sav" , "wb");
    Player * current = players;
    while(current != NULL)
    {
        fwrite(current,sizeof(Player),1,f);
        current = current->next;
    }
    fclose(f);
}

void scoreboard() {

}

void loadLastGame() {

}

void loadGame() {

}

void startGame(int mode){
    ships[0] = NULL;
    ships[1] = NULL;
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
    currentPlayers[0] = choosePlayer();
    if(mode == PLAYER)
        currentPlayers[1] = choosePlayer();
    else
        currentPlayers[1] = NULL;

    putShips(0, ships[0]);
    if(mode == PLAYER)
    {
        putShips(1, ships[1]);
        turn = 0;
        aiMode = false;
    }
    else
    {
        putShipsForAI();
        turn = 0;
        aiMode = true;
    }
    gameLoop();

}

void gameLoop() {
    fflush(stdin);
    while(!checkEnd())
    {
        clear();
        printMap(1-turn,false);
        attack(turn);
        checkShips(1-turn);
        clear();
        printMap(1-turn,false);
        if(checkEnd())break;

        fflush(stdin);
        printf("1. Save game\n");
        printf("2. Exit game\n");
        printf("Press enter to continue.");
        int c = _getch();
        while(c != '1' && c != '2' && c != 13)
        {
            c = _getch();
        }
        if(c == '2')
        {
            return;
        }
        clear();

        if(aiMode)
            aiMove();
        else
            turn = 1-turn;
    }


}

bool checkEnd() {
    if(!ships[0])
    {
        return true;
    }
    else if(!ships[1])
    {
        return true;
    }
    return false;
}

void aiMove() {
    int x = rand()%10;
    int y = rand()%10;
    while(toupper(map[0][x][y]) == map[0][x][y])
    {
        x = rand()%10;
        y = rand()%10;
    }
    map[0][x][y] = toupper(map[0][x][y]);
    printf("Computer attacked (%d,%d)!\nYour map:\n" , x,y);
    printMap(0,false);
    fflush(stdin);
    printf("Press any key to continue...");
    _getch();
}

void checkShips(int num) {
    int size = 'j' - 'a' + 1;
    bool aliveShips[size];
    for(int i = 0;i < size;i++)
    {
        aliveShips[i] = false;
    }
    for(int i = 0;i < 10;i++)
    {
        for(int j = 0;j < 10;j++)
        {
            if(map[num][i][j] >= 'a' && map[num][i][j] <= 'j')
            {
                aliveShips[map[num][i][j] - 'a'] = true;
            }
        }
    }
    for(int i = 0;i < size;i++)
    {
        if(aliveShips[i] == true) continue;
        Ship * current = ships[num];
        Ship * before = NULL;
        while(current != NULL && current->name != 'a' + i)
        {
            before = current;
            current = current->next;
        }
        if(current != NULL)
        {
            if(!aiMode || num == 1)
                getCoins(1-num,current->size);
            for(int j = maximum(minimum(current->start_x , current->end_x) - 1, 0) ; j <= minimum(maximum(current->start_x , current->end_x) + 1, 9); j++)
            {
                for(int k = maximum(minimum(current->start_y , current->end_y) - 1, 0) ; k <= minimum(maximum(current->start_y , current->end_y) + 1, 9); k++)
                {
                    map[num][j][k] = toupper(map[num][j][k]);
                }
            }
            if(before != NULL)
                before->next =  before->next->next;
            else
                ships[num] = current->next;
            free(current);
        }
    }

}

void getCoins(int playerNum, int size) {
    switch (size) {
        case 5:
            currentPlayers[playerNum]->coins += 5;
            break;
        case 3:
            currentPlayers[playerNum]->coins += 8;
            break;
        case 2:
            currentPlayers[playerNum]->coins += 12;
            break;
        case 1:
            currentPlayers[playerNum]->coins += 25;
    }
}

void attack(int turn) {
    printf("Enter your target: ");
    int x , y;
    scanf("%d %d" , &x , &y);
    while(x < 0 || y < 0 || x > 9 || y > 9)
    {
        printf("Wrong input. try again: ");
        scanf("%d %d" , &x , &y);
    }
    map[1-turn][x][y] = toupper(map[1-turn][x][y]);
}

void putShipsForAI() {
    createShip(1,5,'a',0,0,0,4);
    createShip(1,3,'b',2,0,2,2);
    createShip(1,3,'c',4,0,4,2);
    createShip(1,2,'d',6,0,6,1);
    createShip(1,2,'e',8,0,8,1);
    createShip(1,2,'f',9,3,9,4);
    createShip(1,1,'g',6,7,6,7);
    createShip(1,1,'h',4,7,4,7);
    createShip(1,1,'i',0,7,0,7);
    createShip(1,1,'j',8,9,8,9);
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
            printf("%d. %s (%d) with %d coins\n" , i , current->name ,current->score , current->coins);
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
        newPlayer->coins = 0;
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

void putShips(int mapNum){
    printf("------- SET YOUR MAP -------\n\n");
    printf(" You Have 10 Ships in total: \n");
    printf(" 1 ship with the size of 5 \n");
    printf(" 2 ships with the size of 3\n");
    printf(" 3 ships with the size of 2\n");
    printf(" 4 ships with the size of 1\n");
    printf("-----------------------------\n");
    getShipCoordinates(5, mapNum, 'a');
    getShipCoordinates(3, mapNum, 'b');
    getShipCoordinates(3, mapNum, 'c');
    getShipCoordinates(2, mapNum, 'd');
    getShipCoordinates(2, mapNum, 'e');
    getShipCoordinates(2, mapNum, 'f');
    getShipCoordinates(1, mapNum, 'g');
    getShipCoordinates(1, mapNum, 'h');
    getShipCoordinates(1, mapNum, 'i');
    getShipCoordinates(1, mapNum, 'j');
}

void createShip(int num , int size , char name , int start_x , int start_y, int end_x , int end_y)
{
    Ship * newShip = (Ship*) malloc(sizeof(Ship));
    newShip->size = size;
    newShip->name = name;
    newShip->start_x = start_x;
    newShip->end_x = end_x;
    newShip->start_y = start_y;
    newShip->end_y = end_y;
    for(int i = minimum(newShip->start_x,newShip->end_x);i <= maximum(newShip->start_x,newShip->end_x);i++)
    {
        for(int j = minimum(newShip->start_y,newShip->end_y);j <= maximum(newShip->start_y,newShip->end_y);j++)
        {
            map[num][i][j] = newShip->name;
        }
    }

    for(int i = maximum(minimum(newShip->start_x , newShip->end_x) - 1,0) ; i <= minimum(maximum(newShip->start_x , newShip->end_x) + 1,9);i++)
    {
        for(int j = maximum(minimum(newShip->start_y , newShip->end_y) - 1,0) ; j <= minimum(maximum(newShip->start_y , newShip->end_y) + 1,9);j++)
        {
            if(map[num][i][j] == WATER)
                map[num][i][j] = NEIGHBOUR;
        }
    }

    if(ships[num] == NULL)
    {
        ships[num] = newShip;
    }
    else
    {
        Ship * current = ships[num];
        while(current->next != NULL)
        {
            current = current->next;
        }
        current->next = newShip;
    }
    newShip->next = NULL;
}

void getShipCoordinates(int size, int mapNum, char n){

    int start_x , start_y , end_x,end_y;
    printf("---------------------------------\n");
    printf("| ->      Ship Size : %d      <- |\n", size);
    printf("---------------------------------\n");

    while(true){
        printf("         Start Location x:");
        scanf("%d", &start_x);
        while(start_x < 0 || start_x > 9)
        {
            printf("Wrong input. try again.");
            scanf("%d", &start_x);
        }
        printf("         Start Location y:");

        scanf("%d", &start_y);
        while(start_y < 0 || start_y > 9)
        {
            printf("Wrong input. try again.");
            scanf("%d", &start_y);
        }

        if(size != 1)
        {
            printf("         End Location x:");
            scanf("%d", &end_x);
            while(end_x < 0 || end_x > 9)
            {
                printf("Wrong input. try again.");
                scanf("%d", &end_x);
            }
            printf("         End Location y:");
            scanf("%d", &end_y);
            while(end_y < 0 || end_y > 9)
            {
                printf("Wrong input. try again.");
                scanf("%d", &end_y);
            }
        }
        else
        {
            end_y = start_y;
            end_x = start_x;
        }


        if(end_x == start_x ){
            int distance = abs(start_y - end_y - 1);
            if(distance != size ){
                printf("!!!ERROR!!! size doesn't match !!!ERROR!!! \n");
                continue;
            }
        }
        else if(start_y == end_y){
            int temp = abs(start_x - end_x - 1);
            if(temp != size ){
                printf("!!!ERROR!!! size doesn't match' !!!ERROR!!! \n");
                continue;
            }
        }
        else{
            printf("!!!ERROR!!! you can't place your ship like this !!!ERROR!!! \n");
            continue;
        }
        bool flag = false;
        for(int i = maximum(minimum(start_x , end_x) - 1,0) ; i <= minimum(maximum(start_x , end_x) + 1,9);i++)
        {
            for(int j = maximum(minimum(start_y , end_y) - 1,0) ; j <= minimum(maximum(start_y , end_y) + 1,9);j++)
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

        break;
    }
    createShip(mapNum,size,n,start_x,start_y,end_x,end_y);

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
                if(map[mapNum][i][j] == 'w')
                {
                    printf("%d %d|" , i , j );
                }
                else
                {
                    printf(" %c |" ,map[mapNum][i][j]);

                }
            }
            printf("\n");
            printf("|___|___|___|___|___|___|___|___|___|___|\n");
        }
    }
    else
    {
        printf("_________________________________________\n");
        for(int i = 0;i < 10;i++)
        {
            printf("|   |   |   |   |   |   |   |   |   |   |\n");
            printf("|");
            for(int j = 0;j < 10;j++)
            {
                if(map[mapNum][i][j] >= 'a' &&  map[mapNum][i][j] <= 'z')
                {
                    printf("%d %d|" , i , j);
                }
                else if(map[mapNum][i][j] >= 'A' && map[mapNum][i][j] <= 'J')
                {
                    if(shipAlive(map[mapNum][i][j],mapNum))
                    {
                        printf(" E |");
                    }
                    else
                    {
                        printf(" C |");
                    }
                }
                else
                {
                    printf(" W |");
                }
            }
            printf("\n");
            printf("|___|___|___|___|___|___|___|___|___|___|\n");
        }
    }
}

bool shipAlive(char name, int num) {
    Ship * current = ships[num];
    while(current != NULL)
    {
        if(current->name == tolower(name))
            return true;
        current = current->next;
    }
    return false;
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
        {
            clear();
            return;
        }
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
    clear();
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
