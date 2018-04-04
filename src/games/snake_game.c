#include "snake_game.h"
#include "project_settings.h"
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

uint8_t score; //Current score of the game, increments by 1 whenever food is picked up
uint8_t game_id;
enum snake_movements curr_direction; //Current direction that the snake is moving
snake_link_t snake[MAX_SNAKE_LENGTH]; //The entire snake, index 0 is the tail and it grows from the head
food_t food; //The current food on the map


void SnakeGame_Init(void){

    game_id = Game_Register("SNAKE", "Classic Snake Game", Play, Help);
    Game_RegisterCallback(game_id, Callback);
}

//Print the directions for playing the game
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


//Starts the game
void Play(void){

    //Initial setup
    Game_ClearScreen();
    Game_DrawRect(0, 0, SNAKE_MAP_WIDTH, SNAKE_MAP_HEIGHT);
    Game_RegisterPlayer1Receiver(Receiver);
    Game_HideCursor();

    //Reset everything
    uint8_t i;
    for(i = 1; i<MAX_SNAKE_LENGTH; i++){
        snake[i].xcord = 0;
        snake[i].ycord = 0;
        snake[i].in_play = 0;
    }
    score = 0;

    //Start the snake in the middle of the map
    snake[0].xcord = SNAKE_MAP_WIDTH / 2;
    snake[0].ycord = SNAKE_MAP_HEIGHT / 2;
    snake[0].in_play = 1;
    curr_direction = UP;
    Game_CharXY(SNAKE_CHAR, snake[0].xcord, snake[0].ycord);

    //Spawn some food
    SpawnFood();

    //Move the snake every SNAKE_SPEED milliseconds
    Task_Schedule((task_t)GameTick, 0, 3000, SNAKE_SPEED);
}

//Draw the current state of the snake
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


//Add a link at the head of the snake based on the current direction of the snake
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


/**
 * Called every SNAKE_SPEED milliseconds
 * Checks if you hit yourself or a wall or food, moves the snake
 */
void GameTick(void){

    //Since index 0 of the snake is the tail, we need to find the head
    uint8_t i = 0;
    while(snake[i+1].in_play){
        i++;
    }

    //Did you hit a wall?
    if(snake[i].xcord <= 0 || snake[i].xcord >= SNAKE_MAP_WIDTH){
        GameOver();
        return;
    }

    //Did you hit a wall?
    if(snake[i].ycord <= 0 || snake[i].ycord >= SNAKE_MAP_HEIGHT){
        GameOver();
        return;
    }

    //Did you hit yourself?
    uint8_t j;
    for(j = 0; j<i; j++){
        if(snake[j].xcord == snake[i].xcord && snake[j].ycord == snake[i].ycord){
            GameOver();
            return;
        }
    }

    //Did you pickup food?
    if(snake[i].xcord == food.xcord && snake[i].ycord == food.ycord){
        score++;
        SpawnFood(); //Respawn food
        AddLink();   //Add a new link to the head where the food was
    }else{
        MoveSnake();
    }

    //Update the image
    //Drawing the snake comes first so that if food is spawned on top of one of the links it will still appear
    DrawSnake();
    DrawFood();

}

/**
 * Move the snake by one by add a new link at the head and removing the link at the tail
 * Shifting everything over by one
 */
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

    //Remove
    snake[i-1].in_play = 0;
    snake[i-1].xcord = 0;
    snake[i-1].ycord = 0;

}

//Draw the food
void DrawFood(void){
    Game_CharXY(FOOD_CHAR, food.xcord, food.ycord);
}


//Randomly spawn food somewhere within the map
void SpawnFood(void){

    //we just picked up food, so we don't need to clear current spot

    food.xcord = random_int(1, SNAKE_MAP_WIDTH-1);
    food.ycord = random_int(1, SNAKE_MAP_HEIGHT-1);
    DrawFood();


}

/**
 * Handle user input
 * You cannot immediately change the direction of the snake by 180
 * because you would just instantly hit yourself
 */
void Receiver(uint8_t c){
    switch(c){
        case 'w':
        case 'W':
            if(curr_direction != DOWN) curr_direction = UP;
            break;
        case 'a':
        case 'A':
            if(curr_direction != RIGHT) curr_direction = LEFT;
            break;
        case 's':
        case 'S':
            if(curr_direction != UP) curr_direction = DOWN;
            break;
        case 'd':
        case 'D':
            if(curr_direction != LEFT) curr_direction = RIGHT;
            break;
        default:
            break;
    }
}

//Cleanup
void GameOver(){
    Task_Remove(GameTick, 0);
    Game_UnregisterPlayer1Receiver(Receiver);
    Game_CharXY('\r', 0, SNAKE_MAP_HEIGHT + 1);
    Game_Printf("Game Over! Final score: %d", score);
    Game_ShowCursor();
    Game_GameOver();

}
