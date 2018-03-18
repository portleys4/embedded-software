#include "snake_game.h"
#include "system.h"
#include "random_int.h"
#include "game.h"
#include "timing.h"
#include "terminal.h"
#include "task.h"
#include "strings.h"

static void Callback(int argc, char * argv[]);
static void Receiver(uint8_t c);

static void Play(void);
static void Help(void);

static void DrawSnake(void);
static void AddLink(void);
static void MoveSnake(void);
static void GameTick(void);
static void GameOver(void);
static void SpawnFood(void);
static void DrawFood(void);

uint8_t score;
uint8_t game_id;
enum snake_movements curr_direction;
snake_link_t snake[MAX_SNAKE_LENGTH];
food_t food; 


void SnakeGame_Init(void){

    game_id = Game_Register("SNAKE", "Classic Snake Game", Play, Help);
    Game_RegisterCallback(game_id, Callback);
}

void Help(void){
    Game_Printf("WASD to change the direction of the snake, collect the food without hitting yourself or a wall");
}

void Callback(int argc, char * argv[]){
    if(argc == 0) Game_Log(game_id, "too few args");
    if(strcasecmp(argv[0], "reset") == 0){
        score = 0;
        Game_Log(game_id, "Scores reset");
    }else Game_Log(game_id, "command not supported");
}

void Play(void){

    Game_ClearScreen();
    Game_DrawRect(0, 0, SNAKE_MAP_WIDTH, SNAKE_MAP_HEIGHT);
    Game_RegisterPlayer1Receiver(Receiver);
    Game_HideCursor();

    uint8_t i;
    for(i = 1; i<MAX_SNAKE_LENGTH; i++){
        snake[i].xcord = 0;
        snake[i].ycord = 0;
        snake[i].in_play = 0;
    }
    score = 0;

    snake[0].xcord = SNAKE_MAP_WIDTH / 2;
    snake[0].ycord = SNAKE_MAP_HEIGHT / 2;
    snake[0].in_play = 1;
    curr_direction = UP;
    Game_CharXY(SNAKE_CHAR, snake[0].xcord, snake[0].ycord);

    SpawnFood();
    Task_Schedule((task_t)GameTick, 0, 5000, 500);
}


void DrawSnake(void){
    uint8_t i;
    for(i = 0; i < MAX_SNAKE_LENGTH; i++){
        if(snake[i].in_play){
            Game_CharXY(SNAKE_CHAR, snake[i].xcord, snake[i].ycord);
        }else{
            break;
        }
    } 
}


//Add a link at the head of the snake
void AddLink(void){
    uint8_t i;
    for(i = 1; i < MAX_SNAKE_LENGTH; i++){
        if(!snake[i].in_play){

            switch(curr_direction){
                case UP:
                    snake[i].xcord = snake[i-1].xcord;
                    snake[i].ycord = snake[i-1].ycord - 1;
                    break;
                case LEFT:
                    snake[i].xcord = snake[i-1].xcord - 1;
                    snake[i].ycord = snake[i-1].ycord;
                    break;
                case RIGHT:
                    snake[i].xcord = snake[i-1].xcord + 1;
                    snake[i].ycord = snake[i-1].ycord;
                    break;
                case DOWN:
                    snake[i].xcord = snake[i-1].xcord;
                    snake[i].ycord = snake[i-1].ycord + 1;
                    break;
            }
            snake[i].in_play = 1;
            return;
        }
    }

    GameOver(); //You win!
}


void GameTick(void){

    uint8_t i = 0;
    while(snake[i+1].in_play){
        i++;
    }

    if(snake[i].xcord <= 0 || snake[i].xcord >= SNAKE_MAP_WIDTH){
        GameOver();
    }

    if(snake[i].ycord <= 0 || snake[i].ycord >= SNAKE_MAP_HEIGHT){
        GameOver();
    }

    if(snake[i].xcord == food.xcord && snake[i].ycord == food.ycord){
        score++;
        SpawnFood();
        AddLink();
    }else{
        MoveSnake();
    }

    Draw_Snake();
    DrawFood();

}

void MoveSnake(void){

    Game_CharXY(0x20, snake[0].xcord, snake[0].ycord);
    AddLink();

    uint8_t i;
    for(i = 1; i < MAX_SNAKE_LENGTH; i++){
        if(snake[i].in_play){
            snake[i-1] = snake[i];
        }else{
            break;
        }
    }

    snake[i-1].in_play = 0;
    snake[i-1].xcord = 0;
    snake[i-1].ycord = 0;

}


void DrawFood(void){
    Game_CharXY(FOOD_CHAR, food.xcord, food.ycord);
}



void SpawnFood(void){

    //we just picked up food, so we don't need to clear current spot

    food.xcord = random_int(1, SNAKE_MAP_WIDTH-1);
    food.ycord = random_int(1, SNAKE_MAP_HEIGHT-1);
    DrawFood();


}

void Receiver(uint8_t c){
    switch(c){
        case 'w':
        case 'W':
            curr_direction = UP;
            break;
        case 'a':
        case 'A':
            curr_direction = LEFT;
            break;
        case 's':
        case 'S':
            curr_direction = DOWN;
            break;
        case 'd':
        case 'D':
            curr_direction = RIGHT;
            break;
        default:
            break;
    }
}

void GameOver(){
    //cleanup
    Task_Remove(GameTick, 0);
    Game_UnregisterPlayer1Receiver(Receiver);
    Game_CharXY('\r', 0, SNAKE_MAP_HEIGHT + 1);
    Game_Printf("Game Over! Final score: %d", score);
    Game_ShowCursor();
    Game_GameOver();

}
