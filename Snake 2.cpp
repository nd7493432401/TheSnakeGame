/*
Created By  : Niladri Dutta
Date        : 10-1-2014
Language    : C++
*/


#include<iostream>
#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<ctime>
#include<windows.h>
#include<fstream>

#define U VK_UP
#define D VK_DOWN
#define R VK_RIGHT
#define L VK_LEFT

using namespace std;

const int sz=25;            //Maximum Size of Board

const char symbol='O';
const char u=U , d=D , l=L , r=R;
int level=1;

char SnakeDir=u;

void gotoxy(int x,int y);
int TargetPositionCheck(void);                           //Check validation of random Target Position
void NewTarget(void);                                    //Randomize Target Position
void NewBoard(void);                     //Initialize a New Board
void NewSnake(void);
void SetSnake(void);
void SetTarget(void);
void SetBoard(void);
void NewGame(void);                      //Initialize a New Game
void DisplayBoard(void);
void UpdateSnake(void);
void NewPosition(void);         //To check for change in direction
int EndGameCheck(void);                                   //Check for End Game


struct block                //Definition of each Block
{
    char item;
    int status;
}board[sz][sz];             //Declaration of Board

struct snake_segment        //Definition for snake_segment
{
    int x,y;                        //location of segment
    snake_segment *prev,*next;      //Pointer to next and previous objects
    char sym;
}*beg,*rear;

struct TARGET                         //Target structure
{
    int x,y;
    char sym;
}target;

COORD coord={0,0};

void gotoxy(int x,int y)
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

int TargetPositionCheck(void)                           //Check validation of random Target Position
{
    if(board[target.x][target.y].status!=0)             //*****HAS to be checked for the MASTER of the GAME
        return 1;
    else
        return 0;
}

void NewTarget(void)                                    //Randomize Target Position
{
    time_t t;
    srand((unsigned)time(&t));
    target.sym='X';
    do
    {
        target.x = rand()%sz;
        target.y = rand()%sz;
    }while(TargetPositionCheck());
    ++level;
}


snake_segment *CreateSegment(void)      //To create and return the pointer to a new segment of snake
{
    snake_segment *temp;
    temp = new snake_segment;
    temp->prev = temp->next = NULL;
    temp->sym = symbol;
    return temp;                        //Return Loaction of new Segment
}

void NewBoard(void)                     //Initialize a New Board
{
    for(int i=0 ; i<sz ; ++i)
    {
        for(int j=0 ; j<sz ; ++j)
        {
            board[i][j].item = ' ';
            board[i][j].status = 0;
        }
    }
}

void NewSnake(void)
{
    snake_segment *ptr;                 //Temporary Pointer
    beg = CreateSegment();            //Initilaise pointer
    beg->x = beg->y = sz/2;             //Initial Position of beg
    beg->sym = '*';

    beg->next = CreateSegment();        //Create next loction ie Rear

    ptr = beg->next;                //For initiallization of rear segment

    ptr->prev = beg;
    ptr->x = sz/2;
    ptr->y = sz/2-1;

    rear = ptr;
}

void SetSnake(void)
{
    snake_segment *ptr = beg;
    while(ptr != NULL)
    {
        board[ptr->x][ptr->y].item = ptr->sym;
        if(board[ptr->x][ptr->y].status == 0)
        {
            board[ptr->x][ptr->y].status = 1;
        }
                                                                   //For debugging
        else if(board[ptr->x][ptr->y].status == 2)
        {
            board[ptr->x][ptr->y].status = 3;

            NewTarget();                        //Create An New Target
        }
        ptr = ptr->next;
    }
}

void SetTarget(void)
{
    if(board[target.x][target.y].status != 3)
    {
        board[target.x][target.y].item = target.sym;
        board[target.x][target.y].status = 2;
    }
}

void SetBoard(void)
{
    SetSnake();
    SetTarget();
}

void NewGame(void)                      //Initialize a New Game
{
    NewBoard();
    NewSnake();
    SetSnake();
    NewTarget();
    SetBoard();
    level=1;
}

void DisplayBoard(void)
{
    SetBoard();

    cout<<endl<<"+";
    for(int n=0 ; n<sz ; ++n)
        cout<<"-";
    cout<<"+";

    for(int i=0 ; i<sz ; ++i)
    {
        cout<<"\n|";
        for(int j=0 ; j<sz ; ++j)
        {
            cout<<board[i][j].item;
        }
        cout<<"|";
    }

    cout<<endl<<"+";
    for(int n=0 ; n<sz ; ++n)
        cout<<"-";
    cout<<"+";
}

