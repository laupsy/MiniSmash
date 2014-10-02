#include "SpaceInvaders.h"
#include <iostream>
using namespace std;

#define SHEET_SIZE 216

SpaceInvaders::SpaceInvaders() {
    
    Init();
    bool done = false;
    score = 0;
    state = STATE_MAIN_MENU;
    
}

void SpaceInvaders::Init() {
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("LaupsyGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_MODELVIEW);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    loadEntities();
    
}

void SpaceInvaders::loadEntities() {
    
    textSheet = loadTexture("font.png");
    movedirection = 1;
    
    float gridWidth = 48.0f / SHEET_SIZE;
    float gridHeight = 48.0f / SHEET_SIZE;
    
    // x, y, x_sheet, y_sheet, width, height, visibility, texture
    
    Sprite zelda = Sprite( 0.5,
                          -0.7f,
                           0.0f / SHEET_SIZE,
                           0.0f / SHEET_SIZE,
                           gridWidth,
                           gridHeight,
                           true,
                           loadTexture("sprites.png") );
    
    zeldas.push_back(zelda);
    
    for (int i = 0; i < 80; i++) {
        
        // x, y, x_sheet, y_sheet, width, height, visibility, texture
        
        barnAliens.insert(barnAliens.end(), Sprite( -0.8f,
                                                     0.6f,
                                                     0.0  / SHEET_SIZE,
                                                     96.0f / SHEET_SIZE,
                                                     gridWidth,
                                                     gridHeight,
                                                     true,
                                                     loadTexture("sprites.png") ) );
    }
    
    // Make 3 rows
    
    int spriteCounter = 0;
    bool row2 = true;
    bool row3 = true;
    bool row4 = true;
    
    for ( size_t i = 0; i < barnAliens.size(); i++) {
        if (spriteCounter < 20) {
            barnAliens[i].setX(barnAliens[i].getX() + spriteCounter*0.08);
            spriteCounter++;
        }
        else if ( row2 ){
            spriteCounter = 0;
            for (size_t i = 20; i < barnAliens.size(); i++) {
                barnAliens[i].setY(barnAliens[i].getY() - 0.2f);
            }
            row2 = false;
            row3 = true;
            row4 = true;
        }
        else if ( row3 ) {
            spriteCounter = 0;
            for (size_t i = 40; i < barnAliens.size(); i++) {
                barnAliens[i].setY(barnAliens[i].getY() - 0.2f);
            }
            row3 = false;
            row4 = true;
        }
        else if ( row4 ) {
            spriteCounter = 0;
            for ( size_t i = 60; i < barnAliens.size(); i++ ) {
                barnAliens[i].setY(barnAliens[i].getY() - 0.2f);
            }
            row4 = false;
        }
    }
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        Sprite newBullet = Sprite( 0.0f,
                                   0.0f,
                                   144.0  / SHEET_SIZE,
                                   96.0f / SHEET_SIZE,
                                   gridWidth,
                                   gridHeight,
                                   false,
                                   loadTexture("sprites.png") );
        
        bullets.push_back(newBullet);
    }
}

void SpaceInvaders::inputActions() {
    
    if ( state == STATE_GAME_LEVEL ) {
        
        if ( keys[SDL_SCANCODE_Q] ) {
            shootBullet(zeldas[0]);
            zeldas[0].setV( 0.0f / SHEET_SIZE );
            zeldas[0].setU( 96.0f / SHEET_SIZE );
        }
        
        if ( keys[SDL_SCANCODE_P] ) state = STATE_PAUSE;
        
        if ( keys[SDL_SCANCODE_LEFT] ) {
            if ( ( zeldas[0].getX() - 0.003f ) > - 1.0 ) {
                zeldas[0].setX(zeldas[0].getX() - 0.009f );
            }
        }
        
        if ( keys[SDL_SCANCODE_LEFT] && !keys[SDL_SCANCODE_Q] ) {
            zeldas[0].setV( 0.0f / SHEET_SIZE );
            zeldas[0].setU( 144.0f / SHEET_SIZE );
        }
        
        if ( keys[SDL_SCANCODE_RIGHT] ) {
            if ( ( zeldas[0].getX() + 0.003f) < 1.0 ) {
                zeldas[0].setX(zeldas[0].getX() + 0.009f );
            }
        }
        
        if ( keys[SDL_SCANCODE_RIGHT] && !keys[SDL_SCANCODE_Q] ) {
            zeldas[0].setV( 0.0f / SHEET_SIZE );
            zeldas[0].setU( 0.0f / SHEET_SIZE );
        }
    }
    
    else if ( state == STATE_MAIN_MENU ) {
        if ( keys[SDL_SCANCODE_S] ) state = STATE_GAME_LEVEL;
        if ( keys[SDL_SCANCODE_R] ) state = STATE_MAIN_MENU;
    }
    
    else if ( state == STATE_PAUSE ) {
        if ( keys[SDL_SCANCODE_S] ) state = STATE_GAME_LEVEL;
    }
    
    else if ( state == STATE_GAME_OVER) {
        if ( keys[SDL_SCANCODE_R] ) state = STATE_GAME_LEVEL;
    }
    
}

