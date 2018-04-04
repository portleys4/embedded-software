#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

#include <stdint.h>
#include "list.h"

#define SNAKE_MAP_WIDTH 40
#define SNAKE_MAP_HEIGHT 40

#define SNAKE_SPEED 150 //Time between each snake movement, lower -> faster snake
#define MAX_SNAKE_LENGTH 45 //You win the game when you reach this length

#define FOOD_CHAR '*'
#define SNAKE_CHAR '@'

enum snake_movements{
    UP,
    DOWN,
    LEFT,
    RIGHT
};


typedef struct snake_link_t{
    uint8_t xcord; //x-coordinate of this link
    uint8_t ycord; //y-coordinate of this link
    uint8_t in_play; //is this link in play?
} snake_link_t;

typedef struct food{
    uint8_t xcord;
    uint8_t ycord;
} food_t;

/** Snake_Game_Init must be called before the game can be played
 *
 */
void SnakeGame_Init(void);

/** @} */

#endif /* Snake_GAME_H_ */
