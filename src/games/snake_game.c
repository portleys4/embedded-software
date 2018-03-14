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

static void Snake_Init(void);
static uint8_t CheckCollision(void);
static void GameTick(void);
static void GameOver(void);
static void SpawnFood(void);

uint8_t score;
uint8_t game_id;
enum snake_movements curr_direction;
snake_link_t snake;
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
    Game_DrawRect(0, 0, MAP_WIDTH, MAP_HEIGHT);
    Game_RegisterPlayer1Receiver(Receiver);
    Game_HideCursor();
    Snake_Init();
    //TODO: Add task to spawn food
    Task_Schedule((task_t)GameTick, 0, 0, 500);

}

void Snake_Init(){
    snake.xcord = MAP_WIDTH / 2;
    snake.ycord = MAP_HEIGHT / 2;
    snake.next_link = 0;
    curr_direction = UP;
    Game_CharXY('@', snake.xcord, snake.ycord);
}

void GameTick(void){
    //Move snake one tile
    //check collisions with the wall
    //check collsision with food


    snake_link_t new_link;
    switch(curr_direction){
        case UP:
            new_link.xcord = snake.xcord;
            new_link.ycord = snake.ycord - 1;
            break;
        case LEFT:
            new_link.xcord = snake.xcord - 1;
            new_link.ycord = snake.ycord; 
            break;
        case DOWN:
            new_link.xcord = snake.xcord;
            new_link.ycord = snake.ycord + 1;
            break;
        case RIGHT:
            new_link.xcord = snake.xcord + 1;
            new_link.ycord = snake.ycord;
            break;
    }

    new_link.next_link = &snake;
    snake = new_link;
    Game_CharXY('@', snake.xcord, snake.ycord);
    uint8_t is_food = CheckCollision();
    if(!is_food){
        //Remove last link
        snake_link_t* prev = &snake;
        snake_link_t* next = snake.next_link; 
        while(next->next_link){
            prev = prev->next_link;
            next = next->next_link; 
        }
        prev->next_link = 0;
        Game_CharXY(' ', next->xcord, next->ycord); 
    }

}



uint8_t CheckCollision(void){
    if(snake.xcord <= 0 || snake.xcord >= MAP_WIDTH){
        GameOver();
    }

    if(snake.ycord <= 0 || snake.ycord >= MAP_HEIGHT){
        GameOver();
    }

    if(snake.xcord == food.xcord && snake.ycord == food.ycord){
        score++;
        SpawnFood();
        return 1; //Return 1 when food found
    }

    return 0; //return when no food
}



void SpawnFood(void){

    //we just picked up food, so we don't need to clear current spot

    food.xcord = random_int(1, MAP_WIDTH-1);
    food.ycord = random_int(1, MAP_HEIGHT-1);
    Game_CharXY(food.food_char, food.xcord, food.ycord);
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
        case 'd':
        case 'D':
            curr_direction = RIGHT;
        default:
            break;
    }
}

void GameOver(){


    //cleanup

    Game_CharXY('\r', 0, MAP_HEIGHT + 1);

    Game_Printf("Game Over! Final score: %d", score);
    Game_UnregisterPlayer1Receiver(Receiver);

    Game_ShowCursor();
    Game_GameOver();

}