void SpaceInvaders::isColliding() {
    
    float bufferSpace = 0.3;
    
    for (int k = 0; k < bullets.size(); k++) {
        for (int j = 0; j < barnAliens.size(); j++){
            
            if ( bullets[k].getX() > (barnAliens[j].getX() - bufferSpace*barnAliens[j].getWidth()) &&
                 bullets[k].getX() < (barnAliens[j].getX() + bufferSpace*barnAliens[j].getWidth()) &&
                 bullets[k].getY() > (barnAliens[j].getY() - bufferSpace*barnAliens[j].getHeight()) &&
                 bullets[k].getY() < (barnAliens[j].getY() + bufferSpace*barnAliens[j].getHeight()) &&
                 bullets[k].visible ) {
                
                barnAliens[j].setV( 48.0f );
                barnAliens[j].setU( 96.0f );
                
                score += 10;
                
                barnAliens.erase(barnAliens.begin() + j);
                bullets[k].visible = false;
                
                j--;
                
            }
        }
    }
}

void SpaceInvaders::behaviorActions() {
    isColliding();
}

bool SpaceInvaders::Update() {
    
    SDL_Event event;
    keys = SDL_GetKeyboardState(NULL);
    float lastFrameTicks = 0.0f;
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    cooldown++;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
    }
    
    inputActions();
    behaviorActions();
    
    if ( score == 800 ) state = STATE_GAME_OVER;
    if ( elapsed >= 35.000 ) state = STATE_GAME_OVER;
    
    Render();
    
    return done;
    
}

void SpaceInvaders::Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch (state) {
        case STATE_MAIN_MENU:
            renderMain();
            break;
        case STATE_PAUSE:
            renderPause();
            break;
        case STATE_GAME_LEVEL:
            renderLevel();
            break;
        case STATE_GAME_OVER:
            renderEnd();
            break;
    }
    
    SDL_GL_SwapWindow(displayWindow);
    
}

