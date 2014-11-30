
#include "Game.h"

Game::Game() {
    Init();
    LoadObjects();
    Loop();
}

void Game::LoadObjects() {
    
    world = new World();
    world->player = new Entity( world->LoadTexture(world->spriteSheet), TILEWIDTH * 2.0, TILEHEIGHT * 0.0, 0.0f, DEFAULT_Y, true );
    world->player->player1 = true;
    
    world->PlaceBlocks();
    
}
void Game::Loop() {
    
    done = false;
    lastFrameTicks = 0.0f;
    timeLeftOver = 0.0f;
    
    // any other game info that i might want loop eg score
    /////
}

Game::~Game() {
    Mix_FreeMusic(music);
    
    for ( size_t i = 0; i < world->blocks.size(); i++ ) {
        delete world->blocks[i];
    }
    
    for ( size_t i = 0; i < world->rain.size(); i++ ) {
        delete world->rain[i];
    }
    
    SDL_Quit();
}

void Game::Init() {
    
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("laupsygame",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800, 600,SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    // init audo
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    
    // window stuff
    glViewport(0,0,800,600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-2.33, 2.33, -1.0, 1.0, -1.0, 1.0);
    
    // window color
    glClearColor(0.1, 0.11, 0.13, 1.0);
    
    // reset frame
    glClear(GL_COLOR_BUFFER_BIT);
    
    // init music
    music = Mix_LoadMUS("battlestargalactica_roslinandadama.mp3");
    thunder = Mix_LoadWAV("thunder.wav");
    
    // play music
    //Mix_PlayMusic(music, -1);
}

void Game::PlayerControls(Entity * e, const Uint8 *keys, SDL_Event event) {
    
    if ( keys[SDL_SCANCODE_LEFT] ) {
        if ( e->floating )
            e->velocity_x = VELOCITY_X * -1 / 2.0;
        else
            e->velocity_x = VELOCITY_X * -1;
    }
    
    else if ( keys[SDL_SCANCODE_RIGHT] ) {
        if ( e->floating )
            e->velocity_x = VELOCITY_X / 2.0;
        else
            e->velocity_x = VELOCITY_X;
    }
    else if ( keys[SDL_SCANCODE_DOWN] ) {
        if ( e->floating ) {
            e->velocity_y += VELOCITY_Y * -1 / 50;
            e->velocity_x = 0.0f;
        }
    }
    else if ( keys[SDL_SCANCODE_UP] ) {
        if ( e->floating ) {
            e->velocity_y += VELOCITY_Y / 50;
            e->velocity_x = 0.0f;
        }
    }
    else if ( keys[SDL_SCANCODE_F] ) {
        e->velocity_y = -0.5f;
        e->floating = true;
    }
    else
        e->velocity_x = 0.0f;
    
    if ( world->player->collidesBottom ) {
        world->player->velocity_y = 0.0f;
    }
    
    if ( world->player->collidesTop ) {
        world->player->velocity_y = VELOCITY_Y * -1;
    }
    
    if ( world->player->collidesLeft ) {
        world->player->velocity_x = 0;
    }
    
    if ( world->player->collidesRight ) {
        world->player->velocity_x = 0;
    }
    
    if ( world->player->x > 1.33 ) {
        world->player->x = -1.33;
    }
    
    if ( world->player->x < -1.33 ) {
        world->player->x = 1.33;
    }
    
    while (SDL_PollEvent(&event)) {
        
        if (event.type == SDL_KEYDOWN ) {
            
            if ( event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat ) {
                
                
                world->player->floating = false;
                
                if ( world->player->collidesBottom ) {
                    world->player->collidesBottom = false;
                }
                
                if ( !world->player->jumping ) {
                    world->player->velocity_y = 4.0f;
                }
                
                // glScalef(1.0, -1.0, 1.0); this would be cool for like a reverse gravity mode
                
            }
        }
    }
}

void Game::PlayerBehavior(Entity * e) {
    
    if ( e->floating ) {
        if ( world->raining ) e->v = TILEHEIGHT * 0.0;
        if ( world->snowing ) e->v = TILEHEIGHT * 0.0;
        
        if ( e->velocity_x < 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 3.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 3.0;
        }
        
        if ( e->velocity_x > 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 2.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 2.0;
        }
        
        if ( fabs(e->velocity_y) >= 0.4 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 1.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 1.0;
        }
    }
    else {
        
        if ( world->raining ) e->v = TILEHEIGHT * 0.0;
        if ( world->snowing ) e->v = TILEHEIGHT * 0.0;
        
        if ( e->velocity_x < 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 3.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 3.0;
        }
        
        if ( e->velocity_x > 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 2.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 2.0;
        }
    }
}

void Game::Update(float elapsed) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
    }
}

void Game::CollisionCheck() {
    
    world->player->collidesTop = false;
    world->player->collidesBottom = false;
    world->player->collidesLeft = false;
    world->player->collidesRight = false;
    for ( size_t i = 0; i < world->blocks.size(); i++ ) { world->player->collidesWith(world->blocks[i]); }
}

void Game::FixedUpdate() {
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    
    PlayerControls(world->player, keys, event);
    PlayerBehavior(world->player);
    CollisionCheck();
    world->WeatherCheck();
    world->Lightning();
    
    // do game effects, world effects, world->player effects
    if ( world->raining) world->Rain();
    if ( world->snowing ) world->Snow();
    
    if ( !world->player->floating ) world->player->Go();
    else world->player->Float();
}

void Game::Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    if ( world->player->y > DEFAULT_Y )
        camY = world->player->y * -1;
    
    glTranslatef(0.0f, camY, 0.0f);
    
    for ( size_t i = 0; i < world->blocks.size(); i++ ) {
        world->blocks[i]->Draw(SCALE);
    }
    
    // draw world->player last so not overlapped by solids
    world->player->Draw(SCALE);
    
    for ( size_t h = 0; h < world->rain.size(); h++ ) {
        world->rain[h]->Draw(SCALE);
    }
    
    ///////
    // Lock camera onto world->player
    ////// will need to change camera position so that both world->players are always visible
    ///////// do this after all single-world->player game stuff is set, then add second world->player and go from there?
    ///////
    glPopMatrix();
    SDL_GL_SwapWindow(displayWindow);
}

bool Game::UpdateAndRender() {
    // time elapsed stuff
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    // fixed time elapsed stuff
    float fixedElapsed = elapsed + timeLeftOver;
    if ( fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS )
        fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
    // update timestep
    while ( fixedElapsed >= FIXED_TIMESTEP ) {
        fixedElapsed -= FIXED_TIMESTEP;
        FixedUpdate();
    }
    
    timeLeftOver = fixedElapsed;
    
    // update and render
    Update(elapsed);
    Render();
    
    // quit if applicable
    return done;
}