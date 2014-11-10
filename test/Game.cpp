
#include "Game.h"

Game::Game() {
    Init();
    done = false;
    lastFrameTicks = 0.0f;
    timeLeftOver = 0.0f;
}
Game::~Game() {
    SDL_Quit();
}
void Game::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("laupsygame",
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     800, 600,
                                     SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    glViewport(0,0,800,600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glClearColor(0.55, 0.76, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    Entity * player = new Entity(LoadTexture("laurasfirstsprite.png"), 36.0f/360.0f, 0.0f, 0.0f, 0.0f);
    player->MakeLivingEntity();
    entities.push_back(player);
    
    for ( size_t i = 0; i < 15; i++ ) {
        Entity * block = new Entity(LoadTexture("laurasfirstsprite.png"), 144.0f/360.0f, 0.0f/360.0f, -1.3f+i/10.0f, -0.5f);
        entities.push_back(block);
    }
}
void Game::Update(float elapsed) {}
float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

void Game::FixedUpdate() {
    
    for ( size_t i = 0; i < entities.size(); i++ ) {
        
        entities[i]->FixedUpdate();
        
    }
}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    RenderLevel();
    SDL_GL_SwapWindow(displayWindow);
}

bool Game::UpdateAndRender() {
    
    Entity * player = entities[0];

    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    float fixedElapsed = elapsed + timeLeftOver;
    
    if ( fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS ) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    
    while ( fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
        FixedUpdate();
    }
    
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
            done = true;
        
        
        if ( event.key.keysym.scancode == SDL_SCANCODE_LEFT ) {
            player->dir_x = -1.0f;
            if (event.type == SDL_KEYUP)
                player->ResetX();
        }
        
        if ( event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) {
            player->dir_x = 1.0f;
            if (event.type == SDL_KEYUP)
                player->ResetX();
        }

        else if (event.type == SDL_KEYDOWN ) {
            
            if ( event.key.keysym.scancode == SDL_SCANCODE_SPACE )
                player->dir_y = 1.0f;
            
            cout << player->dir_x << endl;
        }
    }
    
    // Update and render
    Update(elapsed);
    Render();
    
    return done;
}

void Game::RenderLevel() {
    
    for ( size_t i = 0; i < entities.size(); i++ )
        entities[i]->Draw(0.5f);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
    }
}

void Game::RenderStart() {
}

void Game::RenderGameOver() {
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
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size});
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        textCoordData.insert(textCoordData.end(),
                             {   textX,
                                 textY,
                                 textX,
                                 textY + textureSize,
                                 textX + textureSize,
                                 textY + textureSize,
                                 textX + textureSize,
                                 textY
                             });
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
