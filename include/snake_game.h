#ifndef SNAKE_GAME_H_
#define SNAKE_GAME_H_

#include <stdint.h>
#include "list.h"

#define MAP_WIDTH 40
#define MAP_HEIGHT 40

#define SNAKE_SPEED 10

enum snake_movements{
    UP,
    DOWN,
    LEFT,
    RIGHT
};


typedef struct snake_link_t{
    uint8_t xcord;
    uint8_t ycord;
    struct snake_link_t* next_link;
} snake_link_t;

typedef struct food{
    uint8_t xcord;
    uint8_t ycord;
    char food_char;
} food_t;

/** Snake_Game_Init must be called before the game can be played
 *
 */
void SnakeGame_Init(void);

/** @} */

#endif /* Snake_GAME_H_ */
