/*
* Project : Console Based Gaming System
* Name    : Muhammad Zohaib
* Reg#    : FA20-BSE-051
* Section : B
* email   : codezpatch@gmail.com
* COMSATS UNIVERSITY ISLAMABAD, LAHORE CAMPUS
*Its a university project created for DSA class.
*
*/
//Date modified : DEC 24, 2021 10:45 pm

/*Any external Library --NOT-- used*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>

#define playerDataFile "playerData.csv"
#define playerDataFormat "%s , %s \n"
#define paintDataFile "paintData.dat"
#define paintDataFormat "%d %d %d %d\n"
#define scoreDataFile "scoreData.csv"
#define scoreDataFormat "%s , %s , %d \n"

/************************************************* Start Universal Constants ******************************************/
//game width and height
#define WIDTH 70
#define HEIGHT 30
#define FPS 60

//arrow keys
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

#define SPACE 32

#define SOLID 219

#define QUIT 27 // ascii for esc

//colors
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGNETA 5
#define BROWN 6
#define YELLOW 14
#define WHITE 15

#define passlen 8

#define _WIN32_WINNT 0x0500

const int colorful[] = {BLUE, GREEN, CYAN, RED, MAGNETA, BROWN, YELLOW, WHITE};

/************************************************* End Universal Constants ******************************************/


/************************************************* Start Paint Constants ******************************************/
#define MARKER 178
#define PEN 177
#define LEAD 176
#define PENUP 0
#define ERASER ' '

#define PICKUP 'u'
#define ERASE 'e'
#define SELECT_MARKER 'm'
#define SELECT_PEN 'p'
#define SELECT_LEAD 'l'  //lead pencil

#define paintAllowed 1
/************************************************* End Paint Constants ******************************************/

/************************************************* Start Space Game Constants ******************************************/
#define MAG_CAPACITY 3  //magzine size of ship
#define BULLET_SPEED 0.3f
#define ENEMY_SPEED 0.03f
#define MAX_ENEMIES (HEIGHT*WIDTH)/300  //generate enemies strength depending on screen area
/************************************************* End Space Game Constants ******************************************/
#define cd 7
#define rd 6

/************************************************* Start Universal ******************************************/
struct WALL{
    int w;
    int h;
};

struct WALL wall = {2,1};

void createWall();
void clearCanvas();
void load();
void clearArea(int x, int y, int w, int h);  //clear the specified area
void drawMenuBox(char opts[][100], int n, char name[]);  //draw menu box at center
void drawMsgBox(int x, int y,char opts[][100], int n, char name[]);  //draw Message box at x,y position
void table(int x, int y, int rowCells, int comCells, int cellWd, int cellHt, int mode);  //self defined function for drawing visual table
void drawString(int x, int y, char* str);  //draw string at desired at desired position

//take customized string input
void myStrInput(char* str);
/************************************************* End Universal ******************************************/


/************************************************* Account management  ******************************************/
struct PLAYER{
    char username[passlen+1];
    char password[passlen+1];
    struct PLAYER* next;
    struct PLAYER* prev;
};

struct PLAYER* playerList =  NULL;
struct PLAYER* player;

void addPlayer(char username[], char password[], int saveToFile);
void delAccount();

void reg();  //register
int login();  //login

void loadAccounts(); //load the accounts to player list so that we don't need to open the file again and again

/************************************************* End Account Management ******************************************/

/************************************************* Start Paint  ******************************************/

int colorNow = WHITE, mainColor;
struct POINT{
    int x;
    int y;
    char tool;
    int color;
};

struct POINT point = {0,0, PEN, WHITE};
struct PAINTNODE{
    struct POINT point;
    struct PAINTNODE* next;
};

struct PAINTNODE* createNode(struct POINT);
void insert(struct POINT);
void animatePaint();
void draw(int x, int y, char tool, int color);
void runPaint();
void printPaintLabel(char tool);
void selectColor();
void reloadPaintBuffer();
void savePlayerPaintData();

struct PAINTNODE* paintHead = NULL;

/************************************************* End Paint  ******************************************/

/************************************************* Start Space Game ******************************************/
struct BULLET{  //bullet for magzine of ship
    int x;
    float y;
    char shape;
    void* direction;
};

struct MAG{   //magzine for ship
    int f;
    int r;
    int n;  //number of bullets available
    struct BULLET que[MAG_CAPACITY];
};

struct SHIP{  //ship itself
    int x;
    int y;
    int wing;
    char material;
    struct MAG mag;
};

struct ENEMY{
    int x;
    float y;
    int w;  //width  of enemy
    int h;  //height of enemy
    int id;  //unique id of every enemy
    char material;
    struct ENEMY* next;
    struct ENEMY* prev;
};

//variables
int score = 0;
char isSpaceGamePlaying = 0;
char enemiesTypes[] = {206, 2, 64, 31, 178};
int enemyPWR = 0;

//related to enemy
struct ENEMY* createEnemy();
void removeEnemy(struct ENEMY* deadEnemy);
void createEnemies();
void drawEnemies();
void eraseEnemies();
void saveScore(int score);
int checkHit(struct BULLET* thisBullet);  //check hit for every bullet in drawbullets() &  return score according to enemy power

