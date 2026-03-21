
#include "game.h"

#define MAX_PIPE 3
#define MAX_FLOOR 2



static void* scene[MAX_SCENE_ITEM];
static uint8_t scene_len;



static void Game_Menu();
static void Game_Over();
static void Game_Loop();
static void Game_Render();
static uint8_t Game_Detect_Collision();
static void Game_Position_Update(uint8_t input);
static void Game_End();
static Floor floors[MAX_FLOOR];
static Pipe pipes[MAX_PIPE];
static Bird bird;
static GameState currentGameState = GAME_MENU;


static void Game_Init(){
    scene_len = 0;

    for(uint8_t i = 0; i < MAX_FLOOR; i++){
        floor_init(&floors[i]);
        scene[scene_len++] = &floors[i];    
    }

    for(uint8_t i = 0; i < MAX_PIPE; i++){
        pipe_init(&pipes[i]);
        scene[scene_len++] = &pipes[i];
    }
    
    bird_init(&bird);
    scene[scene_len++] = &bird;

}

static void Game_AssetReload(){

    for(uint8_t i = 0; i < scene_len; i++){
        SceneType *ptype = (SceneType*)scene[i];
        if(*ptype == FLOOR){
            ((Floor*)scene[i])->init((Floor*)scene[i],i);
        }else if(*ptype == PIPE){
            ((Pipe*)scene[i])->init((Pipe*)scene[i],i < 1);
        }else{
            ((Bird*)scene[i])->init((Bird*)scene[i]);
        }
    }

}

static void Game_Menu(){

    Menu_render();
    uint8_t input = Game_Input();
    if(input > 0){
        Menu_Btn_clicked();
        currentGameState = GAME_LOOP;
        Game_Delay(1000);
    }
}

static void Game_Over(){

    currentGameState = GAME_MENU;
}

static void Game_Loop(){
    uint8_t input = Game_Input();
    Game_Position_Update(input);
    uint8_t collision = Game_Detect_Collision();
    if(collision == 1){
        Game_End();
        return;
    }
    Game_Render();
    Game_Delay(10);
}

void Game_Start(){
    
    gfx_init();
    Game_Init();

    while(1){

        switch(currentGameState){
            case GAME_MENU:
            
                Game_AssetReload();
                Game_Menu();
                break;

            case GAME_LOOP:

                Game_Loop();
                break;

            case GAME_OVER:

                Game_Over();
                break;

        }
        
    }

}

__attribute__((weak))
uint8_t Game_Input(){
    return 0;
}

__attribute__((weak))
void Game_Delay(uint16_t ms){
    for(uint32_t i = 0; i < 100000;i++);
}


static void Game_End(){
    currentGameState = GAME_OVER;
}


static void Game_Render(){
    gfx_clear_buffer();
    for(uint8_t i = 0; i < scene_len; i++){
        SceneType *ptype = (SceneType*)scene[i];
        if(*ptype == FLOOR){
            ((Floor*)scene[i])->render((Floor*)scene[i]);
        }else if(*ptype == PIPE){
            ((Pipe*)scene[i])->render((Pipe*)scene[i]);
        }else{
            ((Bird*)scene[i])->render((Bird*)scene[i]);
        }
    }
    gfx_update_display();

}

static uint8_t Game_Detect_Collision(){
    
    for(uint8_t i = 0; i < scene_len; i++){
        SceneType *ptype = (SceneType*)scene[i];
        uint8_t collision = 0;
        if(*ptype == PIPE){
            collision = ((Pipe*)scene[i])->detect_collision(((Pipe*)scene[i]),&bird);
            if(collision > 0)
                return 1;
        }else if(*ptype == BIRD){
            for(uint8_t f = 0; f < MAX_FLOOR; f++){
                collision = ((Bird*)scene[i])->detect_collision(((Bird*)scene[i]),&floors[f]);
                if(collision > 0)
                    return 1;
            }

        }
    }
    return 0;

}

static void Game_Position_Update(uint8_t input){
    for(uint8_t i = 0; i < scene_len; i++){
        SceneType *ptype = (SceneType*)scene[i];
        if(*ptype == PIPE){
            ((Pipe*)scene[i])->position_update((Pipe*)scene[i]);
        }else if(*ptype == BIRD){
            ((Bird*)scene[i])->position_update((Bird*)scene[i],input);
        }
    }
}
