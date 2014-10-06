//
//  Game.cpp
//  NYUCodebase
//
//  Created by Laura Barbera on 10/3/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#include "Game.h"
#include "Entity.h"

// fixed time step

#define MAX_UPDATES = 20;

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
    
    glClearColor(0.6, 0.4, 0.5, 1.0);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat) {
            state = STATE_GAME_LEVEL;        }
    }

    DrawText(LoadTexture("pixel_font.png"),"CAPTURE ALL THE SNAKES.  ENTER TO START.",-0.8,0.7,0.001,0.04f,1.0f,1.0,1.0,1.0);
}


 // for individual items because jumping and freefall is not behaving as expected...

bool Game::collision(const Entity& e1, const Entity& e2) {
    
                        // right edge of e1 is within 0.02 units of the left edge of e2
    
    bool collideRight = ( ( e1.x + e1.width/2.0 < ( e2.x - e2.width/2.0 - 0.005 ) )
                       && ( e1.x + e1.width/2.0 > ( e2.x - e2.width/2.0 + 0.01 )));
    
                        // left edge of e1 is within 0.02 units of the right edge of e2
    
    bool collideLeft = ( ( e1.x - e1.width/2.0 > ( e2.x + e2.width/2.0 + 0.005 ) )
                      && ( e1.x - e1.width/2.0 < ( e2.x + e2.width/2.0 - 0.005 )));
    
                        // bottom edge of e1 is within 0.02 units of the top edge of e2
    
    bool collideBottom = ( ( e1.y - e1.height/2.0 < ( e2.y + e2.height/2.0 + 0.005 ) )
                        && ( e1.y - e1.height/2.0 > ( e2.y + e2.height/2.0 - 0.005 )));
    
                        // top edge of e1 is within 0.02 units of the bottom edge of e2
    
    bool collideTop = ( ( e1.y + e1.height/2.0 > ( e2.y - e2.height/2.0 + 0.005 ) )
                     && ( e1.y + e1.height/2.0 < ( e2.y - e2.height/2.0 - 0.005 )));
//    
//    
//    cout << "Collide right: " << collideRight << endl;
//    cout << "Collide left: " << collideLeft << endl;
//    cout << "Collide top: " << collideTop << endl;
//    cout << "Collide bottom: " << collideBottom << endl;
//    
    
    return collideRight && collideLeft && collideBottom && collideTop;
}

bool Game::collidesBot(const Entity& e1, const Entity& e2 ) {
    return ( (   e1.y - e1.height/2.0 < ( e2.y + e2.height/2.0 + 0.002 ) )
            && ( e1.y - e1.height/2.0 > ( e2.y + e2.height/2.0 - e2.height ))
            && ( e1.x + e1.width/2.0 <= ( e2.x + e2.width/2.0 + 0.115 ))
            && ( e1.x - e1.width/2.0 >= ( e2.x - e2.width/2.0 - 0.115 )));
}

bool Game::collidesTop(const Entity& e1, const Entity& e2 ) {
    return ( (   e1.y + e1.height/2.0 < ( e2.y - e2.height/2.0 - 0.002 ) )
            && ( e1.y + e1.height/2.0 > ( e2.y - e2.height/2.0 + e2.height ))
            && ( e1.x + e1.width/2.0 <= ( e2.x + e2.width/2.0 + 0.115 ))
            && ( e1.x - e1.width/2.0 >= ( e2.x - e2.width/2.0 - 0.115 )));
}

bool Game::collidesLeft(const Entity& e1, const Entity& e2 ) {
    return ( (   e1.x - e1.width/2.0 < ( e2.x + e2.width/2.0 + 0.005 ) )
            && ( e1.x - e1.width/2.0 > ( e2.x + e2.width/2.0 - 0.005 ))
            && ( e1.y + e1.height/2.0 <= ( e2.y + e2.height/2.0 + 0.115 ))
            && ( e1.y - e1.height/2.0 >= ( e2.y - e2.height/2.0 - 0.115 )));
}

bool Game::collidesRight(const Entity& e1, const Entity& e2 ) {
    return ( (   e1.x + e1.width/2.0 > ( e2.x - e2.width/2.0 - 0.005 ) )
            && ( e1.x + e1.width/2.0 < ( e2.x - e2.width/2.0 + 0.005 ))
            && ( e1.y + e1.height/2.0 <= ( e2.y + e2.height/2.0 + 0.115 ))
            && ( e1.y - e1.height/2.0 >= ( e2.y - e2.height/2.0 - 0.115 )));
}

//bool onPlatform(const Entity& e1) {
//    bool top = ( e1.y < entities[)
//}

