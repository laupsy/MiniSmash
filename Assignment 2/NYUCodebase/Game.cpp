//
//  Game.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/1/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Entity.h"
#include "Game.h"
#include "Bullet.h"

Game::Game() {
    Init();
    done = false;
    lastFrameTicks = 0.0f;
}

Game::~Game() {
    SDL_Quit();
}

void Game::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("LaupsyGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_MODELVIEW);
    
    
    Bullet bullet(0.05f,10.0f,0.0f,0.0f);
    bullets.push_back(bullet);
}

void Game::Render() {
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glClearColor(0.1, 0.0, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // THIS IS WHERE U DRAW STUFF
    
    bullets[0].display();
    
    // STOP DRAWING STUFF

    SDL_GL_SwapWindow(displayWindow);
}

void Game::Update(float elapsed) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true; }
    }
}

bool Game::UpdateAndRender() {
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    Update(elapsed);
    Render();
    return done;
}

GLuint Game::LoadTexture(const char *image_path) {
    
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels); // textures were tinted blue.. so i swapped GL_RGBA for GL_BGRA
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}