struct ENEMY* enemies = NULL;  //list of enemies
//create ship object
struct SHIP ship;
//related to ship
void createShip();
void drawShip();
void eraseShip();
void drawBullets(struct MAG* mag);
void eraseBullets(struct MAG* mag);
void addBullet(int x, int y, struct MAG* mag);  //add bullet to rear of queue
void removeBullet(struct MAG* mag);  //removes bullet from front of queue
//struct BULLET* createBullet(int x, int y);
void runSpaceGame();

/************************************************* End Space Game  ******************************************/

/*************************** Start Connect 4 ******************************************/
int grid[rd+1][cd];
int isPlayingConnect4 = 0;
char turn = 'X';

void drawGrid();
void resetGrid();
void pushPlayer(int stackID);
int checkWinner();
void startConnect4();
/*************************** End Connect 4 ********************************************/




/*************************** Console Settings ******************************************/

HANDLE outHnd;

COORD coord;
void gotoxy(int x, int y){
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setColor(int colorID);
/*************************** End Console Settings ******************************************/


/**************************************** S T A R T     M A I N     F U N C T I O N ****************************************/
int main()
{
    ///prepare console
    outHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT winSize = {0, 0, WIDTH, HEIGHT};
    SetConsoleWindowInfo(outHnd, 1, &winSize);
    SetConsoleTitle("Console Based Gaming System");
    COORD bufferSize = {WIDTH, HEIGHT};
    SetConsoleScreenBufferSize(outHnd, bufferSize);

    //HWND consoleWindow = GetConsoleWindow();
    //SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
///end preparing console

    //seed the random value generator
    srand(time(NULL));
    setColor(GREEN);
    //setColor(colorNow);

    loadAccounts(); //load accounts from file to memory
    char accountOpts[][100] = {
        "1. Register",
        "2. Login",
        "3. Delete Account",
    };

    int opt, loggedIn = 0;
    start:
    do
    {
        clearCanvas();
        drawMenuBox(accountOpts, 3, " Account ");
        opt = getch();
        switch(opt)
        {
            case '1': reg();
            break;
            case '2': loggedIn = login();
            break;
            case '3': delAccount();
            break;
            // case '4':
            // {
            //     system("cls");
            //     struct PLAYER* tempPlayer = playerList;
            //     int count = 0;
            //     while(tempPlayer != NULL)
            //     {
            //         printf("\n%8s %8s\n", tempPlayer->username, tempPlayer->password);
            //         tempPlayer = tempPlayer->next;
            //         count++;
            //     }
            //     table(0,0, 2, count, 9, 2, 0);
            //     getche();
            // }
            // break;
            case QUIT:
                return -1;
        }
    }while(!loggedIn);

    //seed the random value generator
    srand(time(NULL));

    clearCanvas();
    load();
    char gameOpts[][100] = {
        "1. Paint",
        "2. Space Game",
        "3. Connect 4 Game",
        "4. Change Theme",
        "5. Logout"
    };
    do
    {
        clearCanvas();
        char label[100] = " Welcome ";
        strcat(label, player->username);
        strcat(label, " ");
        drawString(WIDTH/2-strlen(label)/2, 0, label);
        drawMenuBox(gameOpts, 5, " Main Menu ");
        opt = getch();
        switch(opt)
        {
            case '1': runPaint(); paintHead =  NULL;
            break;
            case '2': runSpaceGame();
            break;
            case '3': startConnect4();
            break;
            case '4': clearCanvas(); load();
            break;
            case '5':
                //logout
                player = NULL;
                loggedIn = 0;
                goto start;
            break;
        }
    }while(opt != QUIT);
    return 0;
}

/******************************  E N D     M A I N     F U N C T I O N ********************************************/


/************************************************* Universal Functions ******************************************/

void myStrInput(char* str)
{
    char c;
    int i = 0;
    while(1)
    {
        c = getche();
        switch (c)
        {
        case 8:  // 8 = backspace
            if(i > 0)
            {
                printf("_\b");
                i--;
            }else
            {
                printf("_"); //don't allow user to go back and back on backspace
            }
            break;
        case 13:  // 13 = enter
            str[i++] = '\0';
            return;
        case QUIT:
            *str = NULL;
            return;
        default:
            if(i < passlen)
                str[i++] =  c;
            else
                printf("\b_\b");
            break;
        }
    }
}

void clearCanvas()
{
    gotoxy(0,0);
    system("cls");
    createWall();
}

void clearArea(int x, int y, int w, int h)
{
    for(int i = y; i <= y+h; i++)
    {
        gotoxy(x, i);
        for(int j = x; j <= x+w; j++)
        {
            printf(" ");
        }
    }
}

void drawString(int x, int y, char* str)
{
    gotoxy(x, y);
    printf(str);
}
void createWall()
{
    for(int i = 0; i <= HEIGHT; i++)
    {
        for(int j = 0; j <= WIDTH; j++)
        {
            if(i == 0 || i == HEIGHT)
                printf("%c", SOLID);
            else if((j == WIDTH-wall.w || j == 0) && (i != 0 && i != HEIGHT))
                printf("%c%c", SOLID, SOLID);
            else
                printf(" ");
        }
        printf("\n");
    }
    gotoxy(0,0);
}

void load()
{
    //todo
    srand(time(NULL));
    setColor(colorful[rand()%(sizeof(colorful)/sizeof(colorful[0]))]);
    int percent = 50;
    int x = WIDTH/2-percent/2, y = HEIGHT/2;
    int i;
    char label[100] = "Console Gaming System Loading...";
    gotoxy(WIDTH/2-strlen(label)/2, y-1);
    printf(label);
    for (i = 0; i < percent; i++)
    {
        gotoxy(WIDTH/2,y+1);
        printf("%d %c", i*100/percent, '%');
        gotoxy(x+i,y);
        printf("%c", SOLID);
        Sleep(30);
    }

}
void drawMenuBox(char opts[100][100], int n, char name[])
{
    int maxLen = 0;
    for(int i = 0; i < n; i++)  //calculate max str len in opts
    {
        int len = strlen(opts[i]);
        if(len > maxLen)
            maxLen = len;
    }
    int x = WIDTH/2-maxLen/2, y = HEIGHT/2;
    table(x-2, y-n-2, 1, 1, maxLen+3, n+4, 0);
    for(int i = 0; i < n; i++)
    {
		drawString(x, y-n+i, opts[i]);
    }

	drawString(x+2, y-n-2, name);  //menu label
	drawString(x, y+1, "ESC to Exit");
}

void drawMsgBox(int x, int y, char opts[100][100], int n, char name[])
{
    int maxLen = 0;
    for(int i = 0; i < n; i++)  //calculate max str len in opts
    {
        int len = strlen(opts[i]);
        if(len > maxLen)
            maxLen = len;
    }

    table(x, y, 1, 1, maxLen+3, n+2, 0);
    for(int i = 0; i < n; i++)
    {
		drawString(x+2, y+i+1, opts[i]);
    }

	drawString(x+2, y, name);  //msg label
	drawString(x+2, y+n+1, "ESC to Continue");
    while (getch() != QUIT);
        //JUST WAIT TILL USER PRESS QUIT

    clearArea(x, y, maxLen+3, n+2);
}

void table(int x, int y, int rowCells, int comCells, int cellWd, int cellHt, int mode)
{
    //205 =  '='
    //186 = '||'
    //206 = '++'

    // 196 = '-'
    // 179 = '|'
    // 197 = '+'
    char hor, ver, jun;
    hor = mode == 0? 205 : 196;
    ver = mode == 0? 186 : 179;
    jun = mode == 0? 206 : 197;

    int yt = y;
    int xt = x;

    for(int i = 0; i <= comCells; i++)
    {
        gotoxy(x,y);
        for(int j = 0; j < rowCells*cellWd; j++)
        {
            printf("%c",hor);
        }
        y += cellHt;
    }

    y = yt;  //resetting

    for(int i = 0; i <= rowCells; i++)
    {
        y = yt;
        xt = y;  //for the case
        gotoxy(x,y);
        for(int j = 0; j <= comCells*cellHt; j++)
        {
            gotoxy(x,y++);
            if(j+yt == xt)
            {
                printf("%c",jun);
                xt+=cellHt;
            }
            else
            printf("%c",ver);
        }
        x += cellWd;
    }
    printf("\n");
}

void setColor(int colorID)
{
    if(paintAllowed)
    {
        SetConsoleTextAttribute(outHnd, colorID);
        mainColor = colorID;
    }
}

void selectColor()
{
    clearArea(wall.w, wall.h, WIDTH-wall.w*2, HEIGHT-wall.h*2);
    int color = WHITE;
    char label[20];
    int i;

    int nofColors = sizeof(colorful)/sizeof(colorful[0]);
    for(i = 0; i < nofColors; i++)
    {
        sprintf(label, "%d   %c", i, SOLID);
        setColor(colorful[i]);
        drawString(WIDTH/2-2, HEIGHT/2-nofColors+i*2, label);
    }
    setColor(WHITE);
    table(WIDTH/2-4, HEIGHT/2-nofColors-1, 2, nofColors, 4, 2, 1);
    drawString(WIDTH/2-6, HEIGHT/2-nofColors-2, "Select color");
    color = getch()-48;
    while (color < 0 || color >= nofColors)
    {
        color = (getch()-48);
    }
    colorNow = colorful[color];
    setColor(colorNow);
    clearArea(wall.w, wall.h, WIDTH-wall.w*2, HEIGHT-wall.h*2);
    reloadPaintBuffer();  //we had gone to color menu, so again load the drawing.
    setColor(colorNow);
}
/*************************************************** End Universal Functions ***********************************************/

/*************************************************** START  Paint Functions ***********************************************/
void doPainting(){
    clearCanvas();
    const char label[] = " Paint ";
    drawString(WIDTH/2 - strlen(label)/2, 0, label);
    char tool = PEN; //default in write mode
    printPaintLabel(tool);
    point.x = WIDTH/2-1;  //set point to center x
    point.y = HEIGHT/2-1;  //set point to center y

    //draw previous drawing if existed
    reloadPaintBuffer();
    //start drawing
    draw(point.x, point.y, tool, point.color);  //show the user, where to draw
    while(1)
    {
        if(kbhit())
        {
            char c = getch();
            if(c=='q')
                return;
            switch(c)
            {
            case UP:
                --point.y;
                if(point.y < wall.h+2)
                    point.y = wall.h+2;
                break;
            case DOWN:
                ++point.y;
                if(point.y > HEIGHT-wall.h)
                    point.y = HEIGHT-wall.h;
                break;
            case LEFT:
                --point.x;
                if(point.x < wall.w)
                    point.x = wall.w;
                break;
            case RIGHT:
                ++point.x;
                 if(point.x > WIDTH-wall.w)
                    point.x = WIDTH-wall.w;
                break;
            case PICKUP:
                tool = PENUP;
                printPaintLabel(tool);
                break;
            case ERASE:
                tool = ERASER;
                break;
            case SELECT_PEN:
                tool = PEN;
                printPaintLabel(tool);
                continue; // no need to write when tool gets changed for first time
                break;
            case SELECT_MARKER:
                tool = MARKER;
                printPaintLabel(tool);
                continue;
                break;
            case SELECT_LEAD:
                tool = LEAD;
                break;
            case 'c':
                selectColor();
                break;
            case 'n':
                {
                    //delete the previous data if new page
                    struct PAINTNODE* pathPtr = paintHead;
                    while(pathPtr !=  NULL)
                    {
                        paintHead = pathPtr->next;
                        free(pathPtr);
                        pathPtr = paintHead;
                    }
                    paintHead = NULL;
                    clearArea(wall.w, wall.h+1, WIDTH-wall.w*2, HEIGHT-(wall.h)*2-1);
                }
                break;
            case QUIT:
                savePlayerPaintData();
                return;
            default:
                continue;  // handle -32 error
            }
            printPaintLabel(tool);
            if(tool != PENUP)
            {
                point.tool = tool;
                point.color = colorNow;
                draw(point.x, point.y, tool, point.color);
                insert(point);  //insert point to list for later drawing
            }
            gotoxy(point.x, point.y);
            //printf() func pushes the cursor forward, so bring it back to original postion
        }
        Sleep(1);  //give rest to CPU
    }

}

void reloadPaintBuffer()
{
    struct PAINTNODE* pathPtr = paintHead;  //pointer to drawing path
    while(pathPtr != NULL)
    {
        struct POINT p = pathPtr->point;
        draw(p.x, p.y, p.tool, p.color);
        //move the point where the previous drawing had gone
        point.x = p.x;
        point.y = p.y;
        point.tool = p.tool;
        pathPtr = pathPtr->next;  //move the pointer to next pixel/block of path
    }
}

void savePlayerPaintData()
{
    struct PAINTNODE* pathPtr = paintHead;
    char filename[100];
    strcpy(filename, player->username);
    strcat(filename, "_");
    strcat(filename, paintDataFile);
    FILE* file = fopen(filename, "w");
    while(pathPtr != NULL)
    {
        struct POINT p = pathPtr->point;
        fprintf(file, paintDataFormat, p.x, p.y, p.tool, p.color);
        pathPtr = pathPtr->next;
    }
    fclose(file);
}

struct PAINTNODE* createNode(struct POINT point){
    struct PAINTNODE* newNode = (struct PAINTNODE*) malloc(sizeof(struct PAINTNODE));
    newNode->point = point;
    newNode->next = NULL;
    return newNode;
};

void insert(struct POINT point)
{
    static struct PAINTNODE* lastPaintNode = NULL;  //we need to save this variable for next recall
    //it will remember the last node so that we can insert at end again
    struct PAINTNODE* temp = createNode(point);
    if(paintHead == NULL)
    {
        paintHead = temp;
        lastPaintNode = temp;
    }
    else
    {
        lastPaintNode->next = temp;
        lastPaintNode = temp;
    }
}

void animatePaint()
{
    system("cls");
    struct PAINTNODE* temp = paintHead;
    int maxHEIGHT = 0;  //to keep the track how far is y has gone, to print last message
    createWall();
    const char label[] = " Animating Paint ";
    drawString(WIDTH/2 - strlen(label)/2, 0, label);
    while(temp != NULL)
    {
        struct POINT p = temp->point;
        draw(p.x, p.y, p.tool, p.color);
        Sleep(1000/FPS);

        //calcuate max HEIGHT at which the paint has gone, so we can print drawn Successfully
        temp = temp->next;
        if(p.y > maxHEIGHT)
            maxHEIGHT = p.y;
    }
    const char label2[] = " Press Any Key to Continue ";
    drawString(WIDTH/2 - strlen(label2)/2, HEIGHT, label2);
    getch();
}

void draw(int x, int y, char tool, int color)
{
    setColor(color);
    gotoxy(x, y);
    printf("%c", tool);
}

void runPaint()
{
    char filename[100];
    strcpy(filename, player->username);
    strcat(filename, "_");
    strcat(filename, paintDataFile);
    FILE* file = fopen(filename, "r");
    if(file != NULL)
    {
        struct POINT p;
        while(fscanf(file, paintDataFormat, &p.x, &p.y, &p.tool, &p.color) != EOF)
            insert(p);
    }
    fclose(file);
    //reload data from file
    clearCanvas();
    char opt;
    do
    {
        createWall();
        char opts[][100] = {
                        "1. Start Painting\n",
                        "2. Start Animation\n",
                        };
        drawMenuBox(opts, 2, " PAINT ");

        opt = getch();
        switch(opt)
        {
        case '1':
            doPainting();
            break;
        case '2':
            animatePaint();
            break;
        default:
            if(opt != QUIT)
                printf("Select Valid option!");
        }
        clearCanvas();
    }while(opt != QUIT);
}

void printPaintLabel(char tool)
{
    gotoxy(2,2);
    printf("Tool: %c - Tools: m,p,l,e,u - new : n - colors : c - Quit : 'esc'", tool==ERASER? ERASE: tool);
}
/*************************************************** End Paint Functions ***********************************************/

/*************************************************** Start Space Game Functions ***********************************************/
void runSpaceGame()
{
    char scoreLabel[30];
    //prepare environment for game
    clearCanvas();
    setColor(BLUE);
    createWall();
    createShip();
    drawShip();

    createEnemies();
    drawEnemies();
    isSpaceGamePlaying = 1;

    score = 0;
    char btn;
    do
    {
        if(kbhit())
        {
            eraseShip();
            btn = getch();
            switch(btn)
            {
                case LEFT: if(ship.x+ship.wing > wall.w) ship.x--; else ship.x = WIDTH-wall.w;
                break;
                case RIGHT:if(ship.x < WIDTH-wall.w) ship.x++; else ship.x = wall.w;
                break;
                case SPACE: if(ship.mag.n > 0)addBullet(ship.x+1, ship.y-1, &ship.mag);
                break;
                case QUIT: enemies = NULL;
            }
            drawShip();
        }
        eraseBullets(&ship.mag);
        drawBullets(&ship.mag);
        eraseEnemies();
        drawEnemies();
        score += checkHit(&ship.mag);
        sprintf(scoreLabel, " SCORE : %d ", score);
        drawString(WIDTH-wall.w-strlen(scoreLabel), wall.h+1, scoreLabel);
        Sleep(1);
    }while(btn != QUIT && isSpaceGamePlaying);
    saveScore(score);
    char gameEndMenu[][100] =
    {
        "G A M E   O V E R !",
        " "
    };
    drawMenuBox(gameEndMenu, 2, scoreLabel);
    while(getch() != QUIT);  //scoreboard will not go till user exits
}
void createShip()
{
     //create ship object
    struct MAG mag = {-1, -1, MAG_CAPACITY};
    ship.x = WIDTH/2;
    ship.y = HEIGHT-wall.h-2;
    ship.wing = 2;
    ship.material = SOLID;
    ship.mag = mag;
    //create mag object
    //load mag to ship
    //ship.mag = mag;
}

void drawShip()
{
    setColor(RED);
    gotoxy(ship.x+1, ship.y);
    printf("%c", ship.material);  //draw head
    gotoxy(ship.x, ship.y+1);
    printf("%c%c%c", ship.material, ship.material, ship.material);  //draw body
}

void eraseShip()
{
    gotoxy(ship.x+1, ship.y);
    printf(" ");
    gotoxy(ship.x, ship.y+1);
    printf("   ");
}

struct BULLET createBullet(int x, int y)
{
    struct BULLET newBullet;
    newBullet.x = x;
    newBullet.y = y;
    newBullet.shape = 127;
    newBullet.direction = &ship.y;
    return newBullet;
}

void addBullet(int x, int y, struct MAG* mag)
{
    struct BULLET newBullet = createBullet(x, y);
    if((mag->f == 0 && mag->r == MAG_CAPACITY-1) || mag->f == mag->r+1)
        return; //queue overflow

    if(mag->f == -1)
        mag->f = mag->r = 0;
    else if(mag->r == MAG_CAPACITY-1)
        mag->r = 0;
    else
        mag->r++;

    mag->que[mag->r] = newBullet;
    mag->n--;  //bullets removed from mag and added to que to continue
}

void removeBullet(struct MAG* mag)
{
    if(mag->f == -1)
        return; //queue is empty

    mag->que[mag->f].direction = NULL;  //reset direction of bullet to null
    ++mag->n;

    if(mag->f == mag->r)
        mag->f = mag->r = -1;
    else if(mag->f == MAG_CAPACITY-1)
        mag->f = 0;
    else
        mag->f++;
}

void drawBullets(struct MAG* mag)
{
    setColor(YELLOW);
    char label[30];
    sprintf(label, "Bullets : %d", mag->n);
    drawString(wall.w+1,wall.h+1, label);
    int i;

    if(mag->que[mag->f].y < wall.h)  // if front bullet reaches the wall height, then remove it from fired bullets
        removeBullet(mag);

    for(i = 0; i < MAG_CAPACITY; i++)
    {
        struct BULLET* b = &(mag->que[i]);
        if(b->y > wall.h && b->direction != NULL)
        {
            gotoxy(b->x, (b->y) -= BULLET_SPEED);
            printf("%c", b->shape);
        }
        else
            b->y = -BULLET_SPEED;
        if(checkHit(b))
            b->direction = NULL;
    }
}

void eraseBullets(struct MAG* mag)
{
    int i;
    for(i = 0; i < MAG_CAPACITY; i++)
    {
        struct BULLET* b = &(mag->que[i]);
        if(b->y >= wall.h)
        {
            gotoxy(b->x, b->y);
            printf(" ");
        }
    }
}

struct ENEMY* createEnemy()
{
    static char id = '1';  //assign an id for every new enemy
    struct ENEMY* newEnemy = (struct ENEMY*) malloc(sizeof(struct ENEMY));
    newEnemy->id = id++;
    newEnemy->w = 2;
    newEnemy->h = 1;
    newEnemy->x = wall.w + (rand()%(WIDTH-wall.w-newEnemy->w)) ;
    newEnemy->y = wall.h+2 + (rand()%(HEIGHT/2));
    newEnemy->material = enemiesTypes[enemyPWR];
    newEnemy->next = NULL;
    newEnemy->prev = NULL;
    return newEnemy;
}

void createEnemies()
{
    enemies = NULL;
    struct ENEMY* prevEnemy = enemies;
    for(int i = 0; i < MAX_ENEMIES; i++)
    {
        if(prevEnemy == NULL)
        {
            enemies = createEnemy();
            prevEnemy = enemies;
        }
        else
        {
            struct ENEMY* newEnemy = createEnemy();
            prevEnemy->next = newEnemy;
            newEnemy->prev = prevEnemy;
            prevEnemy = newEnemy;
        }
    }
    if(enemyPWR < sizeof(enemiesTypes)/sizeof(enemiesTypes[0])-1)
        enemyPWR += 1;
}

void drawEnemies()
{
    setColor(GREEN);
    struct ENEMY* temp = enemies;
    while(temp != NULL)
    {
        if(temp->y >= ship.y)
            isSpaceGamePlaying = 0;  //stop the game
            //game is over as enemies has landed on ship

        gotoxy(temp->x, temp->y += ENEMY_SPEED);
        for(int i = 0; i < temp->w; i++)
            printf("%c", temp->material);
        temp = temp->next;
    }
}

void eraseEnemies()
{
    struct ENEMY* temp = enemies;
    while(temp != NULL)
    {
        gotoxy(temp->x, temp->y);
        for(int i = 0; i < temp->w; i++)
            printf(" ");
        temp = temp->next;
    }
}

void saveScore(int score)
{
    time_t t;
    time(&t);
    // char filename[100];
    // strcpy(filename, player->username);
    // strcat(filename, "_");
    // strcat(filename, scoreDataFile);
    // FILE* file = fopen(filename, "a");
    FILE* file = fopen(scoreDataFile, "a");
    char date[100];
    strcpy(date, ctime(&t));
    date[strlen(date)-1] = '\0';  //remove newline from default format
    fprintf(file, scoreDataFormat, player->username, date, score);
    fclose(file);
}

int checkHit(struct BULLET* thisBullet)
{
    struct ENEMY* ene = enemies;
    while(ene != NULL && thisBullet->direction != NULL)
    {
        struct ENEMY* nextEnemy = ene->next;
        if(thisBullet->y <= ene->y && thisBullet->y >= ene->y-1 && thisBullet->x >= ene->x && thisBullet->x < ene->x+ene->w)
        {
            gotoxy(ene->x, ene->y);
            for(int i = 0; i < ene->w; i++)
                printf(" ");
            removeEnemy(ene);  //remove this dead enemy from list
            thisBullet->direction = NULL;  //bullet got hit
            score += (enemyPWR+1)*10;
            return 1;
        }
        ene = nextEnemy;  //check for next enemy hit with the same bullet
    }
    return 0;
}

void removeEnemy(struct ENEMY* deadEnemy)
{
    //if first enemy
    if(deadEnemy == enemies)
    {
        if(deadEnemy->next == NULL)  //if no more enemies
        {
            enemies = NULL;
            free(enemies);
            //No enemy left? then create new Enemies
            createEnemies();
        }
        else
        {
            enemies = deadEnemy->next;  //if there are still enemies after first enemy
            free(deadEnemy);
        }
        return;
    }

    deadEnemy->prev->next = deadEnemy->next;  //if intermediate enemy

    //if last enemy
    if(deadEnemy->next != NULL)
        deadEnemy->next->prev = deadEnemy->prev;
    free(deadEnemy);
}

/*******************************End of Space Game ******************************************/

/******************************* Start of Connect4 Game ******************************************/
void drawGrid()
{
    int y;
    //last row is reserved for stack top
    for(int i = 0; i < rd; i++)
    {
        gotoxy((WIDTH/2)-(5*cd)/2, y = (HEIGHT/2)-(2*rd)/2+i*2);
		for(int j = 0; j < cd; j++)
        {
            int val = grid[i][j];
            if(val == 0)
                printf("%c    ", ' ');
            else
            {
                if(val == 'X')
                    setColor(RED);
                else if(val == 'O')
                    setColor(BLUE);
                printf("%c    ", val);
            }
		}
	}
    setColor(GREEN);
	table((WIDTH/2)-(5*cd)/2-2, (HEIGHT/2)-(2*rd)/2-1, cd, rd, 5, 2, 0);
	gotoxy((WIDTH/2)-(5*cd)/2, y+2);
	for(int i = 0; i < cd; i++)
        printf("%d    ", i);
}
void resetGrid()
{
    for(int i = 0; i < cd; i++)
        grid[rd][i] = rd-1;
    //first row is reserved for stack top
    for(int i = 0; i < rd; i++)
    {
		for(int j = 0; j < cd; j++)
        {
			grid[i][j] =  0;
		}
	}
}
void pushPlayer(int stackID)
{
    int top = grid[rd][stackID];
    //printf("top is : %d", top);
    switch (turn)
    {
    case 'X':
        grid[top][stackID] = 'X';
        turn = 'O';
        break;
    case 'O':
        grid[top][stackID] = 'O';
        turn = 'X';
        break;
    default:
        printf("\n\nError\n\n");
    }
    grid[rd][stackID] = top-1;
}
int checkWinner()
{
    //check row
		for(int i = 0; i < rd; i++) {
			for(int j = 0; j < cd-3; j++) {
				if(
						grid[i][j] != 0
					&&	grid[i][j] == grid[i][j+1]
					&&  grid[i][j+1] == grid[i][j+2]
					&&  grid[i][j+2] == grid[i][j+3]
				)
					return grid[i][j];
			}
		}

		//check column
		for(int i = 0; i < cd; i++) {
			for(int j = rd-1; j > 2; j--) {
				if(
						grid[j][i] != 0
					&&	grid[j][i] == grid[j-1][i]
					&&  grid[j-1][i] == grid[j-2][i]
					&&  grid[j-2][i] == grid[j-3][i]
				)
					return grid[j][i];
			}
		}
		//check non \ diagonal
		for(int i = 0; i < rd-3; i++) {
			for(int j = 0; j < cd-3; j++) {
				if(
						grid[i][j] != 0
					&&	grid[i][j] == grid[i+1][j+1]
					&&  grid[i+1][j+1] == grid[i+2][j+2]
					&&  grid[i+2][j+2] == grid[i+3][j+3]
				)
					return grid[i][j];
			}
		}

		//check diagonal /
		for(int i = rd-1; i > 2; i--) {
			for(int j = 0; j < cd-3; j++) {
				if(
						grid[i][j] != 0
					&&	grid[i][j] == grid[i-1][j+1]
					&&  grid[i-1][j+1] == grid[i-2][j+2]
					&&  grid[i-2][j+2] == grid[i-3][j+3]
				)
					return grid[i][j];
			}
		}
		return 0;
}
void startConnect4()
{
    //prepare environment for game
    clearCanvas();
    createWall();

    //start game
    isPlayingConnect4 = 1;
    turn = 'X';
    resetGrid();
    drawGrid();
    int input;
    do
    {
        int input; //initial input do continue the first loop
	    while(1)  //continuesly take input till user enters valid input
        {
            char turnLabel[100];
            sprintf(turnLabel, "Turn : %c  %c  Select a Stack: ", turn, 26);
            drawString(WIDTH/2-strlen(turnLabel)/2+wall.w, wall.h+1, turnLabel);
            input = getche() - 48;
            if(input+48 == QUIT)
                return;
            if(input >= 0 && input < cd)
                break;

            char errorMsg[100][100] =
            {
            "Select a valid stack!",
            " ",
            " "
            };
            char heading[100];
            sprintf(heading, " Dear '%c' ", turn);
            drawMsgBox(WIDTH/2-strlen(heading), HEIGHT-wall.h-5, errorMsg, 3, heading);
        }
        pushPlayer(input);
        drawGrid();
        int winner = checkWinner();
        if(winner)
        {
            isPlayingConnect4 = 0;
            setColor(winner == 'X'? RED: BLUE);
        }
    }
    while(isPlayingConnect4);
    //drawGrid();
    char winnerMsg[100][100] =
    {
        "G A M E   O V E R!",
        " "
    };
    char heading[100];
    sprintf(heading, " Winner : %c ", checkWinner());
    drawMsgBox(WIDTH/2-strlen(heading), HEIGHT-wall.h-5, winnerMsg, 2, heading);
}
/*******************************End of Connect4 game******************************************/

/*******************************Account Management******************************************/
void loadAccounts()
{
    FILE* file = fopen(playerDataFile, "r");
    if(file == NULL)
        return;
    char data[20];
    char username[passlen+1];
    char password[passlen+1];
    gotoxy(0,0);
    while(fscanf(file, playerDataFormat, username, password) != EOF)
    {
        addPlayer(username, password, 0);
        //0 means not need to save players to file while reading
    }
    fclose(file);
}

void reg()
{
    int color = mainColor;
    clearCanvas();
    char opts[100][100] = {
        "Max Length for name & password : 8",
        " ",
        "Username : _______________",
        "  ",
        "Password : _______________",
        "  ",
        "I agree to T&Cs (y/n) : _",
    };
    drawMenuBox(opts, 7, " Register Player ");

    const int x = coord.X;
    const int y = coord.Y;
    gotoxy(x+12, y-6);
    char username[passlen+1];
    myStrInput(username);

    if(*username == NULL)
        return; //if user wants to exit

    gotoxy(x+12, y-4);
    char password[passlen+1];
    myStrInput(password);

    gotoxy(x+24, y-2);
    char agreed;
    agreed = getche();
    if(agreed != 'y' && agreed != 'Y')
    {
        setColor(RED);
        char heading[] = " ERROR! ";
        char msg[][100] = {
            "You don't agree with T&Cs",
            " ",
            "Account not Created!"
        };
        gotoxy(x+24, y-2);
        printf("X");
        drawMsgBox(WIDTH/2-strlen(msg[0])/2-wall.w, HEIGHT - wall.h-1-5, msg, 3, heading);
        setColor(color);
        return;
    }
    gotoxy(x+24, y-2);
    printf("%c", 251);

    //check if there is user with same name
    struct PLAYER* temPlayer = playerList;
    while(temPlayer != NULL)
    {
        if(strcmp(temPlayer->username, username) == 0)
        {
            setColor(RED);
            char heading[] = " ERROR! ";
            char msg[][100] = {
            "Username Already Exists!",
            " ",
            "Try using different username"
            };
            drawMsgBox(WIDTH/2-strlen(msg[0])/2-wall.w, HEIGHT - wall.h-1-5, msg, 3, heading);
            setColor(color);
        return;
        }
        temPlayer = temPlayer->next;
    }
    addPlayer(username, password, 1);

    char heading[] = " Success ";
        char msg[][100] = {
            "Player Registered",
            " "
        };
    drawMsgBox(WIDTH/2-strlen(msg[0])/2-wall.w, HEIGHT - wall.h-1-5, msg, 2, heading);
}

int login()
{
    int color = mainColor;
    clearCanvas();
    char opts[100][100] = {
        "Username : _______________",
        "  ",
        "Password : _______________"
    };
    drawMenuBox(opts, 3, " Login ");

    const int x = coord.X;
    const int y = coord.Y;
    gotoxy(x+12, y-4);
    char username[passlen+1];
    myStrInput(username);

    if(*username == NULL)
        return 0; //if user wants to exit

    gotoxy(x+12, y-2);
    char password[passlen+1];
    myStrInput(password);

    //find the similar username
    struct PLAYER* temPlayer = playerList;
    while(temPlayer != NULL)
    {
        if(strcmp(temPlayer->username, username) == 0)
            break;
        temPlayer = temPlayer->next;
    }
    if(temPlayer == NULL){
        setColor(RED);
        char heading[] = " Error! ";
        char msg[][100] = {
            "Player Not Found!",
            " ",
            "Try Again!"
        };
        drawMsgBox(WIDTH/2-strlen(msg[0])/2-wall.w, wall.h+1, msg, 3, heading);
    }
    else if(strcmp(temPlayer->password, password) == 0)
    {
        // setColor(GREEN);
        // char heading[] = " Success ";
        // char msg[][100] = {
        //     "Login Successful!",
        //     " ",
        //     "Enjoy your play"
        // };
        // drawMsgBox(WIDTH/2-strlen(msg[0])/2 -wall.w, wall.h+1, msg, 3, heading);
        player = temPlayer;
        return 1;
    }
    else
    {
        setColor(RED);
        char heading[] = " Error! ";
        char msg[][100] = {
            "Wrong Password",
            " ",
            "Try Again!",
        };
        drawMsgBox(WIDTH/2-strlen(msg[0])/2 -wall.w, wall.h+1, msg, 3, heading);
    }
    setColor(color);
    return 0;
}

void addPlayer(char username[], char password[], int saveToFile)
{
    struct PLAYER* newPlayer = (struct PLAYER*) malloc(sizeof(struct PLAYER));
    strcpy(newPlayer->username, username);
    strcpy(newPlayer->password, password);
    newPlayer->next = newPlayer->prev = NULL;

    if(playerList == NULL)
    {
        playerList = newPlayer;
    }
    else
    {
        //finding the last player in the list
        struct PLAYER* temPlayer = playerList;
        while(temPlayer->next != NULL)
            temPlayer = temPlayer->next;

        temPlayer->next = newPlayer;
        newPlayer->prev = temPlayer;
    }

    if(saveToFile)
    {
        FILE* file = fopen(playerDataFile, "a");
        fprintf(file, playerDataFormat, newPlayer->username, newPlayer->password);
        fclose(file);
    }
}

void delAccount()
{
    if(login())
    {
        if(player->prev == NULL)
        {
            playerList = player->next;
            free(player);
        }
        else
        {
            player->prev->next = player->next;
            if(player->next != NULL)
                player->next->prev = player->prev;
            free(player);
            player = NULL;  //deleted from memory
        }

        //lets update the file
        FILE* file = fopen(playerDataFile, "w");
        struct PLAYER* temPlayer = playerList;
        while (temPlayer != NULL)
        {
            fprintf(file, playerDataFormat, temPlayer->username, temPlayer->password);
            temPlayer = temPlayer->next;
        }
        fclose(file);
        char heading[] = " Success ";
        char msg[][100] = {
            "Account Deleted!",
            " ",
            "Sorry to see you go :("
        };
        drawMsgBox(WIDTH/2-strlen(msg[0])/2 -wall.w, wall.h+1, msg, 3, heading);
    }
}
