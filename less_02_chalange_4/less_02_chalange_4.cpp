// less_02_chalange_4.cpp 
/*
Created by Antonenko Andry

IDE: Visual Studio 2019

*/

#include <iostream>
#include <windows.h>
#include <sysinfoapi.h>
#include <conio.h>
#include <list>

#define MIN_TIME_STEP 1
#define MAX_TIME_STEP 1000

#define ARROW_UP ((uint16_t)224) | ((uint16_t)72 << 8)
#define ARROW_DOWN ((uint16_t)224) | ((uint16_t)80 << 8)
#define ARROW_LEFT ((uint16_t)224) | ((uint16_t)75 << 8)
#define ARROW_RIGHT ((uint16_t)224) | ((uint16_t)77 << 8)
#define ESC ((uint16_t)27)
#define PLUS ((uint16_t)43)
#define MINUS ((uint16_t)45)

struct Key
{
    uint8_t first_code;
    uint8_t second_code;
    uint16_t code;
};

struct SnakeSegment
{
    char ch;
    uint8_t x;
    uint8_t y;
};

enum eDirectionMovement
{
    DM_UP,
    DM_DOWN,
    DM_LEFT,
    DM_RIGHT
};

struct Snake
{
    std::list<SnakeSegment> segments;
};

#define FILD_SIZE 20

struct GameSnake
{
    Snake snake;
    eDirectionMovement direction;
    char fild[FILD_SIZE][FILD_SIZE] = { ' ' };
};

enum class eStateRunning
{
    SR_GAMERUNNING,
    SR_GAMEOVER,
};

void draw_fild(char* pFild);
eStateRunning movementSnake(GameSnake* g);
void mappingSnakeOnFild(GameSnake* g);
void clean_fild(char* pFild);

int main()
{
    BOOLEAN bRunGame = true;
    Key key = {0,0};
    int iTimeStep = 1000;

    GameSnake game;
    SnakeSegment seg = {'o',0,0};
    bool bAddSeg = false;

    game.snake.segments.push_back(seg);
    game.direction = DM_RIGHT;
    
    ULONGLONG time = GetTickCount64();

    while (bRunGame)
    {
        if (_kbhit())
        {
            key.first_code = _getch();
            if (key.first_code == 224) key.second_code = _getch();
            else key.second_code = 0;
            key.code = ((uint16_t)key.first_code) | ((uint16_t)key.second_code << 8);
        }
        switch (key.code)
        {
        case ARROW_UP:
            game.direction = DM_UP;
            break;
        case ARROW_DOWN:
            game.direction = DM_DOWN;
            break;
        case ARROW_RIGHT:
            game.direction = DM_RIGHT;
            break;
        case ARROW_LEFT:
            game.direction = DM_LEFT;
            break;
        case ESC:
            bRunGame = false;
            break;
        case PLUS:
            if (iTimeStep > MIN_TIME_STEP) iTimeStep--;
            break;
        case MINUS:
            if (iTimeStep < MAX_TIME_STEP) iTimeStep++;
            break;
        default:
            if (key.code != 0)
            {
                seg = { (char)key.code, 0, 0 };
                bAddSeg = true;
            }
            break;
        }
        
        if ((GetTickCount64() - time) >= iTimeStep)
        {
            time = GetTickCount64();
            system("cls");

            if (bAddSeg == true)
            {
                game.snake.segments.push_back(seg);
                bAddSeg = false;
            }
 
            eStateRunning sr = movementSnake(&game);
            mappingSnakeOnFild(&game);
            draw_fild(&game.fild[0][0]);

            switch (sr)
            {
            case eStateRunning::SR_GAMERUNNING:
                std::cout << "Time step: " << iTimeStep;
                
                break;
            case eStateRunning::SR_GAMEOVER:
                std::cout << "Game over!";
                bRunGame = false;
                break;
            default:
                break;
            }
        }
        key.code = 0;
    }

    return 0;
}

void mappingSnakeOnFild(GameSnake* g)
{
    clean_fild(&g->fild[0][0]);
    for (auto iter = g->snake.segments.begin(); iter != g->snake.segments.end(); iter++)
    {
        g->fild[iter->y][iter->x] = iter->ch;
    }
}

eStateRunning movementSnake(GameSnake *g)
{
    eStateRunning result = eStateRunning::SR_GAMERUNNING;
    bool enable_move = false;
    
    SnakeSegment seg,seg_next;
    int lengthSnake = g->snake.segments.size();
    
    auto iter = g->snake.segments.begin();
    seg = *iter;
    
    switch (g->direction)
    {
    case DM_UP:
        seg.ch = iter->ch;
        seg.x = iter->x;
        if (iter->y > 0)
        {
            iter->y--;
            enable_move = true;
        }
        else if (iter->y == 0)
        {
            result = eStateRunning::SR_GAMEOVER;
        }
        break;
    case DM_DOWN:
        seg.ch = iter->ch;
        seg.x = iter->x;
        if (iter->y < FILD_SIZE - 1)
        {
            iter->y++;
            enable_move = true;
        }
        else if (iter->y == FILD_SIZE - 1)
        {
            result = eStateRunning::SR_GAMEOVER;
        }
        break;
    case DM_RIGHT:
        seg.ch = iter->ch;
        seg.y = iter->y;
        if (iter->x < FILD_SIZE - 1)
        {
            iter->x++;
            enable_move = true;
        }
        else if (iter->x == FILD_SIZE - 1)
        {
            result = eStateRunning::SR_GAMEOVER;
        }
        break;
    case DM_LEFT:
        seg.ch = iter->ch;
        seg.y = iter->y;
        if (iter->x > 0)
        {
            iter->x--;
            enable_move = true;
        }
        else if (iter->x == 0)
        {
            result = eStateRunning::SR_GAMEOVER;
        }
        break;
    }
    
    if (enable_move)
    {
        while (++iter != g->snake.segments.end())
        {
            seg_next = *iter;
            iter->x = seg.x;
            iter->y = seg.y;
            seg = seg_next;
        }
    }

    return result;
}

void draw_fild(char *pFild)
{
    for (int i = 0; i < FILD_SIZE+2; i++)
    {
        std::cout << '|';
        for (int j = 0; j < FILD_SIZE; j++)
        {
            if (i == 0 || i == (FILD_SIZE + 1))
            {
                std::cout << '-';
            }
            else
            {
                std::cout << *(pFild + (i-1) * FILD_SIZE + j);
            }
        }
        std::cout << '|' << std::endl;
    }
}

void clean_fild(char* pFild)
{
    for (int i = 0; i < FILD_SIZE; i++)
    {
        for (int j = 0; j < FILD_SIZE; j++)
        {
            *(pFild + i * FILD_SIZE + j) = ' ';
        }
    }
}