void Game::renderGameOver() {
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_S && !event.key.repeat) {
            state = STATE_START;
        }
    }
//    entities.clear();
    
    setEntities();
    
    DrawText(LoadTexture("pixel_font.png"),"GAME OVER.  PRESS S TO START OVER. ",-0.925,0.9,0.001,0.05f,1.0f,1.0,1.0,1.0);
}

void Game::setEntities() {
    
    entities.clear();
    
    score = 0;
    
    //    Entity * e = new Entity(LoadTexture("betty_0.png"), 0.5f, -0.7f, -0.8f, 0.6f, 0.0/216.0, 96.0f/216.0);
    //    entities.push_back(e);
    
    float startX = -0.918f;
    float nextX = 0.158f;
    
    Entity player = Entity(
                           LoadTexture("betty_0.png"),
                           0.8f, 0.9f, // x, y
                           ( gridSize * 0 ) / spriteSheetSize, // x index
                           ( gridSize * 2.0 ) / spriteSheetSize, // y index
                           gridSize, // width
                           gridSize  // height
                           );
    
    entities.push_back(player);
    lives = 1;
                                 
    // bottom row
    
    for ( int i = 0; i < 4; i++ ) {
        Entity p = Entity(
                          LoadTexture("arne_sprites.png"),
                          startX + nextX * i, -0.900f, // x, y
                          ( gridSize * 2.5) , // x index
                          ( gridSize * 2.8) , // y index
                          gridSize * 1.0, // width
                          gridSize * 1.4 // height
                          );
        entities.push_back(p);
    }
    
    startX = 0.918f;
    
    for ( int i = 0; i < 7; i++ ) {
        Entity p = Entity(
                          LoadTexture("arne_sprites.png"),
                          startX + nextX * i, -0.900f, // x, y
                          ( gridSize * 2.5) , // x index
                          ( gridSize * 2.8) , // y index
                          gridSize * 1.0, // width
                          gridSize * 1.4 // height
                          );
        entities.push_back(p);
    }
    
    startX = -0.918f;
                                 
    // middle left row
    
    for ( int i = 0; i < 4; i++ ) {
        Entity p = Entity(
                          LoadTexture("arne_sprites.png"),
                          startX + nextX * i, -0.118f, // x, y
                          ( gridSize * 2.5) , // x index
                          ( gridSize * 2.8) , // y index
                          gridSize * 1.0, // width
                          gridSize * 1.4 // height
                          );
        entities.push_back(p);
    }
    
    startX = 0.938f;
    
    // middle right row
    
    for ( int i = 0; i < 4; i++ ) {
        Entity p = Entity(
                          LoadTexture("arne_sprites.png"),
                          startX - nextX * i, -0.118f, // x, y
                          ( gridSize * 2.5) , // x index
                          ( gridSize * 2.8) , // y index
                          gridSize * 1.0, // width
                          gridSize * 1.4 // height
                          );
        entities.push_back(p);
    }
    
    startX = 0.938f;
                                 
    // top right row
    
    for ( int i = 0; i < 4; i++ ) {
        Entity p = Entity(
                          LoadTexture("arne_sprites.png"),
                          startX + nextX * i, 0.618f, // x, y
                          ( gridSize * 2.5) , // x index
                          ( gridSize * 2.8) , // y index
                          gridSize * 1.0, // width
                          gridSize * 1.4 // height
                          );
        entities.push_back(p);
    }
    
    Entity p = Entity(
                      LoadTexture("characters_3.png"),
                      -0.4f, -0.65f, // x, y
                      ( gridSize * 0) , // x index
                      ( gridSize * 3.5) , // y index
                      gridSize * 0.5, // width
                      gridSize * 1.0 // height
                      );
    entities.push_back(p);
    
    Entity p1 = Entity(
                       LoadTexture("characters_3.png"),
                       -0.8f, -0.65f, // x, y
                       ( gridSize * 0) , // x index
                       ( gridSize * 3.5) , // y index
                       gridSize * 0.5, // width
                       gridSize * 1.0 // height
                       );
    entities.push_back(p1);
    
    Entity p2 = Entity(
                       LoadTexture("characters_3.png"),
                       -0.7f, 0.35f, // x, y
                       ( gridSize * 0) , // x index
                       ( gridSize * 3.5) , // y index
                       gridSize * 0.5, // width
                       gridSize * 1.0 // height
                       );
    entities.push_back(p2);
    
    Entity p3 = Entity(
                       LoadTexture("characters_3.png"),
                       -0.4f, -0.35f, // x, y
                       ( gridSize * 0) , // x index
                       ( gridSize * 3.5) , // y index
                       gridSize * 0.5, // width
                       gridSize * 1.0 // height
                       );
    entities.push_back(p3);
    
    Entity p4 = Entity(
                       LoadTexture("characters_3.png"),
                       -0.2f, -0.55f, // x, y
                       ( gridSize * 1) , // x index
                       ( gridSize * 3.5) , // y index
                       gridSize * 0.5, // width
                       gridSize * 1.0 // height
                       );
    entities.push_back(p4);
    
}

