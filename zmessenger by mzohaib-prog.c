/*
*This program is created by Muhammad Zohaib, on jan 15, 2020
*This program works as messenger. You can create account in this program and secure it with a password.The passwords of all users can be easily accessed by an option in the main menu of program. You can send and receive messages from the available users in the program. All the messages are saved in the memory with file handling and you can see it inbox and outbox.
*This program is created as project for educational purposes.
*This program will work best in mingw compiler in codeblocks in windows.
*/
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define maxUsers 10
#define maxMessages 100
#define mLen 100
#define filename "userData.bin"

typedef struct{
    int id;
    char password[8];
    char name[20];
    char inbox[maxMessages][mLen];
    char outbox[maxMessages][mLen];
}USER;

USER* users;

COORD coord;
void gotoxy(int x, int y){
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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
}

char* toString(int num)
{
    char* snum = malloc(20);
    sprintf(snum,"%d",num);
    return snum;
}

void clearCanvas()
{
    system("CLS");
}

void loginMenu()
{
    printf("1.Inbox\n");
    printf("2.Outbox\n");
    printf("3.Write Message\n");
    printf("4.Delete Inbox\n");
    printf("5.Delete Outbox \n");
    printf("6.Log Out\n");
    printf("Select a number from above options: ");
}

void delInbox(USER* userin)
{
    int i;
    for(i = 0; i < maxMessages; i++)
        strcpy(userin->inbox[i], "");
    printf("\nInbox cleared!\n\n");
}

void delOutbox(USER* userin)
{
    int i;
    for(i = 0; i < maxMessages; i++)
        strcpy(userin->outbox[i], "");
    printf("\nOutbox cleared!\n\n");
}

void inbox(USER* userin)
{
    int j;
    for(j = maxMessages-1; j >= 0; j--)
    {
        printf("%s", userin->inbox[j]);
        printf("\n\n");
    }
}

void outbox(USER* userin)
{
    int j;
    for(j = maxMessages-1; j >= 0; j--)
    {
        printf("%s", userin->outbox[j]);
        printf("\n\n");
    }
}
void writeMessage(USER* userin)
{
    int i,j, rID;
    char message[mLen];
    char message2[mLen];
    printf("Enter Receiver's ID: ");
    scanf("%d", &rID);

    for(i = 0; i < maxUsers; i++)
    {
        if(rID == users[i].id)
            break;
    }
    printf("Receiver's name: %s\n", users[i].name);
    printf("Type your Message: ");
    fflush(stdin);
    gets(message);

    //saving anaother message for resetting
    strcpy(message2, message);

    //saving for receiver
    strcat(message, "\nSent by: ");
    strcat(message, userin->name);
    strcat(message, "  ");
    strcat(message, toString(userin->id));
    strcat(message, "\nDate: ");
    strcat(message, __DATE__);
    strcat(message, "  ");
    strcat(message, __TIME__);

    for(j = 0; j < maxMessages; j++)
    {
        if(!strcmp(users[i].inbox[j], ""))
            break;
    }

    strcpy(users[i].inbox[j], message);

    //resetting message
    strcpy(message, message2);

    for(j = 0; j < maxMessages; j++)
    {
        if(!strcmp(userin->outbox[j], ""))
            break;
    }
    //saving for sender outbox
    strcat(message, "\nSent To: ");
    strcat(message, users[i].name);
    strcat(message, "  ");
    strcat(message, toString(users[i].id));
    strcat(message, "\nDate: ");
    strcat(message, __DATE__);
    strcat(message, "  ");
    strcat(message, __TIME__);

    strcpy(userin->outbox[j], message);
    printf("\nMessage Sent Successfully. . .\n\n");
}

void manageLogin(USER* userin)
{
    int option;
    loginMenu();
    scanf("%d", &option);
    while(option != 6)
    {
        switch(option)
        {
        case 1:
            inbox(userin);
            break;
        case 2:
            outbox(userin);
            break;
        case 3:
            writeMessage(userin);
            break;
        case 4:
            delInbox(userin);
            break;
        case 5:
            delOutbox(userin);
        default:
        printf("\nPlease Select a valid option!\n\n");
        }

        system("pause");
        clearCanvas();
        loginMenu();
        fflush(stdin);
        scanf("%d", &option);
    }

    printf("\nLogged Out Successfully!\n\n");
}
void mainMenu()
{
    printf("1.Login\n");
    printf("2.Sign Up\n");
    printf("3.Display Users\n");
    printf("4.Delete User\n");
    printf("5.Exit\n");
    printf("Select a number from above options: ");
}

void login()
{
    int i, id;
    char password[8];
    printf("Enter ID to Login: ");
    scanf("%d", &id);
    for(i = 0; i < maxUsers; i++)
    {
        if(users[i].id == id)
            break;
    }
    if(i == maxUsers)
    {
        printf("\nNo User Found. . .!\n\n");
        return;
    }

    printf("Your account alias: %s\n", users[i].name);

    printf("Enter your password: ", password);
    fflush(stdin);
    gets(password);
    if(!strcasecmp(password, users[i].password))
    {
        clearCanvas();
        printf("Login Successful\n\n");
        //autoLoad(10,5);
        manageLogin(&users[i]);
    }
    else
    {
        printf("Wrong password!\n\n");
    }
}

void signUp()
{
    int i;
    for(i = 0; i < maxUsers; i++)
    {
        if(users[i].id == 0)
            break;
    }
    if(i == maxUsers)
    {
        printf("\nNo space. . .!\n");
        return;
    }

    printf("Enter your name: ");
    fflush(stdin);
    gets(users[i].name);
    printf("Enter your ID: ");
    scanf("%d", &users[i].id);
    printf("Enter your password: ");
    fflush(stdin);
    gets(users[i].password);

    printf("\nAccount Created Successfully. . .\n");
}

void save(FILE *file)
{
    file = fopen(filename, "wb");
    fwrite((USER*)users, sizeof(USER), maxUsers, file);
    fclose(file);
}

void displayUsers()
{
    int i;
    int count = 0;
    printf("\n\n%20s %20s %20s", "ID", "NAME", "PASSWORD");
    for(i = 0; i < maxUsers; i++)
        if(users[i].id != 0)
        {
            printf("\n\n%20d %20s %20s", users[i].id, users[i].name, users[i].password);
            count++;
        }
        table(0,7,3,count+1,21,2,0);
        gotoxy(0, count*2 +10);
}
void delUser()
{
    int i, id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    for(i = 0; i < maxUsers; i++)
    {
        if(users[i].id == id)
            break;
    }
    if(i == maxUsers)
    {
        printf("\nNo User Found. . .!\n\n");
        return;
    }

    users[i].id = 0;
    printf("\nUser deleted Successfully. . .\n\n");
}
int main()
{
    users = calloc(sizeof(USER), maxUsers);
    FILE *file = fopen(filename, "rb");
    if(file == NULL)
    {
        file = fopen(filename, "wb");
    }
    fread((USER*)users, sizeof(USER), maxUsers, file);
    fclose(file);

    int option;

    mainMenu();
    scanf("%d", &option);
    while(option != 5)
    {
        switch(option)
        {
        case 1:
            login();
            break;
        case 2:
            signUp();
            break;
        case 3:
            displayUsers();
            break;
        case 4:
            delUser();
            break;
        default:
        printf("\nPlease Select a valid option!\n\n");
        }

        system("pause");
        clearCanvas();
        mainMenu();
        fflush(stdin);
        scanf("%d", &option);
    }
    save(file);
    printf("\nProgram Exited. . .\n\n");
    return 0;
}
