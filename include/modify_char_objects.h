#include <stdint.h>
#include "game.h"


void ModifyCharObject(void (*modify_fun_ptr)(char_object_t* char_obj) , char_object_t* obj_array, uint16_t obj_array_len);

void MoveRight(char_object_t* o);

void MoveLeft(char_object_t* o);

void MoveRight(char_object_t* o);

void MoveDown(char_object_t* o);
