/*This program has been written by Muhammad Zohaib on 03 Jan 2021
*This program has been written from scratch, without any help seek.
*This program will help to understand multi-threading in C, for beginners
*This is a game, which is made little interesting by adding time limit for the players.

*NOTE:- This program will work in windows as it includes windows.h, copy this program in code-blocks, compile and run.
Any improvement will be appreciated. Thanks!

All rights reserved.
*/
#include <stdio.h>
#include <conio.h>
#include <pthread.h>

#include <time.h>
#include <windows.h>

#define LIGHT 176
#define MEDIUM 177
#define DARK 178



COORD coord;
void gotoxy(int x, int y){
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void delay(int millis)
{
    int start = clock();
    while(clock() - start <= millis);
}

void delays(int secs)
{
    register int start = clock()/1000;
    while(clock()/1000 - start <= secs);
}
void clearCanvas()
{
    system("CLS");
}


void load(int x, int y, char color)
{
    int percent = 0;
    gotoxy(x+40, y-1);
    printf("Loading. . .");
    gotoxy(x, y+1);

    for(int i = 0; i <= 100; i++)
    {
        gotoxy(i+x, y);
        printf("%c", color);
        delay(50);
        gotoxy(x+40+2, y+1);
        printf("%d%c",percent++,'%');

    }
    gotoxy(0, y+10);
}

void autoLoad(int x, int y)
{
    int percent = 0;
    gotoxy(x+40, y-1);
    printf("Loading. . .");
    gotoxy(x, y+1);

    for(int i = 0; i<=100; i++)
    {
        gotoxy(i+x,y);
        printf("%c",LIGHT);
    }
    for(int i = 0; i <= 100; i++)
    {
        gotoxy(i+x, y);
        printf("%c", DARK);
        delay(20);
        gotoxy(x+40+2, y+1);
        printf("%d%c",percent++,'%');

    }
    gotoxy(0, y+10);
}


typedef struct
{
    int turn;
    char entries[3][3];
    char entry;
    int win;
}PLAYER;

//Global players
PLAYER player1;
PLAYER player2;

int count = 0;
int secs = 3;
char ch = 'A';

pthread_t id;

char numbering[3][3] = {{'1','2','3'},
                        {'4','5','6'},
                        {'7','8','9'}};

void drawStruct()
{
    //Drawing spaces
    printf("\n");
    for(int i = 2; i < 20; i++)
    {
        if(i%7==0)
            printf("%c", 186);
        else
            printf("%c", ' ');
    }
    printf("\n");
    /****************End of spaces****************************/
    for(int i = 2; i < 20; i++)
    {
        if(i%7==0)
            printf("%c", 206);
        else
            printf("%c", 205);
    }
    printf("\n");
    /********************************************/

    for(int i = 2; i < 20; i++)
    {
        if(i%7==0)
            printf("%c", 186);
        else
            printf("%c", ' ');
    }
    printf("\n");

    /****************End of spaces****************************/
    for(int i = 2; i < 20; i++)
    {
        if(i%7==0)
            printf("%c", 206);
        else
            printf("%c", 205);
    }
    printf("\n");
    /********************************************/
    for(int i = 2; i < 20; i++)
    {
        if(i%7==0)
            printf("%c", 186);
        else
            printf("%c", ' ');
    }
    printf("\n");
    /****************END of Spaces**********************/
}

void setNumbering()
{
    gotoxy(2,1);
    printf("%c", numbering[0][0]);

    gotoxy(8,1);
    printf("%c", numbering[0][1]);

    gotoxy(15,1);
    printf("%c", numbering[0][2]);

    gotoxy(2,3);
    printf("%c", numbering[1][0]);

    gotoxy(8,3);
    printf("%c", numbering[1][1]);

    gotoxy(15,3);
    printf("%c", numbering[1][2]);

    gotoxy(2,5);
    printf("%c", numbering[2][0]);

    gotoxy(8,5);
    printf("%c", numbering[2][1]);

    gotoxy(15,5);
    printf("%c", numbering[2][2]);
}

void createPlayers()
{
    printf("                             Player 1: X         :        Player 2: O");
    player1.turn = 1;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            player1.entries[i][j] = i+j;

    player2.turn = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            player2.entries[i][j] = i+j;
}

void setEnteries(PLAYER* player, char n)
{
    switch(n)
    {
    case '1':
        player->entries[0][0] = 'x';
        break;
    case '2':
        player->entries[0][1] = 'x';
        break;
    case '3':
        player->entries[0][2] = 'x';
        break;
    case '4':
        player->entries[1][0] = 'x';
        break;
    case '5':
        player->entries[1][1] = 'x';
        break;
    case '6':
        player->entries[1][2] = 'x';
        break;
    case '7':
        player->entries[2][0] = 'x';
        break;
    case '8':
        player->entries[2][1] = 'x';
        break;
    case '9':
        player->entries[2][2] = 'x';
        break;
    default:
        clearCanvas();
        printf("Error Occurred!!!");
    }
    count++;
}


int replaceNumWith(char n, char c)
{
    int errorFound = 0;
    switch(n)
    {
    case '1':
        if(numbering[0][0] != 'X' && numbering[0][0] != 'O')
            numbering[0][0] = c;
        else
            errorFound = 1;
        break;
    case '2':
        if(numbering[0][1] != 'X' && numbering[0][1] != 'O')
            numbering[0][1] = c;
        else
            errorFound = 1;
        break;
    case '3':
        if(numbering[0][2] != 'X' && numbering[0][2] != 'O')
            numbering[0][2] = c;
        else
            errorFound = 1;
        break;
    case '4':
        if(numbering[1][0] != 'X' && numbering[1][0] != 'O')
            numbering[1][0] = c;
        else
            errorFound = 1;
        break;
    case '5':
        if(numbering[1][1] != 'X' && numbering[1][1] != 'O')
            numbering[1][1] = c;
        else
            errorFound = 1;
        break;
    case '6':
        if(numbering[1][2] != 'X' && numbering[1][2] != 'O')
            numbering[1][2] = c;
        else
            errorFound = 1;
        break;
    case '7':
        if(numbering[2][0] != 'X' && numbering[2][0] != 'O')
            numbering[2][0] = c;
        else
            errorFound = 1;
        break;
    case '8':
        if(numbering[2][1] != 'X' && numbering[2][1] != 'O')
            numbering[2][1] = c;
        else
            errorFound = 1;
        break;
    case '9':
        if(numbering[2][2] != 'X' && numbering[2][2] != 'O')
            numbering[2][2] = c;
        else
            errorFound = 1;
        break;
    }

    return errorFound;
}

void checkWin(PLAYER* player)
{
    if(player->entries[0][0] == 'x' && player->entries[0][1] == 'x' && player->entries[0][2] == 'x')
        player->win = 1;
    else if(player->entries[1][0] == 'x' && player->entries[1][1] == 'x' && player->entries[1][2] == 'x')
        player->win = 1;
    else if(player->entries[2][0] == 'x' && player->entries[2][1] == 'x' && player->entries[2][2] == 'x')
        player->win = 1;
    else if(player->entries[0][0] == 'x' && player->entries[1][0] == 'x' && player->entries[2][0] =='x')
        player->win = 1;
    else if(player->entries[0][1] == 'x' && player->entries[1][1] == 'x' && player->entries[2][1] =='x')
        player->win = 1;
    else if(player->entries[0][2] == 'x' && player->entries[1][2] == 'x' && player->entries[2][2] =='x')
        player->win = 1;
    else if(player->entries[0][0] == 'x' && player->entries[1][1] == 'x' && player->entries[2][2] =='x')
        player->win = 1;
    else if(player->entries[0][2] == 'x' && player->entries[1][1] == 'x' && player->entries[2][0] =='x')
        player->win = 1;
}

void declareWinner()
{
    if(player1.win)
        {
            printf("Player 1 Wins!\n\n");
        }
        else if(player2.win)
        {
            printf("Player 2 Wins!\n\n");
        }
}

void* timerThread(void* vargp)
{
    while(secs >= 0)
    {
        gotoxy(5, 15);
        printf("Time : %d", secs);
        delay(1000);
        secs--;
    }
    if(ch == 'A')
    {
        MessageBox(0,"You lose your turn", "TIME UP!", 1);
        gotoxy(0,6);
        printf("press any key to continue. . .");
        gotoxy(0,5);
    }
}

void setTimer()
{
    secs = 3;
    id = pthread_self();
    pthread_create(&id,NULL,timerThread,NULL);
}

int main()
{
    autoLoad(10,10);
    clearCanvas();

    createPlayers();
    drawStruct();
    setNumbering();

    setTimer();
    while(player1.win != 1 && player2.win != 1)
    {
        if(player1.turn)
        {
            replayOne:
            if(secs < 0)
                setTimer();

            ch = 'A';
            printf("\nPlayer 1 Select a number above: ");
            secs = 4;
            fflush(stdin);
            ch = player1.entry = getch();

            if(secs < 0)
                goto replayTwo;

            if(replaceNumWith(player1.entry, 'X') == 1)
            {
                MessageBox(0,"Already occupied!!!", "ERROR!", 1);
                 setNumbering();
                 gotoxy(0, 5);
                goto replayOne;
            }
            setEnteries(&player1, player1.entry);
            player1.turn = 0;
            player2.turn = 1;
        }
        else if(player2.turn)
        {
            replayTwo:
            if(secs < 0)
                setTimer();

            ch = 'A';
            printf("\nPlayer 2 Select a number above: ");
            secs = 4;
            fflush(stdin);
            ch = player2.entry = getch();

            if(secs < 0)
                goto replayOne;

            if(replaceNumWith(player2.entry, 'O') == 1)
            {
                MessageBox(0,"Already occupied!!!", "ERROR!", 1);
                setNumbering();
                gotoxy(0, 5);
                goto replayTwo;
            }
            setEnteries(&player2, player2.entry);
            player1.turn = 1;
            player2.turn = 0;
        }
        setNumbering();
        checkWin(&player1);
        checkWin(&player2);

        if(count >= 9 && player1.win != 1 && player2.win != 1)
        {
            gotoxy(12, 15);
            printf("Game Ended. . .Its Draw\n\n");
            return 0;
        }
    }

    gotoxy(12, 15);
    printf("Game Ended. . .");
    declareWinner();
    return 0;
}
