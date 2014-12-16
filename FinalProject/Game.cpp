
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
    
    for ( size_t i = 0; i < world->statics.size(); i++ ) {
        delete world->statics[i];
    }
    
    delete world->player;
    delete world->player2;
    delete world->platform;
    
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

void Game::PlayerControls(const Uint8 *keys, SDL_Event event) {
    
    world->player->velocity_x = 0.0f;
    world->player2->velocity_x = 0.0f;
    
    // PLAYER 1 CONTROLS
    
    if ( keys[SDL_SCANCODE_LEFT] ) { // move left
        if ( world->player->floating )
            world->player->velocity_x = VELOCITY_X * -1 / 2.0;
        else
            world->player->velocity_x = VELOCITY_X * -1;
    }
    
    else if ( keys[SDL_SCANCODE_RIGHT] ) { // move right
        if ( world->player->floating )
            world->player->velocity_x = VELOCITY_X / 2.0;
        else
            world->player->velocity_x = VELOCITY_X;
    }
    
    else if ( keys[SDL_SCANCODE_DOWN] ) { // move down
        if ( world->player->floating ) {
            world->player->velocity_y += VELOCITY_Y * -1 / 50;
            world->player->velocity_x = 0.0f;
        }
    }
    
    else if ( keys[SDL_SCANCODE_UP] ) { // move up
        if ( world->player->floating ) {
            world->player->velocity_y += VELOCITY_Y / 50;
            world->player->velocity_x = 0.0f;
        }
    }
    
    else if ( keys[SDL_SCANCODE_L] ) { // float
        world->player->velocity_y = -0.5f;
        world->player->floating = true;
    }
    
    else if ( keys[SDL_SCANCODE_K] ) { // stationary shoot
        world->player->ShootProjectile(world->projectiles[0], world->player);
        world->player->notShooting = false;
    }
    
    // PLAYER 2 CONTROLS
    
    if ( keys[SDL_SCANCODE_Q] ) { // move left
        if ( world->player2->floating )
            world->player2->velocity_x = VELOCITY_X * -1 / 2.0;
        else
            world->player2->velocity_x = VELOCITY_X * -1;
    }
    
    else if ( keys[SDL_SCANCODE_E] ) {  // move right
        if ( world->player2->floating )
            world->player2->velocity_x = VELOCITY_X / 2.0;
        else
            world->player2->velocity_x = VELOCITY_X;
    }
    
    else if ( keys[SDL_SCANCODE_S] ) { // move down
        if ( world->player2->floating ) {
            world->player2->velocity_y += VELOCITY_Y * -1 / 50;
            world->player2->velocity_x = 0.0f;
        }
    }
    
    else if ( keys[SDL_SCANCODE_2] ) { // move up
        if ( world->player2->floating ) {
            world->player2->velocity_y += VELOCITY_Y / 50;
            world->player2->velocity_x = 0.0f;
        }
    }
    
    else if ( keys[SDL_SCANCODE_F] ) { // float
        world->player2->velocity_y = -0.5f;
        world->player2->floating = true;
    }
    
    else if ( keys[SDL_SCANCODE_W] ) { // stationary shoot
        world->player2->ShootProjectile(world->projectiles[1], world->player2);
        world->player2->notShooting = false;
    }
    
    // Player 1 and 2 jump & shoot
    
    while (SDL_PollEvent(&event)) {
        
        if (event.type == SDL_KEYDOWN ) {
            
            // Player 1 jump
            
            if ( event.key.keysym.scancode == SDL_SCANCODE_SEMICOLON && !event.key.repeat ) {
                world->player->floating = false;
                if ( world->player->collidesBottom )
                    world->player->collidesBottom = false;
                if ( !world->player->jumping )
                    world->player->velocity_y = 4.0f;
            }
            
            // Player 1 shoot
            
            if ( event.key.keysym.scancode == SDL_SCANCODE_K && !event.key.repeat ) {
                if ( world->player->velocity_x != 0.0f ) {
                    world->player->ShootProjectile(world->projectiles[0], world->player);
                    world->player->notShooting = false;
                }
            }
            
            // Player 2 jump
            
            if ( event.key.keysym.scancode == SDL_SCANCODE_R && !event.key.repeat ) {
                world->player2->floating = false;
                if ( world->player2->collidesBottom )
                    world->player2->collidesBottom = false;
                if ( !world->player2->jumping )
                    world->player2->velocity_y = 4.0f;
            }
            
            // Player 2 shoot
            
            if ( event.key.keysym.scancode == SDL_SCANCODE_W && !event.key.repeat ) {
                if ( world->player2->velocity_x != 0.0f ) {
                    world->player2->ShootProjectile(world->projectiles[1], world->player2);
                    world->player2->notShooting = false;
                }
            }
        }
    }
    
    // Player 1 & 2 reactions
    
    if ( world->player->collidesBottom ) {
        world->player->velocity_y = 0.0f;
        //shake = true;
        world->player->shaking = true;
    }
    
    if ( world->player2->collidesBottom ) {
        world->player2->velocity_y = 0.0f;
        //shake = true;
        world->player2->shaking = true;
    }
    
    if ( world->player->collidesTop ) {
        world->player->velocity_y = VELOCITY_Y * -1;
    }
    
    if ( world->player2->collidesTop ) {
        world->player->velocity_y = VELOCITY_Y * -1;
    }
    
    if ( world->player->collidesLeft ) {
        world->player->velocity_x = 0;
    }
    
    if ( world->player2->collidesLeft ) {
        world->player2->velocity_x = 0;
    }
    
    if ( world->player->collidesRight ) {
        world->player->velocity_x = 0;
    }
    
    if ( world->player2->collidesRight ) {
        world->player2->velocity_x = 0;
    }
    
    if ( world->player->x > 1.33 ) {
        world->player->x = -1.33;
    }
    
    if ( world->player2->x > 1.33 ) {
        world->player2->x = -1.33;
    }
    
    if ( world->player->y < -1.33 ) {
        world->player->y = 1.33;
    }
    
    if ( world->player2->y < -1.33 ) {
        world->player2->y = 1.33;
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
            e->lastDirection = -1;
        }
        
        else if ( e->velocity_x > 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 2.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 5.0;
            if ( world->inSpace ) e->v = TILEHEIGHT * 8.0;
            e->lastDirection = 1;
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
            e->lastDirection = -1;
        }
        
        if ( e->velocity_x > 0 ) {
            if ( world->raining ) e->v = TILEHEIGHT * 2.0;
            if ( world->snowing ) e->v = TILEHEIGHT * 5.0;
            if ( world->inSpace ) e->v = TILEHEIGHT * 8.0;
            e->lastDirection = 1;
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

void Game::ShootProjectile(Entity * e, Entity * p) {
    
    e->acceleration_x = ACCELERATION_X * 15.0;
    if ( p->velocity_x < 0.0 ) e->acceleration_x = ACCELERATION_X * -15.0;
    else if ( p->velocity_x == 0.0 ) e->acceleration_x = ACCELERATION_X * 15.0 * p->lastDirection;
    e->acceleration_y = ACCELERATION_Y;

    e->velocity_x += e->acceleration_x * FIXED_TIMESTEP;
    e->x += e->velocity_x * FIXED_TIMESTEP;
    
    e->velocity_y += e->acceleration_y * FIXED_TIMESTEP;
    e->y += e->velocity_y * FIXED_TIMESTEP;

    if ( e->x > 1.33 && e->x < 5.32 ) {
        e->acceleration_x = ACCELERATION_X;
        //shake = true;
    }
    else if ( e->x < -1.33 && e->x > -5.32 ) {
        e->acceleration_x = -ACCELERATION_X;
        //shake = true;
    }
    else if ( e->x <= -5.32 || e->x >= 5.32 ) {
        p->notShooting = true;
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
    
    if ( world->projectiles[0]->collidesWith(world->player2) ) {
        // crit chance
        int crit = rand() % 50;
        if ( crit > 20 ) crit = 50;
        else crit = 0;
        int dmg = 10 + rand() % 10 + crit;
        cout << crit << endl;
        world->projectiles[0]->onHitDamage = dmg;
        shake = true;
        world->player2->damage += dmg;
        world->player->velocity_y = 4.0f;
        world->player2->floating = false;
        world->projectiles[0]->velocity_x = 0.0;
        // KO
        if ( crit > 40 ) {
            world->player2->velocity_x *= 20;
            world->player2->damage = 0;
            cout << "SHOULD FLY" << endl;
        }
    }
    else {
        world->projectiles[0]->onHitDamage = 1;
    }
//    if ( world->projectiles[1]->collidesWith(world->player) ) {
//        // crit chance
//        crit = rand() % 50;
//        if ( crit > 40 ) crit = 50;
//        else crit = 0;
//        dmg = 10 + rand() % 10 + crit;
//        shake = true;
//        world->player->damage += dmg;
//        world->player->floating = false;
//        world->player->hit = true;
//        world->projectiles[1]->velocity_x = 0.0;
//        world->projectiles[1]->onHitDamage = dmg;
//        
//        // KO
//        if ( crit > 40 ) {
//            world->player->velocity_y = 4.0f;
//            world->player->damage = 0;
//        }
//    }
//    else {
//        world->projectiles[1]->onHitDamage = 1;
//    }
    
    //cout << world->player2->floating << endl;
}

void Game::ProjectileCheck(Entity * e) {
    if ( e->player1 ) {
        if ( e->notShooting )
            world->projectiles[0]->x = -4.0;
        else {
            ShootProjectile(world->projectiles[0], e);
            world->projectiles[0]->y = e->y;
        }
    }
    if ( e->player2 ) {
        if ( e->notShooting )
            world->projectiles[1]->x = -4.0;
        else {
            ShootProjectile(world->projectiles[1], e);
            world->projectiles[1]->y = e->y;
        }
    }
}

void Game::FixedUpdate() {
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    
    PlayerControls(keys, event);
    PlayerBehavior(world->player);
    PlayerBehavior(world->player2);
    
    world->WeatherCheck();
    world->Lightning();
    world->MovePlatform();
    world->AnchorStatics();
    
    // do game effects, world effects, world->player effects
    if ( world->raining) world->Rain();
    if ( world->snowing ) world->Snow();
    if ( world->inSpace ) world->Space();
    
    ProjectileCheck(world->player);
    ProjectileCheck(world->player2);
    
    CollisionCheck(world->player);
    CollisionCheck(world->player2);
    
    CollisionCheck(world->projectiles[0]);
    CollisionCheck(world->projectiles[1]);
    
    if ( shake ) {
        world->player->velocity_x = 0.0f;
        world->player->velocity_y = 1.0f;
        world->player2->velocity_x = 0.0f;
        world->player2->velocity_y = 1.0f;
    }
    
    else {
        world->player->shaking = false;
        world->player2->shaking = false;
    }
    
    // jump shake range cutoff
    float jumpshake;
    if ( world->player->shaking ) jumpshake = fabs((world->player->y - world->player->height/2) - (world->platform->y + world->platform->height/2));
    else jumpshake = fabs((world->player2->y - world->player2->height/2) - (world->platform->y + world->platform->height/2));
    // check jumpshake range
    if ( jumpshake > 0.095 & jumpshake < 0.1 ) shake = false;
    
    
    // movement p1
    if ( !world->player->floating && !world->player->shaking ) world->player->Go(world->platform->y);
    else if ( !world->player->hit ) world->player->Float(world->platform->y);
    // movement p2
    if ( !world->player2->floating && !world->player2->shaking ) world->player2->Go(world->platform->y);
    else if ( !world->player2->hit ) world->player2->Float(world->platform->y);
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
        world->projectiles[j]->Draw(SCALE/2 + world->projectiles[j]->velocity_x/5 * world->projectiles[j]->onHitDamage);
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
        world->statics[k]->Draw(SCALE * 1.2);
    }
    
    //DrawText( GLuint textTexture, std::string text, float x, float y, float spacing, float size, float r, float g, float b, float a )
    world->DrawText(world->LoadTexture("pixel_font.png"), to_string(world->player->damage) + "%", -0.15f, -0.92, FONT_SPACING, FONT_SIZE, 1.0, 1.0, 1.0, 1.0 );
    world->DrawText(world->LoadTexture("pixel_font.png"), to_string(world->player2->damage) + "%", 0.02f, -0.92, FONT_SPACING, FONT_SIZE, 1.0, 1.0, 1.0, 1.0 );
    
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