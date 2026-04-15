#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define WINDOWS

#ifdef WINDOWS
    #include<Windows.h>
#else
    #include<unistd.h>
#endif


#define XRES 80
#define YRES 50

#define PLACE_CHANCE 75

/*Rain Per Tick*/
#define RPT 5

char screen[YRES][XRES];

enum FieldStates : char
{
    NoRain = 0,
    Rain = 1,
    Floor = 2
};

enum FieldStates (*field)[YRES][XRES];
enum FieldStates (*nextField)[YRES][XRES];

enum FieldStates fieldBuffer1[YRES][XRES];
enum FieldStates fieldBuffer2[YRES][XRES];

void init(void)
{
    for(int y = 0; y < YRES; y++)
    {
        for(int x = 0; x < XRES; x++)
        {
            screen[y][x] = 'a';
        }
    }
    
    for(int x = 0; x < XRES; x++)
    {
        (*field)[YRES - 1][x] = Floor;
    }
}

void clearConsoleScreen()
{
    printf("\e[1;1H\e[2J");
}

void writeScreen()
{
    for(int y = 0; y < YRES; y++)
    {
        for(int x = 0; x < XRES; x++)
        {
            putchar(screen[y][x]);
        }
        
        putchar('\n');
    }
}

void sleepMS(int msecs)
{
    #ifdef WINDOWS
    Sleep(msecs);
    #else
    usleep(msecs * 1000);
    #endif
}

void setScreenPos(int x, int y, enum FieldStates state)
{
    switch(state)
    {
        case NoRain:
            screen[y][x] = '.';
            break;
        case Rain:
            screen[y][x] = '@';
            break;
        case Floor:
            screen[y][x] = '#';
            break;
    }
}

void updateField()
{
    for(int y = 0; y < YRES; y++)
    {
        for(int x = 0; x < XRES; x++)
        {
            if(y == YRES - 1)
            {
                continue;
            }
            
            else if((*field)[y][x] == Rain)
            {
                (*nextField)[y][x] = NoRain;
                (*nextField)[y + 1][x] = Rain;
            }
        }
    }
}

void updateScreen()
{
    for(int y = 0; y < YRES; y++)
    {
        for(int x = 0; x < XRES; x++)
        {
            setScreenPos(x, y, (*field)[y][x]);
        }
    }
}

void placeRain(void)
{
    int index = rand() % XRES;
    (*field)[0][index] = Rain;
}

void main(void)
{    
    srand((unsigned)time(NULL));
    
    field = &fieldBuffer1;
    nextField = &fieldBuffer2;
    
    init();    
    clearConsoleScreen();
    
    while(1)
    {        
        if(rand() % 100 < PLACE_CHANCE)
        {        
            for(int i = 0; i < RPT; i++)
            {
                placeRain();
            }
        }
        
        updateField();  
        updateScreen();
        writeScreen();      
        sleepMS(25);
        clearConsoleScreen();
        
        enum FieldStates (*temp)[YRES][XRES];
        
        temp = field;
        field = nextField;
        nextField = temp;
 
        for(int y = 0; y < YRES; y++)
        {
            for(int x = 0; x < XRES; x++)
            {
               (*nextField)[y][x] = NoRain;
            }
            
            for(int x = 0; x < XRES; x++)
            {
                (*nextField)[YRES - 1][x] = Floor;
            }
        }               
    }
}