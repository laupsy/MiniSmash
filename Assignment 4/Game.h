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
    
    void inputEvents();
    
    bool collision(const Entity& e1, const Entity& e2);
    bool collidesTop(const Entity& e1, const Entity& e2);
    bool collidesBot(const Entity& e1, const Entity& e2);
    bool collidesLeft(const Entity& e1, const Entity& e2);
    bool collidesRight(const Entity& e1, const Entity& e2);
    
    bool UpdateAndRender();
    
    GLuint LoadTexture(const char *image_path);
    void DrawText( GLuint textTexture, string text, float x, float y, float spacing, float size, float r, float g, float b, float a );
    
    void setEntities();
    
    float elapsed;
    float lastFrameTicks;
    bool done;
    
    float smallScale = 0.2f;
    float mediumScale = 0.4f;
    float largeScale = 0.6f;
    
    float spriteSheetSize = 216.0f;
    float gridSize = 48.0f / spriteSheetSize;
    
    int state;
    int score = 0;
    int lives = 1;
    
    SDL_Event event;
    SDL_Window* displayWindow;
    const Uint8 *keys;
    
    vector<GLuint*> spriteSheets;
    vector<Entity> entities;
    
};

#endif /* defined(__NYUCodebase__Game__) */
