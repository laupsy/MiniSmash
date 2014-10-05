//
//  Game.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/3/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Game.h"
#include "Entity.h"

Game::Game() {
    Init();
    done = false;
    lastFrameTicks = 0.0f;
    
    state = STATE_START;
}

Game::~Game() {
    
//    for ( std::vector<GLuint*>::iterator it = spriteSheets.begin(); it != spriteSheets.end(); it++ ) {
//        delete *it;
//    }
  
    SDL_Quit();
}

void Game::Init() {
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("laupsygame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    glMatrixMode(GL_MODELVIEW);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glClearColor(0.2, 0.5, 0.9, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    setEntities();
}

void Game::Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch (state) {
        case STATE_START:
            renderStart();
            break;
        case STATE_GAME_LEVEL:
            renderLevel();
            break;
        case STATE_GAME_OVER:
            renderGameOver();
            break;
    }
    
    SDL_GL_SwapWindow(displayWindow);
}

void Game::Update(float elapsed) {
    
    
    switch (state) {
        case STATE_START:
            renderStart();
            break;
        case STATE_GAME_LEVEL:
            renderLevel();
            break;
        case STATE_GAME_OVER:
            renderGameOver();
            break;
    }
//
//    inputEvents();
    
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
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);

    spriteSheets.push_back(&textureID);
    
    return textureID;
}

void Game::DrawText( GLuint textTexture, string text, float x, float y, float spacing, float size, float r, float g, float b, float a ) {
    glBindTexture(GL_TEXTURE_2D, textTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    glTranslatef( x, y, 0.0 );
    vector<float> vertexData;
    vector<float> textCoordData;
    vector<float> colorData;
    float textureSize = 1.0/16.0f;
    for ( int i=0; i <text.size(); i++ ) {
        float textX = (float) (((int)text[i]) % 16 ) / 16.0f;
        float textY = (float) (((int)text[i]) / 16 ) / 16.0f;
        vertexData.insert(vertexData.end(), {((size+spacing) * i) + (-0.5f * size), 0.5f * size, ((size+spacing) * i) +
            (-0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), 0.5f
            * size});
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        textCoordData.insert(textCoordData.end(), {textX, textY, textX, textY + textureSize, textX + textureSize, textY + textureSize, textX + textureSize, textY });
    }
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer (2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, textCoordData.data());
    glEnableClientState( GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Game::renderStart() {
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat) {
            state = STATE_GAME_LEVEL;
        }
    }
    
    DrawText(LoadTexture("pixel_font.png"),"HELLO.TESTING.",0.3,-0.4,0.001,0.05f,1.0f,1.0,1.0,1.0);
}

void Game::renderGameOver() {
    
}

void Game::setEntities() {
    
    //    Entity * e = new Entity(LoadTexture("betty_0.png"), 0.5f, -0.7f, -0.8f, 0.6f, 0.0/216.0, 96.0f/216.0);
    //    entities.push_back(e);
    
    Entity player = Entity(
                           LoadTexture("betty_0.png"),
                           0.0f, 0.0f, // x, y
                           ( gridSize * 0 ) / spriteSheetSize, // x index
                           ( gridSize * 0 ) / spriteSheetSize, // y index
                           gridSize, // width
                           gridSize  // height
                           );
    
    Entity platform_1 = Entity(
                               LoadTexture("arne_sprites.png"),
                               -0.918f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_2 = Entity(
                               LoadTexture("arne_sprites.png"),
                               -0.758f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_3 = Entity(
                               LoadTexture("arne_sprites.png"),
                               -0.598f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_4 = Entity(
                               LoadTexture("arne_sprites.png"),
                               -0.438f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_5 = Entity(
                               LoadTexture("arne_sprites.png"),
                               -0.278f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_6 = Entity(
                               LoadTexture("arne_sprites.png"),
                               -0.118f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_7 = Entity(
                               LoadTexture("arne_sprites.png"),
                               0.042f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_8 = Entity(
                               LoadTexture("arne_sprites.png"),
                               0.202f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_9 = Entity(
                               LoadTexture("arne_sprites.png"),
                               0.362f, -0.918f, // x, y
                               ( gridSize * 2.5) , // x index
                               ( gridSize * 3) , // y index
                               gridSize * 1.0, // width
                               gridSize * 1.5 // height
                               );
    
    Entity platform_10 = Entity(
                                LoadTexture("arne_sprites.png"),
                                0.522f, -0.918f, // x, y
                                ( gridSize * 2.5) , // x index
                                ( gridSize * 3) , // y index
                                gridSize * 1.0, // width
                                gridSize * 1.5 // height
                                );
    
    Entity platform_11 = Entity(
                                LoadTexture("arne_sprites.png"),
                                0.682f, -0.918f, // x, y
                                ( gridSize * 2.5) , // x index
                                ( gridSize * 3) , // y index
                                gridSize * 1.0, // width
                                gridSize * 1.5 // height
                                );
    
    Entity platform_12 = Entity(
                                LoadTexture("arne_sprites.png"),
                                1.526f, -0.918f, // x, y
                                ( gridSize * 2.5) , // x index
                                ( gridSize * 3) , // y index
                                gridSize * 1.0, // width
                                gridSize * 1.5 // height
                                );
    
    entities.push_back(player);
    entities.push_back(platform_1);
    entities.push_back(platform_2);
    entities.push_back(platform_3);
    entities.push_back(platform_4);
    entities.push_back(platform_5);
    entities.push_back(platform_6);
    entities.push_back(platform_7);
    entities.push_back(platform_8);
    entities.push_back(platform_9);
    entities.push_back(platform_10);
    entities.push_back(platform_11);
    entities.push_back(platform_12);
    
}

void Game::renderLevel() {

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        
        else if ( event.type == SDL_KEYDOWN && !event.key.repeat && event.key.keysym.scancode != SDL_SCANCODE_SPACE ) {
            entities[0].resetPhysics();
        }
        else if ( event.type == SDL_KEYUP ) {
            if ( event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                entities[0].slowDown();
            }
        }
    }
    
    if ( event.key.keysym.scancode == SDL_SCANCODE_LEFT ) {
        if ( !event.key.repeat ) entities[0].moveLeft();
        else entities[0].constantLeft();
    }
    if ( event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) {
        if ( !event.key.repeat ) entities[0].moveRight();
        else entities[0].constantRight();
    }
    if ( event.key.keysym.scancode == SDL_SCANCODE_SPACE && event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) {
        entities[0].moveRight();
        entities[0].jump();
    }
    
    for ( size_t i = 0; i < entities.size(); i++ ) {
        entities[i].Draw(mediumScale);
    }

    entities[0].getCollision(entities[1]);
    
    entities[0].fall(elapsed);
    
//    cout << "Colliding: " << entities[0].colliding << ", Attempted direction: " << entities[0].direction_x << ", " << entities[0].direction_y << "\n";
    
}