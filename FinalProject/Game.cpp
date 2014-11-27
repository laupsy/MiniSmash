
#include "Game.h"

Game::Game() {
    Init();
    LoadObjects();
    Loop();
}

void Game::LoadObjects() {
    
    world = new World();
    player = new Entity(world->LoadTexture(world->spriteSheet), TILEWIDTH * playerFloating, TILEHEIGHT * cat, 0.0f, DEFAULT_Y);
    player->floating = true;
    
    // create raindrops
    for ( int i = 0; i < 400; i++ ) {
        Entity * raindrop = new Entity(world->LoadTexture(world->spriteSheet), TILEWIDTH * 3.0, TILEHEIGHT * 2.0, -1.33f + i / 100.0, player->y + rand() % 10 - 5);
        rain.push_back(raindrop);
    }
    
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
    glClearColor(0.2, 0.22, 0.26, 1.0);
    
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
        
        if ( raining ) e->u = TILEWIDTH * playerWalkingLeft;
        if ( snowing ) e->u = TILEWIDTH * earmuffsWalkingLeft;
        
        if ( e->floating )
            e->velocity_x = VELOCITY_X * -1 / 10.0;
        else
            e->velocity_x = VELOCITY_X * -1;
    }
    
    else if ( keys[SDL_SCANCODE_RIGHT] ) {
        
        if ( raining ) e->u = TILEWIDTH * playerWalkingRight;
        if ( snowing ) e->u = TILEWIDTH * earmuffsWalkingRight;
        
        if ( e->floating )
            e->velocity_x = VELOCITY_X / 10.0;
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
    
    if ( player->collidesBottom ) {
        player->velocity_y = 0.0f;
    }
    
    if ( player->collidesTop ) {
        player->velocity_y = VELOCITY_Y * -1;
    }
    
    if ( player->collidesLeft ) {
        player->velocity_x = 0;
    }
    
    if ( player->collidesRight ) {
        player->velocity_x = 0;
    }
    
    if ( player->x > 1.33 ) {
        player->x = -1.33;
    }
    
    if ( player->x < -1.33 ) {
        player->x = 1.33;
    }
    
    while (SDL_PollEvent(&event)) {
        
        if (event.type == SDL_KEYDOWN ) {
            
            if ( event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat ) {
                
                player->floating = false;
                
                if ( player->collidesBottom ) {
                    player->collidesBottom = false;
                }
                
                if ( !player->jumping ) {
                    player->velocity_y = 5.0f;
                }
                
                // glScalef(1.0, -1.0, 1.0); this would be cool for like a reverse gravity mode
                
            }
        }
    }
}

void Game::PlayerBehavior(Entity * e) {
    if ( e->floating ) {
        if ( raining ) e->u = TILEWIDTH * playerFloating;
        if ( snowing ) e->u = TILEWIDTH * earmuffsFloating;
    }
    else {
        if ( e->velocity_x < 0 ) {
            if ( raining ) e->u = TILEWIDTH * playerWalkingLeft;
            if ( snowing ) player->u = TILEWIDTH * earmuffsWalkingLeft;
        }
        else if ( e->velocity_x > 0 ) {
            if ( raining ) e->u = TILEWIDTH * playerWalkingRight;
            if ( snowing ) e->u = TILEWIDTH * earmuffsWalkingRight;
        }
        else {
            if ( raining ) e->u = TILEWIDTH * playerStanding;
            if ( snowing ) e->u = TILEWIDTH * earmuffsFloating;
        }
    }
}

void Game::Lightning() {
    if ( raining && ( SDL_GetTicks() % 123 == 0 || SDL_GetTicks() % 124 == 0 ) ) {
        glClearColor(0.4, 0.42, 0.46, 1.0);
        Mix_PlayChannel(-1, thunder, 0);
    }
    else {
        glClearColor(fabs(0.2 + player->y / 25.0), fabs(0.22 + player->y / 25.0), fabs(0.26 + player->y / 10.0), 1.0);
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
    
    player->collidesTop = false;
    player->collidesBottom = false;
    player->collidesLeft = false;
    player->collidesRight = false;
    
    for ( size_t i = 0; i < world->blocks.size(); i++ ) { player->collidesWith(world->blocks[i]); }
}

void Game::WeatherCheck() {
    
    raining = false;
    snowing = false;
    inSpace = false;
    
    if ( player->y < 3.0 ) raining = true;
    else if ( player->y >= 3.0 ) snowing = true;
    else inSpace = false;
}

void Game::FixedUpdate() {
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    
    PlayerControls(player, keys, event);
    PlayerBehavior(player);
    CollisionCheck();
    WeatherCheck();
    
    // do game effects, world effects, player effects
    Lightning();
    Rain();
    
    if ( !player->floating )
        player->Go();
    else
        player->Float();
}

void Game::Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    if ( player->y > DEFAULT_Y )
        camY = player->y * -1;
    
    glTranslatef(0.0f, camY, 0.0f);
    
    // Render the level (things includes drawing stuff
    for ( size_t k = 0; k < clouds.size(); k++ ) {
        clouds[k]->Draw(SCALE);
    }
    for ( size_t j = 0; j < bg.size(); j++ ) {
        bg[j]->Draw(SCALE);
    }
    for ( size_t i = 0; i < world->blocks.size(); i++ ) {
        world->blocks[i]->Draw(SCALE);
    }
    
    // draw player last so not overlapped by solids
    player->Draw(SCALE);
    
    for ( size_t h = 0; h < rain.size(); h++ ) {
        rain[h]->Draw(SCALE * ( rand() % 10 / 10.0 ));
    }
    
    ///////
    // Lock camera onto player
    ////// will need to change camera position so that both players are always visible
    ///////// do this after all single-player game stuff is set, then add second player and go from there?
    ///////
    glPopMatrix();
    SDL_GL_SwapWindow(displayWindow);
}

void Game::Rain() {
    for ( size_t i = 0; i < rain.size(); i++ ) {
        
        if ( raining ) {
            rain[i]->u = TILEWIDTH * 3.0;
            rain[i]->velocity_y = (rand() % 6 * -1);
            rain[i]->velocity_x = 0.0f;
            rain[i]->floating = false;
            rain[i]->Go();
        }
        else if ( snowing ) {
            rain[i]->u = TILEWIDTH * 4.0;
            rain[i]->Flutter();
            if ( rain[i]->x < player->x - 0.66 ) rain[i]->FlutterLeft();
        }
        
        if ( rain[i]->y < player->y - 1.33 ) rain[i]->y = player->y + 1.33;
    }
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