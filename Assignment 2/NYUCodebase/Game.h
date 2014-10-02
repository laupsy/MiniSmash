//
//  Game.h
//  NYUCodebase
//
//  Created by Laura Barbera on 10/1/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Game__
#define __NYUCodebase__Game__

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>

#include "Bullet.h"
#include "Entity.h"

using namespace std;

class Game {
public:
    Game();
    ~Game();
    void Init();
    void Render();
    void Update(float elapsed);
    bool UpdateAndRender();
    GLuint LoadTexture(const char *image_path);
    
    SDL_Window* displayWindow;
    bool done;
    float lastFrameTicks;
    SDL_Event event;
    
    vector<Bullet> bullets;
};


#endif /* defined(__NYUCodebase__Game__) */
