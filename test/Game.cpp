
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
    
    Entity * player = new Entity(LoadTexture("laurasfirstsprite.png"), 36.0f/360.0f, 0.0f, 0.0f, 0.0f, false);
    entities.push_back(player);
    
    for ( size_t i = 0; i < 15; i++ ) {
        Entity * block = new Entity(LoadTexture("laurasfirstsprite.png"), 144.0f/360.0f, 0.0f/360.0f, -1.3f+i/10.0f, -0.5f, true);
        entities.push_back(block);
    }
    
}
void Game::Update(float elapsed) {
}
void Game::EventsFromCollision(Entity * e) {
    // Stop movement if colliding
    if ( e->collidesLeft ) {
        e->velocity_x = 0.0f;
        cout << "colliding left" << endl;
    }
    if ( e->collidesRight ) {
        e->velocity_x = 0.0f;
        cout << "colliding right" << endl;
    }
    if ( e->collidesTop ) {
        e->velocity_y = 0.0f;
        cout << "colliding top" << endl;
    }
    if ( e->collidesBottom ) {
        e->velocity_y = 0.0f;
        cout << "colliding bot" << endl;
    }
}
void Game::SetCollisionToFalse(Entity * e) {
    e->collidesLeft = false;
    e->collidesRight = false;
    e->collidesTop = false;
    e->collidesBottom = false;
}
void Game::Fall(Entity * e) {
    e->velocity_y += e->acceleration_y * FIXED_TIMESTEP;
    e->y -= e->velocity_y * FIXED_TIMESTEP;
}
void Game::CheckCollisionX(Entity * e) {
    for ( size_t j = 0; j < entities.size(); j++ ) {
        if ( &e != &entities[j]) { // are they the same entity?
            if ( e->CheckCollision(entities[j]) ) { // are they colliding?
                // this is where you do the penetration thing from the slides to check from which direction it collided from?
                // determine penetration on x/y axis, check to see from which direction it collided from, and then modify the x/y value so that it's not on top of the thing
                float x_distance = fabs( e->x - entities[j]->x );
                float x_penetration = fabs( x_distance - e->width/2 - entities[j]->width/2 );
                // check where it collided from
                if ( e->x > entities[j]->x ) {
                    // i collided on its left side (aka the left side is what collides)
                    // adjust position to slightly to the right
                    e->x -= x_penetration + 0.0001f;
                    e->collidesLeft = true;
                }
                else if ( e->x < entities[j]->x ) {
                    // i collided on its right side
                    // adjust position to slightly to the left
                    e->x -= x_penetration + 0.0001f;
                    e->collidesRight = true;
                }
            }
        }
    }
}
void Game::CheckCollisionY(Entity * e) {
    for ( size_t j = 0; j < entities.size(); j++ ) {
        if ( &e != &entities[j]) { // are they the same entity?
            if ( e->CheckCollision(entities[j]) ) { // are they colliding?
                // check y-axis
                float y_distance = fabs( e->y - entities[j]->y );
                float y_penetration = fabs( y_distance - e->height/2 - entities[j]->height/2 );
                // check where it collided from
                if ( e->y > entities[j]->y ) {
                    // i collided on its top side
                    // adjust position slightly up
                    e->y -= y_penetration - OFFSET;
                    e->collidesTop = true;
                }
                else if ( e->y < entities[j]->y ) {
                    // i collided on its bottom side
                    // adjust position sligtly down
                    e->y += y_penetration + OFFSET;
                    e->collidesBottom = true;
                    e->jumping = false;
                }
            }
        }
    }
}
float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

void Game::FixedUpdate() {
    
    for ( size_t i = 0; i < entities.size(); i++ ) {
        
        entities[i]->FixedUpdate();
        EventsFromCollision(entities[i]);
        SetCollisionToFalse(entities[i]);
        
        entities[i]->velocity_x = lerp(entities[i]->velocity_x, 0.0f, elapsed * entities[i]->friction_x);
        entities[i]->velocity_y = lerp(entities[i]->velocity_y, 0.0f, elapsed * entities[i]->friction_y);
        
        if ( !entities[i]->isStatic ) {
            Fall(entities[i]);
            //CheckCollisionX(entities[i]);
            CheckCollisionY(entities[i]);
        }
    }
}

void Game::Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    // Render each entity
    // Call RenderLevel.  This will render the level aka build the world
    RenderLevel();
    
    SDL_GL_SwapWindow(displayWindow);
}

bool Game::UpdateAndRender() {

    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;

    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            if ( event.key.keysym.scancode == SDL_SCANCODE_SPACE ) {
                entities[0]->Jump();
                entities[0]->jumping = true;
            }
            if ( event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) {
                if ( event.key.keysym.scancode == SDL_SCANCODE_LEFT ) entities[0]->dir_x = -1.0f;
                else if ( event.key.keysym.scancode == SDL_SCANCODE_RIGHT ) entities[0]->dir_x = 1.0f;
                entities[0]->MoveHoriz();
            }
        }
    }
    
    float fixedElapsed = elapsed + timeLeftOver;
    
    if ( fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS ) {
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    }
    
    while ( fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
        FixedUpdate();
    }
    
    // Update and render
    Update(elapsed);
    Render();
    
    return done;
}

void Game::RenderLevel() {
    
    for ( size_t i = 0; i < entities.size(); i++ ) entities[i]->Draw(0.5f);
    
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

bool Game::readHeader(std::ifstream &stream) {
    return true;
}

bool Game::readLayerData(std::ifstream &stream) {
    return true;
}

bool Game::readEntityData(std::ifstream &stream) {
    return true;
}

void Game::placeEntity(float placeX, float placeY) {
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
