#include <stdint.h> 
#include "modify_char_objects.h"




void ModifyCharObject(void (*modify_fun_ptr)(char_object_t* char_obj), char_object_t* obj_array, uint16_t obj_array_len){

    uint16_t i = 0;
    while(i < obj_array_len){
        modify_fun_ptr(obj_array++);
        i++;
    }
}

void MoveRight(char_object_t* o){
    if(o->x < 40 - 1){
        Game_CharXY(' ', o->x, o->y);
        o->x++;
        Game_CharXY(o->c, o->x, o->y);
    }
}

void MoveLeft(char_object_t* o){
    if(o->x > 1){
        Game_CharXY(' ', o->x, o->y);
        o->x--;
        Game_CharXY(o->c, o->x, o->y);
    }
}

void MoveUp(char_object_t* o){
    if(o->y > 1){
        Game_CharXY(' ', o->x, o->y);
        o->y--;
        Game_CharXY(o->c, o->x, o->y);
    }
}

void MoveDown(char_object_t* o){
    if(o->y < 40 - 1){
        Game_CharXY(' ', o->x, o->y);
        o->y++;
        Game_CharXY(o->c, o->x, o->y);
    }
}