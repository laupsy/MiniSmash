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
#include <fstream>
#include <sstream>

#include <Math.h>

#include "Entity.h"
#include "SheetSprite.h"

#define LEVEL_HEIGHT 16
#define LEVEL_WIDTH 22
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 22
#define TILE_SIZE 0.2f
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

using namespace std;

//enum GameState { STATE_START, STATE_GAME_LEVEL, STATE_GAME_OVER };

class Game {
public:
    
    Game();
    ~Game();
    
    void Init();
    void Update(float elapsed);
    void FixedUpdate();
    void Render();
    void RenderStart();
    void RenderLevel();
    void RenderGameOver();
    bool UpdateAndRender();
    
    void ReadLevel();
    
    float lerp(float v0, float v1, float t);
    
    bool readHeader(std::ifstream &stream);
    bool readLayerData(std::ifstream &stream);
    bool readEntityData(std::ifstream &stream);
    
    void placeEntity(const string& type, float placeX, float placeY);
    
    GLuint LoadTexture(const char *image_path);
    void DrawText( GLuint textTexture, string text, float x, float y, float spacing, float size, float r, float g, float b, float a );
    
    vector<Entity*> entities;
    
    Entity * player;
    
    unsigned char** levelData;
    
    bool done;
    
    int state;
    int mapWidth;
    int mapHeight;
    
    float elapsed;
    float lastFrameTicks;
    float timeLeftOver;
    
    const Uint8 *keys;
    
    GLuint mainSpriteTexture;
    SDL_Event event;
    SDL_Window* displayWindow;
    
};

#endif /* defined(__NYUCodebase__Game__) */
