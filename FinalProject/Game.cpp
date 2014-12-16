
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
    
    world->player2 = new Entity( world->LoadTexture(world->spriteSheet), TILEWIDTH * 1.0, TILEHEIGHT * 0.0, 0.0f, DEFAULT_Y, true );
    world->player2->player2 = true;
    
    world->platform = new Entity(world->LoadTexture(world->spriteSheet), TILEWIDTH * 4.0, TILEHEIGHT * 0.0, 0.0, DEFAULT_Y - 0.7);
    world->platform->width = TILEWIDTH * 6.0;
    world->platform->height = TILEHEIGHT * 2.0;
    
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
    
    shake = false;
}

void Game::PlayerControls(Entity * e, const Uint8 *keys, SDL_Event event) {
    
    Uint8 ctrl_left, ctrl_right, ctrl_up, ctrl_down;
    Uint8 ctrl_float;
    SDL_Scancode ctrl_shootProjectile, ctrl_jump;
    
    // Player 1 controls
    
    if ( e->player1 ) {
        ctrl_left = keys[SDL_SCANCODE_LEFT];
        ctrl_right = keys[SDL_SCANCODE_RIGHT];
        ctrl_up = keys[SDL_SCANCODE_UP];
        ctrl_down = keys[SDL_SCANCODE_DOWN];
        ctrl_jump = SDL_SCANCODE_SEMICOLON;
        ctrl_shootProjectile = SDL_SCANCODE_K;
        ctrl_float = keys[SDL_SCANCODE_L];
    }
    
    // Player 2 controls
    
    else {
        ctrl_left = keys[SDL_SCANCODE_Q];
        ctrl_right = keys[SDL_SCANCODE_E];
        ctrl_up = keys[SDL_SCANCODE_2];
        ctrl_down = keys[SDL_SCANCODE_S];
        ctrl_jump = SDL_SCANCODE_R;
        ctrl_shootProjectile = SDL_SCANCODE_W;
        ctrl_float = keys[SDL_SCANCODE_F];
    }
    
    if ( ctrl_left ) {
        if ( e->floating )
            e->velocity_x = VELOCITY_X * -1 / 2.0;
        else
            e->velocity_x = VELOCITY_X * -1;
    }
    
    else if ( ctrl_right ) {
        if ( e->floating )
            e->velocity_x = VELOCITY_X / 2.0;
        else
            e->velocity_x = VELOCITY_X;
    }
    else if ( ctrl_down ) {
        if ( e->floating ) {
            e->velocity_y += VELOCITY_Y * -1 / 50;
            e->velocity_x = 0.0f;
        }
    }
    else if ( ctrl_up ) {
        if ( e->floating ) {
            e->velocity_y += VELOCITY_Y / 50;
            e->velocity_x = 0.0f;
        }
    }
    else if ( ctrl_float ) {
        e->velocity_y = -0.5f;
        e->floating = true;
    }
    else
        e->velocity_x = 0.0f;
    
    if ( e->collidesBottom ) {
        e->velocity_y = 0.0f;
        shake = true;
    }
    
    if ( e->collidesTop ) {
        e->velocity_y = VELOCITY_Y * -1;
    }
    
    if ( e->collidesLeft ) {
        e->velocity_x = 0;
    }
    
    if ( e->collidesRight ) {
        e->velocity_x = 0;
    }
    
    if ( e->x > 1.33 ) {
        e->x = -1.33;
    }
    
    if ( e->x < -1.33 ) {
        e->x = 1.33;
    }
    
    while (SDL_PollEvent(&event)) {
        
        if (event.type == SDL_KEYDOWN ) {
            
            if ( event.key.keysym.scancode == ctrl_jump && !event.key.repeat ) {
                
                e->floating = false;
                
                if ( e->collidesBottom ) {
                    e->collidesBottom = false;
                }
                
                if ( !e->jumping ) {
                    e->velocity_y = 4.0f;
                }
                
                // glScalef(1.0, -1.0, 1.0); this would be cool for like a reverse gravity mode
                
            }
            
            if ( event.key.keysym.scancode == ctrl_shootProjectile && !event.key.repeat ) {
                if ( e->player1 && e->velocity_x != 0.0f ) {
                    e->ShootProjectile(world->projectiles[0], e);
                    e->notShooting = false;
                }
            }
        }
    }
}

