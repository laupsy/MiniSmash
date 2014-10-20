
#include "Game.h"
#include "Entity.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

Game::Game() {
    
    Init();
    done = false;
    lastFrameTicks = 0.0f;
    timeLeftOver = 0.0f;
    
    state = STATE_START;
    
}

Game::~Game() {
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
    
}

void Game::FixedUpdate() {
    for ( size_t i = 0; i < entities.size(); i++ ) {
        entities[i].FixedUpdate();
    }
}

bool Game::UpdateAndRender() {
    float fixedElapsed = elapsed + timeLeftOver;
    
    if ( fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    
    while ( fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
        FixedUpdate();
    }
    
    timeLeftOver = fixedElapsed;
    
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

bool Game::collidesBot(const Entity& e1, const Entity& e2 ) {
    return ( (   e1.y - e1.height/2.0 < ( e2.y + e2.height/2.0 + 0.002 ) )
            && ( e1.y - e1.height/2.0 > ( e2.y + e2.height/2.0 - e2.height * 1.005 ))
            && ( e1.x + e1.width/2.0 <= ( e2.x + e2.width/2.0 + 0.115 ))
            && ( e1.x - e1.width/2.0 >= ( e2.x - e2.width/2.0 - 0.115 )));
}

bool Game::collidesTop(const Entity& e1, const Entity& e2 ) {
    return ( (   e1.y + e1.height/2.0 < ( e2.y - e2.height/2.0 - 0.002 ) )
            && ( e1.y + e1.height/2.0 > ( e2.y - e2.height/2.0 + e2.height * 1.005 ))
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
    
    setEntities();
    
    DrawText(LoadTexture("pixel_font.png"),"GAME OVER.  PRESS S TO START OVER. ",-0.925,0.9,0.001,0.05f,1.0f,1.0,1.0,1.0);
}

void Game::setEntities() {
    
    entities.clear();
    
    score = 0;
    
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
    
    for ( int i = 0; i < 8; i++ ) {
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
    
    startX = 0.938f;
    
    // top right row
    
    for ( int i = 0; i < 14; i++ ) {
        Entity p = Entity(
                          LoadTexture("characters_3.png"),
                          -0.4f, -0.65f, // x, y
                          ( gridSize * 0) , // x index
                          ( gridSize * 3.5) , // y index
                          gridSize * 0.5, // width
                          gridSize * 1.0 // height
                          );
        entities.push_back(p);
    }
    
    startX = 0.938f;
    
    // top right row
    
    for ( int i = 0; i < 14; i++ ) {
        Entity p = Entity(
                          LoadTexture("characters_3.png"),
                          0.1f, 0.15f, // x, y
                          ( gridSize * 0) , // x index
                          ( gridSize * 3.5) , // y index
                          gridSize * 0.5, // width
                          gridSize * 1.0 // height
                          );
        entities.push_back(p);
    }
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
        if ( entities[i].visible == false ) {
            entities[i].y = 15.0;
            entities[i].u = 0.0f;
            entities[i].v = 0.0f;
        }
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
    else if ( collidesBot(entities[0],entities[26]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[27]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[28]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[29]) ) entities[0].floating = false;
    else if ( collidesBot(entities[0],entities[30]) ) entities[0].floating = false;
    
    else entities[0].floating = true;
    
    if ( entities[0].floating == false ) entities[0].jumping = false;
    
    for ( size_t i = 28; i < entities.size(); i++ ) {
        
        // Make them look like they're slithering
        
        if ( collidesLeft(entities[0],entities[i]) || collidesRight(entities[0],entities[i])) {
            score += 10;
            entities[i].visible = false;
            entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
            entities[0].u = ( gridSize * 2 ) / spriteSheetSize;
        }
        
        if ( (rand()% 10 + 1) % 2 == 0 ) {
            
            if ( entities[i].x > 0.95f ) {
                entities[i].x = -0.95f;
            }
            if ( entities[i].x < -0.95f ) {
                entities[i].x = 0.95f;
            }
            if ( entities[i].y > 0.95f ) {
                entities[i].y = -0.95f;
            }
            if ( entities[i].y < -0.95f ) {
                entities[i].y = 0.95f;
            }
            
            entities[i].x += 0.01f;
            entities[i].y += 0.01f;
        }
        else if ( (rand()% 10 + 1) % 2 != 0 ) {
            entities[i].x += 0.01f;
            entities[i].y -= 0.01f;
        }
        entities[i].offScreen();
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
    
}