void UpdateSnake(void)
{
    snake_segment *ptr=NULL;        //Temperary Pointer

    if(board[rear->x][rear->y].status  == 3)
    {
        rear->next = CreateSegment();       //Create New Back Componet

        rear->next->prev = rear;

        rear = rear->next;

        rear->sym = symbol;

        rear->x = rear->prev->x;
        rear->y = rear->prev->y;
    }

    board[rear->x][rear->y].item    = ' ';      //Set Last position to BLANK
    board[rear->x][rear->y].status  = 0;        //Set status to ZERO

    ptr=rear;

    while(ptr->prev != NULL)
    {
        if(ptr->prev != NULL)
        {
            ptr->x = ptr->prev->x;              //Copying Position Values
            ptr->y = ptr->prev->y;
        }
        ptr = ptr->prev;
    }
}


void NewPosition(void)          //To check for change in direction
{
    UpdateSnake();
    if(GetAsyncKeyState(U)||GetAsyncKeyState(D)||GetAsyncKeyState(L)||GetAsyncKeyState(R))
    {
    if(GetAsyncKeyState(U))
           {
               if(SnakeDir!=d)
                {
                    if(beg->x-1==0)
                    {
                        beg->x=sz-1;
                    }
                    else
                    {
                        beg->x--;
                    }
                    SnakeDir=u;
                }
           }
        if(GetAsyncKeyState(D))
            {
                if(SnakeDir!=u)
                {
                    if(beg->x+1==sz)
                    {
                        beg->x=0;
                    }
                    else
                    {
                        beg->x++;
                    }
                    SnakeDir=d;
                }
            }
        if(GetAsyncKeyState(L))
            {
                if(SnakeDir!=r)
                {
                    if(beg->y-1==0)
                    {
                        beg->y=sz-1;
                    }
                    else
                    {
                        beg->y--;
                    }
                    SnakeDir=l;
                }
            }
        if(GetAsyncKeyState(R))
            {
                if(SnakeDir!=l)
                {
                    if(beg->y+1==sz-1)
                    {
                        beg->y=0;
                    }
                    else
                    {
                        beg->y++;
                    }
                    SnakeDir=r;
                }
            }
    }
    else
    {
        switch (SnakeDir)
        {
        case u:
            if(SnakeDir!=d)
                {
                    if(beg->x-1==0)
                    {
                        beg->x=sz-1;
                    }
                    else
                    {
                        beg->x--;
                    }
                    SnakeDir=u;
                }
            break;
        case d:
            if(SnakeDir!=u)
                {
                    if(beg->x+1==sz)
                    {
                        beg->x=0;
                    }
                    else
                    {
                        beg->x++;
                    }
                    SnakeDir=d;
                }
            break;
        case l:
            if(SnakeDir!=r)
                {
                    if(beg->y-1==0)
                    {
                        beg->y=sz-1;
                    }
                    else
                    {
                        beg->y--;
                    }
                    SnakeDir=l;
                }
            break;
        case r:
            if(beg->y+1==sz-1)
                    {
                        beg->y=0;
                    }
                    else
                    {
                        beg->y++;
                    }
                    SnakeDir=r;
            break;
        }
    }
}

int EndGameCheck(void)                                   //Check for End Game
{
    if(board[beg->x][beg->y].item=='O')
        return 0;
    else
        return 1;
}

void SaveScore()
{
    char name[40],filename[50]="HIGHSCORE.TXT",str[60];
    fstream file(filename,ios::in|ios::out);
    file.seekg(0,ios::beg);
    while(!file.eof())
    {
        file.getline(str,'\n');
        cout<<"\n"<<str;
    }
    cout<<"\nEnter Name to Save Highscore :";
    cin.getline(name,40);
    file.seekp(0,ios::end);
    file<<name;
    cout<<"\t";
    file<<level<<endl;
}

int main()
{
    char choice;
    do
    {
        NewGame();                                  //Start a New Game
        do
        {
            gotoxy(0,0);
            cout<<"\tSNAKE GAME\n";
            cout<<"\nLEVEL : "<<level<<endl;
            DisplayBoard();
            NewPosition();
            Sleep(100-level);
        }while(EndGameCheck());
        SaveScore();
        cout<<"\nDo you want to Continue?(Y/N)";
        cin>>choice;
    }while(tolower(choice)=='y');

    return 0;
}
