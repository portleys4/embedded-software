#include <stdint.h>
#include "tag_game.h"
#include "game.h"
#include "timing.h"
#include "modify_char_objects.h"

char_object_t player;
char_object_t ai;

uint8_t player_movement_lock = 0; //Player can only move once every 200ms
uint32_t ai_delay;

uint8_t game_id;
uint8_t score;
void Tag_Init(void){
    game_id = Game_Register("TAG", "Tag Game", Play, Help);
    Game_RegisterCallback(game_id, Callback);
}

void Help(void){
    Game_Printf("WASD to move the player, avoid the AI who is 'it' ");
}

void Callback(int argc, char* argv[]){
    if(argc == 0) Game_Log(game_id, "too few args");
    if(strcasecmp(argv[0], "reset") == 0){
        score = 0;
        Game_Log(game_id, "Scores reset");
    }else{game_id, "command not supoprted"}
}


void Play(void){
    Game_ClearScreen();
    Game_DrawRect(0, 0, TAG_MAP_WIDTH, TAG_MAP_HEIGHT);
    Game_RegisterPlayer1Receiver(Receiver);
    Game_HideCursor();
}

void GameTick(void){
    static uint32_t ai_timer = 0;

    if(TimeSince(ai_timer) > ai_delay){
        ai_timer = TimeNow();
        AI_Update();
    }
}

uint8_t CollisionCheck(){
    return (player.x == ai.x) && (player.y == ai.y);
}

void AI_Update(){

    int8_t hdist = ai.x - player.x;
    if(hdist < 0) hdist = hdist * -1;

    int8_t vdist = ai.y - player.y;
    if(vdist < 0) vdist = vdist * -1;

     
}

void Receiver(uint8_t c){
    if(player_movement_lock) return;

    switch(c){
        case 'w':
        case 'W':
            ModifyCharObject(MoveUp, &player, 1);
            break;
        case 'a':
        case 'A':
            ModifyCharObject(MoveLeft, &player, 1);
            break;
        case 's':
        case 'S':
            ModifyCharObject(MoveDown, &player, 1);
            break;
        case 'd':
        case 'D':
            ModifyCharObject(MoveRight, &player, 1);
            break;
     }

}

void Unlock_Player_Movement(void){
    player_movement_lock = 0;
}
