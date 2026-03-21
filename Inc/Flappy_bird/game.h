#ifndef GAME_H
#define GAME_H

#include<stdint.h>
#include<stddef.h>
#include "../Bsp/header/sh1106_graphics.h"

#define MAX_SCENE_ITEM 8

typedef enum Input{
    IDLE,PRESSED
} BUTTON;

typedef enum SceneType{
    FLOOR,PIPE,BIRD
} SceneType;

typedef enum GameState{
    GAME_MENU,GAME_LOOP,GAME_OVER
} GameState;



void Game_Start();
uint8_t Game_Input();
void Game_Delay(uint16_t ms);




#include "pipe.h"
#include "floor.h"
#include "bird.h"
#include "menu.h"

#endif