void SpaceInvaders::renderMain() {
    drawText( textSheet, "THE LEGEND OF ZELDA: MAJORA'S MASK,DAY TWO", -0.8f, 0.1f, 0.04f, 1.0f, 0.4f, 0.8f, 1.0f);
    drawText( textSheet, "ROMANI'S COWS ARE IN TROUBLE AND LINK ISN'T AROUND.", -0.8f, -0.2f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
    drawText( textSheet, "ZELDA, YOUR JOB IS TO KILL THE ALIENS", -0.8f, -0.25f, 0.025f, 1.0f, 0.5f, 1.0f, 1.0f);
    drawText( textSheet, "BEFORE THEY STEAL ALL THE COWS. YOU HAVE", -0.8f, -0.3f, 0.025f, 1.0f, 0.5f, 1.0f, 1.0f);
    drawText( textSheet, "UNTIL SUNRISE.  IF YOU FAIL, ROMANI AND", -0.8f, -0.35f, 0.025f, 1.0f, 0.5f, 1.0f, 1.0f);
    drawText( textSheet, "HER COWS WILL BE ABDUCTED.", -0.8f, -0.4f, 0.025f, 1.0f, 0.5f, 1.0f, 1.0f);
    drawText( textSheet, "PRESS S WHEN YOU'RE FULLY PREPARED TO SAVE ROMANI'S COWS.", -0.8f, -0.8f, 0.03f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void SpaceInvaders::renderPause() {
    drawText( textSheet, "PAUSED", -0.8f, 0.1f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
    drawText( textSheet, "PRESS S TO RESUME.", -0.8f, 0.0f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void SpaceInvaders::renderLevel() {
    
    float lastFrameTicks = 0.0f;
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;

//    std::cout << elapsed << "\n";
    
    
    float spriteScale = 0.5f;
    
    glClearColor(elapsed/100.0, elapsed/70.0, elapsed/50.0, 1.0);
    drawText(textSheet, to_string(score) + " / " + to_string(barnAliens.size() * 10) + " ALIENS KILLED.", -0.9f, 0.9f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);

    if ( elapsed > 15.000 ) {
        drawText(textSheet, "HURRY! THE SUN IS ABOUT TO RISE.", -0.4f, -0.9f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    zeldas[0].drawSprite(spriteScale);
    
    for( int k = 0; k < barnAliens.size(); k++ ) {
        barnAliens[k].drawSprite(spriteScale / 1.5f);
    }
    
    for (int i = 0; i < bullets.size(); i++) {
        if (bullets[i].visible) {
            bullets[i].setY(bullets[i].getY() + 8.0*0.008);
        }
    }
    
    for (int j = 0; j < bullets.size(); j++) {
        if (bullets[j].visible) {
            bullets[j].drawSprite(spriteScale / 2.0f);
        }
    }
    
    float minX = barnAliens[0].getX();
    float maxX = barnAliens[0].getX();
    for (int counter = 0; counter < barnAliens.size(); counter++ ) {
        if (barnAliens[counter].getX() > maxX ) {
            maxX = barnAliens[counter].getX();
        }
    }
    
    if ( maxX > 0.9 ) movedirection = 0;
    else if (minX < -0.9) movedirection = 1;
    
    if (movedirection == 1) {
        for (int a = 0; a < barnAliens.size(); a++){
            barnAliens[a].setX(barnAliens[a].getX() + 0.001 );
            barnAliens[a].setY(barnAliens[a].getY() - 0.000006 + (rand() % 100 - 50 )/10000.0);
        }
    }
    else if (movedirection == 0) {
        for (int b = 0; b < barnAliens.size(); b++){
            barnAliens[b].setX(barnAliens[b].getX() - 0.001 );
            barnAliens[b].setY(barnAliens[b].getY() - 0.0006 + (rand() % 100 - 50 )/10000.0);
        }
    }
}

void SpaceInvaders::renderEnd() {
    GLuint textSheet = loadTexture("font.png");
    if ( score < 800 ) {
        glClearColor(0.1, 0.1, 0.1, 1.0);
        drawText(textSheet, "GAME OVER.  THE COWS AND ROMANI ARE BOTH GONE.", -0.8f, 0.4f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        glClearColor(0.8, 0.3, 0.6, 1.0);
        drawText(textSheet, "YOU SAVED ROMANI AND HER COWS.", -0.8f, 0.4f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);
        drawText(textSheet, "TERMINIA WILL NOT BE DEPRIVED OF MILK AFTER ALL!", -0.8f, 0.5f, 0.035f, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    drawText(textSheet, "PRESS R PLAY THE SONG OF TIME AND GO BACK TO DAWN OF DAY TWO.", -0.8f, -0.4f, 0.025f, 1.0f, 1.0f, 1.0f, 1.0f);
}

GLuint SpaceInvaders::loadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void SpaceInvaders::drawText( GLuint textTexture, string text, float x, float y, float size, float r, float g, float b, float a ) {
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
        vertexData.insert(
            vertexData.end(), { size*i + -1*size, size, size*i + -1*size, -1*size, size*i + size, -1*size, size*i + size, size });
        colorData.insert(
            colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        textCoordData.insert(
            textCoordData.end(), {textX, textY, textX, textY + textureSize, textX + textureSize, textY + textureSize, textX + textureSize, textY });
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

void SpaceInvaders::shootBullet( Sprite obj ) {
    if (cooldown > 20) {
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getY() > 1.0f) {
                bullets[i].visible = false;
            }
        }
        int index = 0;
        while ((bullets[index].visible) && index < MAX_BULLETS - 1) {
            index++;
            if (index == MAX_BULLETS) {
                index = 0;
            }
        }
        
        bullets[index].visible = true;
        bullets[index].setX(obj.getX());
        bullets[index].setY(obj.getY());
        
        cooldown = 0;
    }
}