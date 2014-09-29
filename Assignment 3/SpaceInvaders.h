//
//  SpaceInvaders.h
//  NYUCodebase
//
//  Created by Laura Barbera on 9/28/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__SpaceInvaders__
#define __NYUCodebase__SpaceInvaders__


#include <SDL.h>
#include <SDL_opengl.h>

#include "Bullet.h"
#include "Ship.h"
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

class SpaceInvaders {
    
public:
    
    SpaceInvaders();
    ~SpaceInvaders();
    
    // Game functions: Start, end, update score, show menu screen, detect collision
    // Ship functions:  lose life, move up/down, collide
    // Bullet functions:  move, collide
    
    void DrawSprite(GLint texture, float x, float y, float rotation);
    GLuint LoadTexture(const char *image_path);
    
    void Init();
    void Render();
    void Update(float elapsed);
    bool UpdateAndRender();
    
    void startGame();
    void endGame();
    void updateScore();
    void showMenu(); // not enabling pause (show menu during game) because i have no idea how to do that
    
    int getLives();
    int getScore();
    
    bool collision(const Ship& e1, const Bullet& e2);
    
private:
    
    bool done;
    float lastFrameTicks, elapsed;
    SDL_Window* displayWindow;
    
    int lives, score;
    vector<Bullet*> bullets;
    vector<Ship*> ships;
    
    bool STATE_TITLE_SCREEN;
    bool STATE_GAME;
    bool STATE_GAME_OVER;
    
};

#endif /* defined(__NYUCodebase__SpaceInvaders__) */