void Game::PlayerBehavior(Entity * e) {
    
    if ( e->floating ) {
        if ( world->raining ) e->v = TILEHEIGHT * 0.0;
        if ( world->snowing ) e->v = TILEHEIGHT * 4.0;
        if ( world->inSpace ) e->v = TILEHEIGHT * 7.0;
        
        if ( e->velocity_x < 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 3.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 6.0;
            if ( world->inSpace ) e->v = TILEHEIGHT * 9.0;
        }
        
        else if ( e->velocity_x > 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 2.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 5.0;
            if ( world->inSpace ) e->v = TILEHEIGHT * 8.0;
        }
        
        else if ( fabs(e->velocity_y) >= 0.4 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 1.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 4.0;
            if ( world->inSpace ) e->v = TILEHEIGHT * 7.0;
        }
    }
    else {
        if ( world->raining ) e->v = TILEHEIGHT * 0.0;
        if ( world->snowing ) e->v = TILEHEIGHT * 4.0;
        if ( world->inSpace ) e->v = TILEHEIGHT * 7.0;
        
        if ( e->velocity_x < 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 3.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 6.0;
            if ( world->inSpace ) e->v = TILEHEIGHT * 9.0;
        }
        
        if ( e->velocity_x > 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 2.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 5.0;
            if ( world->inSpace ) e->v = TILEHEIGHT * 8.0;
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

void Game::ShootProjectile(Entity * e) {
    
    e->acceleration_x = ACCELERATION_X * 15.0;
    if ( world->player->velocity_x < 0.0 ) e->acceleration_x = ACCELERATION_X * -15.0;
    e->acceleration_y = ACCELERATION_Y;

    e->velocity_x += e->acceleration_x * FIXED_TIMESTEP;
    e->x += e->velocity_x * FIXED_TIMESTEP;
    
    e->velocity_y += e->acceleration_y * FIXED_TIMESTEP;
    e->y += e->velocity_y * FIXED_TIMESTEP;

    if ( e->x > 1.33 && e->x < 5.32 ) {
        e->acceleration_x = ACCELERATION_X;
        shake = true;
    }
    else if ( e->x < -1.33 && e->x > -5.32 ) {
        e->acceleration_x = -ACCELERATION_X;
        shake = true;
    }
    else if ( e->x <= -5.32 || e->x >= 5.32 ) {
        world->player->notShooting = true;
        shake = false;
    }
    else {
        shake = false;
    }
}

void Game::CollisionCheck(Entity * e) {
    
    e->collidesTop = false;
    e->collidesBottom = false;
    e->collidesLeft = false;
    e->collidesRight = false;
    e->collidesWith(world->platform);
    
}

void Game::ProjectileCheck() {
    if ( world->player->player1 ) {
        if ( world->player->notShooting )
            world->projectiles[0]->x = -4.0;
        else {
            ShootProjectile(world->projectiles[0]);
            world->projectiles[0]->y = world->player->y;
        }
    }
    if ( world->player->player2 ) {
        if ( world->player->notShooting )
            world->projectiles[1]->x = -4.0;
        else {
            ShootProjectile(world->projectiles[1]);
            world->projectiles[1]->y = world->player->y;
        }
    }
}

void Game::FixedUpdate() {
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    
    PlayerControls(world->player, keys, event);
    PlayerBehavior(world->player);
    
    world->WeatherCheck();
    world->Lightning();
    world->MovePlatform();
    world->AnchorStatics();
    
    // do game effects, world effects, world->player effects
    if ( world->raining) world->Rain();
    if ( world->snowing ) world->Snow();
    if ( world->inSpace ) world->Space();
    
    ProjectileCheck();
    CollisionCheck(world->player);
    CollisionCheck(world->player2);
    
    if ( shake ) {
        world->player->velocity_x = 0.0f;
        world->player->velocity_y = 1.0f;
    }
    
    // jump shake range cutoff
    float jumpshake = fabs((world->player->y - world->player->height/2) - (world->platform->y + world->platform->height/2));
    
    if ( jumpshake > 0.095 & jumpshake < 0.1 ) shake = false;
    
    if ( !world->player->floating && !shake ) world->player->Go(world->platform->y);
    else world->player->Float(world->platform->y);
}

void Game::Render() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    if ( world->player->y > DEFAULT_Y )
        camY = world->platform->y * -1 - 0.5;
    
    if ( shake == true ) {
        glTranslatef( (rand() % 6 - 3)/ 100.0, camY + (rand() % 8 - 4)/100.0, 0.0f);
    }
    
    else {
        glTranslatef(0.0f, camY, 0.0f);
    }
    
    for ( size_t i = 0; i < world->blocks.size(); i++ ) {
        world->blocks[i]->Draw(SCALE);
    }
    
    for ( size_t j = 0; j < world->projectiles.size(); j++ ) {
        world->projectiles[j]->Draw(SCALE);
    }
    
    // draw world->player last so not overlapped by solids
    world->platform->Draw(SCALE);
    world->player->Draw(SCALE);
    world->player2->Draw(SCALE);
    
    for ( size_t h = 0; h < world->rain.size(); h++ ) {
        world->rain[h]->Draw(SCALE);
    }
    
    // STATICS
    for ( size_t k = 0; k < world->statics.size(); k++ ) {
        world->statics[k]->Draw(SCALE);
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