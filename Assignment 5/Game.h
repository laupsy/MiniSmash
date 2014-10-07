//
//  Game.h
//  NYUCodebase
//
//  Created by Laura Barbera on 10/3/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Game__
#define __NYUCodebase__Game__

#include <stdio.h>
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <Math.h>
#include "Entity.h"

using namespace std;

enum GameState { STATE_START, STATE_GAME_LEVEL, STATE_GAME_OVER };

class Game {
public:
    Game();
    ~Game();
    void Init();
    void Render();
    void Update(float elapsed);
    void renderStart();
    void renderLevel();
    void renderGameOver();
    bool UpdateAndRender();
    GLuint LoadTexture(const char *image_path);
    void DrawText( GLuint textTexture, string text, float x, float y, float spacing, float size, float r, float g, float b, float a );
    
    float elapsed;
    float lastFrameTicks;
    bool done;
    
    int state;
    
    /* 
        0 = sky
        1 = standing
        2 = walking right
        3 = walking left
        4 = platform
        5 = platform distant
        6 = clouds
        7 = clouds distant
        8 = pink puff
        9 = blue puff
     */
    
    
    vector<int> gridndx = {0,0,0,6,0,0,7,0,0,0,6,
                           0,5,0,6,5,0,4,4,4,4,4,
                           7,9,6,5,4,4,4,0,6,0,7,
                           4,4,4,4,0,5,0,7,4,4,4};
    vector<Entity*> entities;
    
    SDL_Event event;
    SDL_Window* displayWindow;
    const Uint8 *keys;
    
};

#endif /* defined(__NYUCodebase__Game__) */