void Game::renderLevel() {
    
    
    for ( size_t i = 0; i < lives; i++ ) {
        Entity life = Entity(
                             LoadTexture("betty_0.png"),
                             -0.75f + i/10.0, 0.91f, // x, y
                             ( gridSize * 0 ) / spriteSheetSize, // x index
                             ( gridSize * 0 ) / spriteSheetSize, // y index
                             gridSize, // width
                             gridSize  // height
                             );
        life.Draw(smallScale);
    }
    
    DrawText(LoadTexture("pixel_font.png"),to_string(score),-0.925,0.9,0.001,0.05f,1.0f,1.0,1.0,1.0);
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if ( event.type == SDL_KEYDOWN && !event.key.repeat ) {
            entities[0].resetPhysics();
        }
        else if ( event.type == SDL_KEYUP ) {
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
    if ( event.key.keysym.scancode == SDL_SCANCODE_A ) {
        if ( !event.key.repeat ) entities[0].moveDown();
    }
    if ( event.key.keysym.scancode == SDL_SCANCODE_SPACE ) {
        if ( !event.key.repeat ) entities[0].jump();
        if ( event.key.keysym.scancode == SDL_SCANCODE_LEFT ) {
            if ( !event.key.repeat ) entities[0].moveLeft();
            else entities[0].constantLeft();
        }
        if ( event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) {
            if ( !event.key.repeat ) entities[0].moveRight();
            else entities[0].constantRight();
        }
    }
    for ( size_t i = 0; i < entities.size(); i++ ) {
        entities[i].Draw(mediumScale);
    }
    
    if ( collidesBot(entities[0],entities[1]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[2]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[3]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[4]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[5]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[6]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[7]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[8]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[9]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[10]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[11]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[12]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[13]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[14]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[15]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[16]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[17]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[18]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[19]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[20]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[21]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[22]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[23]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[24]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[25]) ) entities[0].floating = false;
    
    else entities[0].floating = true;
    
    if ( entities[0].floating == false ) entities[0].jumping = false;
    
    entities[26].constantRight();
    entities[26].offScreen();
    entities[27].constantLeft();
    entities[27].offScreen();
    entities[28].constantLeft();
    entities[28].offScreen();
    
//    // lose life if bump head!
//    
//    if ( collidesTop(entities[0],entities[13]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[14]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[15]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[16]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[17]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[18]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[19]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[20]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[21]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[22]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[23]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[24]) ) cout << "bump" << endl;
//    if ( collidesTop(entities[0],entities[25]) ) cout << "bump" << endl;
    
    if ( collidesLeft(entities[0],entities[26]) || collidesRight(entities[0],entities[26])) {
        score += 10;
        entities[26].setCoords(2.0,2.0);
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
    }
    
    if ( collidesLeft(entities[0],entities[27]) || collidesRight(entities[0],entities[27])) {
        score += 10;
        entities[27].setCoords(-2.0,-2.0);
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
    }
    
    if ( collidesLeft(entities[0],entities[28]) || collidesRight(entities[0],entities[28])) {
        score += 10;
        entities[28].setCoords(-2.0,-2.0);
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
    }
    
    if ( collidesLeft(entities[0],entities[29]) || collidesRight(entities[0],entities[29])) {
        score += 10;
        entities[29].setCoords(-2.0,-2.0);
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
    }
    
    if ( collidesLeft(entities[0],entities[30]) || collidesRight(entities[0],entities[30])) {
        score += 10;
        entities[30].setCoords(-2.0,-2.0);
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
    }
    
    if ( collidesLeft(entities[0],entities[31]) || collidesRight(entities[0],entities[31])) {
        score += 10;
        entities[31].setCoords(-2.0,-2.0);
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
        entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
    }
    
    entities[0].fall();
    entities[0].offScreen();
    
    if ( entities[0].y < -1.4f ) {
        lives--;
    }
    
    if ( lives == 0 ) {
        state = STATE_GAME_OVER;
        entities[0].resetPhysics();
        entities[0].floating = false;
    }
    
//    cout << "Colliding: " << entities[0].colliding << ", Attempted direction: " << entities[0].direction_x << ", " << entities[0].direction_y << "\n";
    
}